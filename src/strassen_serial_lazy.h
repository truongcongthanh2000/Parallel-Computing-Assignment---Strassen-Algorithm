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
        return this->strassen(mats);
    }

private:
    Matrix<T> strassen(array<Matrix<T>, 2>& mats) const {
        const array<size_t, 3> dimensions = { mats[0].size(0), mats[0].size(1), mats[1].size(1) };
        if (this->reached_base_dimensions(dimensions)) {
            return NaiveSerial<T>()(mats);
        }

        auto dimensions_new = this->dimensions_normalized(dimensions);

        array<size_t, 3> dimensions_sub{ 0 };
        transform(dimensions_new.begin(), dimensions_new.end(), dimensions_sub.begin(), [](size_t x) { return x >> 1; });

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

        // Calculating p1 to p7:
        array<Matrix<T>, 2> s;
        array<Matrix<T>, 7> p;

        s[0] = mats_sub[0][0][0] + mats_sub[0][1][1];   // a11 + a22
        s[1] = mats_sub[1][0][0] + mats_sub[1][1][1];   // b11 + b22
        p[0] = this->strassen(s);                       // p1 = (a11 + a22) * (b11 + b22)

        for (size_t i = 0; i < 2; ++i) {
            s[0] = mats_sub[0][1 - i][i] + mats_sub[0][1 - i][1 - i];   // a21 + a22                    a12 + a11
            s[1].swap(mats_sub[1][i][i]);                               // b11                          b22
            p[1 + i] = this->strassen(s);                               // p2 = (a21 + a22) * b11       p3 = (a11 + a12) * b22
            s[1].swap(mats_sub[1][i][i]);
        }

        for (size_t i = 0; i < 2; ++i) {
            s[0].swap(mats_sub[0][i][i]);                               // a11                          a22
            s[1] = mats_sub[1][i][1 - i] - mats_sub[1][1 - i][1 - i];   // b12 - b22                    b21 - b11
            p[3 + i] = this->strassen(s);                               // p4 = a11 * (b12 - b22)       p5 = a22 * (b21 - b11)
            s[0].swap(mats_sub[0][i][i]);
        }

        for (size_t i = 0; i < 2; ++i) {
            s[0] = mats_sub[0][1 - i][i] - mats_sub[0][i][i];   // a21 - a11                            a12 - a22
            s[1] = mats_sub[1][i][i] + mats_sub[1][i][1 - i];   // b11 + b12                            b22 + b21
            p[5 + i] = this->strassen(s);                       // p6 = (a21 - a11) * (b11 + b12)       p7 = (a12 - a22) * (b22 + b21)
        }

        // calculating c11, c12, c21 and c22:
        array<array<Matrix<T>, 2>, 2> res_sub;

        for (size_t i = 0; i < 2; ++i) {
            res_sub[i][i] = p[0] - p[2 - i] + p[4 - i] + p[6 - i];  // c11 = p1 - p3 + p5 + p7      c22 = p1 - p2 + p4 + p6
            res_sub[i][1 - i] = p[2 - i] + p[3 + i];                // c12 = p3 + p4                c21 = p2 + p5
        }

        res_sub[0][1].resize(dimensions_new[0] >> 1, dimensions_sub[1]);
        res_sub[1][1].resize(dimensions_sub[0], dimensions_sub[1]);
        res_sub[1][0].resize(dimensions_sub[0], dimensions_new[1] >> 1);

        // Grouping the results obtained in a single matrix
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
        
        return res;
    }

    array<size_t, 3> dimensions_normalized(const array<size_t, 3>& dimensions) const {
        array<size_t, 3> res(dimensions);

        for (auto& dimension : res) {
            dimension = (dimension + 1) & ~1;
        }

        return res;
    }


    bool reached_base_dimensions(const std::array<size_t, 3>& dimensions) const {
        // return accumulate(dimensions.begin(), dimensions.end(), 1, multiplies<size_t>()) <= B;
        return all_of(dimensions.begin(), dimensions.end(), [](size_t x) { return x <= B; });
    }
};
