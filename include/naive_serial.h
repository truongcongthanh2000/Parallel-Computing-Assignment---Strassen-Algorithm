#pragma once

#include "decorator.h"

#include <algorithm>

using namespace std;


template <class T = int>
class NaiveSerial : public Decorator<T> {
public:
	Matrix<T> operator ()(array<Matrix<T>, 2>& mats) const override {
		// initialize c
		Matrix<T> res(mats[0].size(0), mats[1].size(1));

		// transpose b for cache obliviousness
		auto temp = this->transpose(mats[1]);
		mats[1].swap(temp);

		// multiply
		for (size_t i = 0; i < mats[0].size(0); ++i) {
			for (size_t j = 0; j < mats[1].size(0); ++j) {
				for (size_t k = 0; k < mats[1].size(1); ++k) {
					res[i][j] += mats[0][i][k] * mats[1][j][k];
				}
			}
		}

		mats[1].swap(temp);

		return res;
	}

private:
	Matrix<T> transpose(const Matrix<T>& mat) const {
		Matrix<T> mat_T(mat.size(1), mat.size(0));

		if (all_of(mat.size().begin(), mat.size().end(), [](size_t x) { return x != 0; })) {
			this->transpose_helper(0, mat.size(0), 0, mat.size(1), mat, mat_T);
		}

		return mat_T;
	}

	void transpose_helper(size_t x, size_t dx, size_t y, size_t dy, const Matrix<T>& mat, Matrix<T>& mat_T) const {
		if (dx <= 16 && dy <= 16) {
			for (size_t i = 0; i < dx; ++i) {
				for (size_t j = 0; j < dy; ++j) {
					mat_T[y + j][x + i] = mat[x + i][y + j];
				}
			}
		}
		else if (dx > dy) {
			int ddx = dx / 2;
			this->transpose_helper(x, ddx, y, dy, mat, mat_T);
			this->transpose_helper(x + ddx, dx - ddx, y, dy, mat, mat_T);
		}
		else {
			int ddy = dy / 2;
			this->transpose_helper(x, dx, y, ddy, mat, mat_T);
			this->transpose_helper(x, dx, y + ddy, dy - ddy, mat, mat_T);
		}
	}
};