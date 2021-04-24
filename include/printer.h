#pragma once

#include "decorator.h"

#include <iostream>

using namespace std;


template <class T = int>
class Printer : public OutputDecorator<T> {
public:
	Printer(const Decorator<T>* decoratee, ostream* os = &cout) : OutputDecorator<T>(decoratee, os) {}

	Matrix<T> operator ()(array<Matrix<T>, 2>& mats) const override {
		auto res = (*this->m_decoratee)(mats);

		for (const auto& mat : { mats[0], mats[1], res }) {
			(*this->m_ostream) << mat.size(0) << " " << mat.size(1) << "\n";
			for (size_t i = 0; i < mat.size(0); ++i) {
				for (size_t j = 0; j < mat.size(1); ++j) {
					(*this->m_ostream) << mat[i][j] << (j == mat.size(1) - 1 ? "\n" : " ");
				}
			}
			(*this->m_ostream) << "\n";
		}

		return res;
	}
};
