#pragma once

#include <omp.h>

#include <algorithm>
#include <array>
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

private:
	index_type m_size;
	array_type m_array;

public:
	Matrix() : m_size{ 0, 0 }, m_array(0) {}
	Matrix(const index_type& size) : m_size(size), m_array(size.at(0), vector<T>(size.at(1), 0)) {}
	Matrix(size_t n_row, size_t n_column) : m_size{n_row, n_column}, m_array(n_row, vector<T>(n_column, 0)) {}

	const index_type& size() const { return this->m_size; }
	size_t size(size_t axis) const { return this->m_size.at(axis); }

	void resize(size_t n_row, size_t n_column) {
		this->m_array.resize(n_row, row_type(n_column, 0));

		if (n_column != this->m_size.at(1)) {
			for (size_t i = 0; i < n_row; ++i) {
				this->m_array.at(i).resize(n_column, 0);
			}
		}

		this->m_size.at(0) = n_row;
		this->m_size.at(1) = n_column;
	}

	void swap(Matrix& other) {
		std::swap(this->m_size, other.m_size);
		this->m_array.swap(other.m_array);
	}

	Matrix& assign(const Matrix& other) {
		this->resize(other.m_size.at(0), other.m_size.at(1));
		for (size_t i = 0; i < this->m_size.at(0); ++i) {
			for (size_t j = 0; j < this->m_size.at(1); ++j) {
				this->m_array.at(i).at(j) = other.m_array.at(i).at(j);
			}
		}

		return *this;
	}

	Matrix& assign_parallel(const Matrix& other) {
		this->resize(other.m_size.at(0), other.m_size.at(1));

		#pragma omp parallel for
		for (size_t i = 0; i < this->m_size.at(0); ++i) {
			for (size_t j = 0; j < this->m_size.at(1); ++j) {
				this->m_array.at(i).at(j) = other.m_array.at(i).at(j);
			}
		}

		return *this;
	}

	Matrix& add(const Matrix& other) {
		for (size_t i = 0; i < this->m_size.at(0); ++i) {
			for (size_t j = 0; j < this->m_size.at(1); ++j) {
				this->m_array.at(i).at(j) += other.m_array.at(i).at(j);
			}
		}

		return *this;
	}

	Matrix& add_parallel(const Matrix& other) {
		#pragma omp parallel for
		for (size_t i = 0; i < this->m_size.at(0); ++i) {
			for (size_t j = 0; j < this->m_size.at(1); ++j) {
				this->m_array.at(i).at(j) += other.m_array.at(i).at(j);
			}
		}

		return *this;
	}

	Matrix& sub(const Matrix& other) {
		for (size_t i = 0; i < this->m_size.at(0); ++i) {
			for (size_t j = 0; j < this->m_size.at(1); ++j) {
				this->m_array.at(i).at(j) -= other.m_array.at(i).at(j);
			}
		}

		return *this;
	}

	Matrix& sub_parallel(const Matrix& other) {
		#pragma omp parallel for
		for (size_t i = 0; i < this->m_size.at(0); ++i) {
			for (size_t j = 0; j < this->m_size.at(1); ++j) {
				this->m_array.at(i).at(j) -= other.m_array.at(i).at(j);
			}
		}

		return *this;
	}

	const row_type& operator [](size_t idx) const { return this->m_array.at(idx); }
	row_type& operator [](size_t idx) { return this->m_array.at(idx); }
};
