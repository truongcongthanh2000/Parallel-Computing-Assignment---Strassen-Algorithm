#pragma once

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <type_traits>
#include <vector>

using namespace std;


template <class T = int, typename = typename enable_if<is_arithmetic<T>::value, T>::type>
class Matrix {
public:
	typedef array<size_t, 2> index_type;
	typedef vector<T> row_type;
	typedef vector<row_type> array_type;

	Matrix() : m_size{ 0, 0 }, m_array(0) {}
	Matrix(const index_type& size) : m_size(size), m_array(size[0], vector<T>(size[1], 0)) {}
	Matrix(size_t n_row, size_t n_column) : m_size{n_row, n_column}, m_array(n_row, vector<T>(n_column, 0)) {}

	const index_type& size() const { return this->m_size; }
	size_t size(size_t axis) const { return this->m_size[axis]; }

	void resize(size_t n_row, size_t n_column) {
		this->m_array.resize(n_row, row_type(0));

		if (n_column != this->m_size[1]) {
			for (size_t i = 0; i < n_row; ++i) {
				this->m_array[i].resize(n_column, 0);
			}
		}

		this->m_size[0] = n_row;
		this->m_size[1] = n_column;
	}

	void swap(Matrix& other) {
		std::swap(this->m_size, other.m_size);
		this->m_array.swap(other.m_array);
	}

	const row_type& operator [](size_t idx) const { return this->m_array[idx]; }
	row_type& operator [](size_t idx) { return this->m_array[idx]; }

	Matrix operator +(const Matrix& other) const {
		Matrix res(this->size());

		for (size_t i = 0; i < this->m_size[0]; ++i) {
			for (size_t j = 0; j < this->m_size[1]; ++j) {
				res.m_array[i][j] = this->m_array[i][j] + other.m_array[i][j];
			}
		}

		return res;
	}

	Matrix operator -(const Matrix& other) const {
		Matrix res(this->size());

		for (size_t i = 0; i < this->m_size[0]; ++i) {
			for (size_t j = 0; j < this->m_size[1]; ++j) {
				res.m_array[i][j] = this->m_array[i][j] - other.m_array[i][j];
			}
		}

		return res;
	}

	Matrix& operator +=(const Matrix& other) {
		for (size_t i = 0; i < this->m_size[0]; ++i) {
			for (size_t j = 0; j < this->m_size[0]; ++j) {
				this->m_array[i][j] += other.m_array[i][j];
			}
		}

		return *this;
	}

	Matrix& operator -=(const Matrix& other) {
		for (size_t i = 0; i < this->m_size[0]; ++i) {
			for (size_t j = 0; j < this->m_size[0]; ++j) {
				this->m_array[i][j] -= other.m_array[i][j];
			}
		}

		return *this;
	}

private:
	index_type m_size;
	array_type m_array;
};
