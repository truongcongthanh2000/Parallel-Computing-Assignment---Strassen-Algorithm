/*
* author: Phú
* contributor: Thành
*/

#pragma once

#include "decorator.h"
#include "naive_serial.h"

#include <algorithm>
#include <functional>
#include <numeric>

using namespace std;


template <class T = int, size_t B = 1>
class StrassenSerialLazy : public Decorator<T> {
public:
    Matrix<T> operator ()(array<Matrix<T>, 2>& mats) const override {
        // multiply
        return this->strassen(mats);
    }

private:
    Matrix<T> strassen(array<Matrix<T>, 2>& mats) const {
        // get 3 dimensions
        const array<size_t, 3> dimensions = { mats[0].size(0), mats[0].size(1), mats[1].size(1) };

        // check base case
        if (this->reached_base_dimensions(dimensions)) {
            return NaiveSerial<T>()(mats);
        }

        // divide with resizing
        // resize
        auto dimensions_new = this->dimensions_normalized(dimensions);
        for (size_t i = 0; i < mats.size(); ++i) {
            mats[i].resize(dimensions_new[i], dimensions_new[i + 1]);
        }

        array<size_t, 3> dimensions_sub;
        transform(begin(dimensions_new), end(dimensions_new), begin(dimensions_sub), [](size_t x) { return x >> 1; });  // this is dimensions / 2

        // copy into submatrices
        array<array<array<Matrix<T>, 2>, 2>, 2> mats_sub;
        for (size_t i = 0; i < mats_sub.size(); ++i) {
            for (size_t j = 0; j < mats_sub[i].size(); ++j) {
                for (size_t k = 0; k < mats_sub[i][j].size(); ++k) {
                    mats_sub[i][j][k] = Matrix<T>(dimensions_sub[i], dimensions_sub[i + 1]);
                }
            }
        }

        for (size_t i = 0; i < mats_sub.size(); ++i) {
            for (size_t j = 0, pj = 0; j < mats_sub[i].size(); ++j, pj += dimensions_sub[i]) {
                for (size_t k = 0, pk = 0; k < mats_sub[i][j].size(); ++k, pk += dimensions_sub[i + 1]) {
                    for (size_t g = 0; g < dimensions_sub[i]; ++g) {
                        for (size_t h = 0; h < dimensions_sub[i + 1]; ++h) {
                            mats_sub[i][j][k][g][h] = mats[i][pj + g][pk + h];
                        }
                    }
                }
            }
        }

        // Calculate p0 to p6
        array<Matrix<T>, 2> s;
        array<Matrix<T>, 7> p;

        s[0].assign(mats_sub[0][0][0]).add(mats_sub[0][1][1]);  // a00 + a11
        s[1].assign(mats_sub[1][0][0]).add(mats_sub[1][1][1]);  // b00 + b11
        p[0] = this->strassen(s);                               // p0 = (a00 + a11) * (b00 + b11)

        for (size_t i = 0; i < 2; ++i) {
            s[0].assign(mats_sub[0][1 - i][i]).add(mats_sub[0][1 - i][1 - i]);  // a10 + a11                    a01 + a00
            s[1].swap(mats_sub[1][i][i]);                                       // b00                          b11
            p[1 + i] = this->strassen(s);                                       // p1 = (a10 + a11) * b00       p2 = (a00 + a01) * b11
            s[1].swap(mats_sub[1][i][i]);
        }

        for (size_t i = 0; i < 2; ++i) {
            s[0].swap(mats_sub[0][i][i]);                                       // a00                          a11
            s[1].assign(mats_sub[1][i][1 - i]).sub(mats_sub[1][1 - i][1 - i]);  // b01 - b11                    b10 - b00
            p[3 + i] = this->strassen(s);                                       // p3 = a00 * (b01 - b11)       p4 = a11 * (b10 - b00)
            s[0].swap(mats_sub[0][i][i]);
        }

        for (size_t i = 0; i < 2; ++i) {
            s[0].assign(mats_sub[0][1 - i][i]).sub(mats_sub[0][i][i]);      // a10 - a00                            a01 - a11
            s[1].assign(mats_sub[1][i][i]).add(mats_sub[1][i][1 - i]);      // b00 + b01                            b11 + b10
            p[5 + i] = this->strassen(s);                                   // p5 = (a10 - a00) * (b00 + b01)       p6 = (a01 - a11) * (b11 + b10)
        }

        // Calculate c00, c11, c01 and c10
        array<array<Matrix<T>, 2>, 2> res_sub;

        for (size_t i = 0; i < 2; ++i) {
            res_sub[i][i].assign(p[0]).sub(p[2 - i]).add(p[4 - i]).add(p[6 - i]);   // c00 = p0 - p2 + p4 + p6      c11 = p0 - p1 + p3 + p5
            res_sub[i][1 - i].assign(p[2 - i]).add(p[3 + i]);                       // c01 = p2 + p3                c10 = p1 + p4
        }

        res_sub[0][1].resize(dimensions_sub[0], dimensions[2] >> 1);
        res_sub[1][1].resize(dimensions[0] >> 1, dimensions[2] >> 1);
        res_sub[1][0].resize(dimensions[0] >> 1, dimensions_sub[2]);

        // Group into a single matrix
        Matrix<T> res(dimensions[0], dimensions[2]);

        for (size_t i = 0, pi = 0; i < res_sub.size(); pi += res_sub[i][0].size(0), ++i) {
            for (size_t j = 0, pj = 0; j < res_sub[i].size(); pj += res_sub[i][j].size(1), ++j) {
                for (size_t k = 0; k < res_sub[i][j].size(0); ++k) {
                    for (size_t g = 0; g < res_sub[i][j].size(1); ++g) {
                        res[pi + k][pj + g] = res_sub[i][j][k][g];
                    }
                }
            }
        }

        // Resize back
        for (size_t i = 0; i < mats.size(); ++i) {
            mats[i].resize(dimensions[i], dimensions[i + 1]);
        }

        return res;
    }

    // scale dimensions to 2k
    array<size_t, 3> dimensions_normalized(const array<size_t, 3>& dimensions) const {
        array<size_t, 3> res(dimensions);

        for (auto& dimension : res) {
            dimension = (dimension + 1) & ~1;
        }

        return res;
    }


    // check base case
    bool reached_base_dimensions(const std::array<size_t, 3>& dimensions) const {
        // return accumulate(dimensions.begin(), dimensions.end(), 1, multiplies<size_t>()) <= B;
        return all_of(dimensions.begin(), dimensions.end(), [](size_t x) { return x <= B; });
    }
};
