#pragma once

#include "decorator.h"

#include <algorithm>
#include <iostream>

using namespace std;


template <class T = int>
class Checker : public OutputDecorator<T> {
public:
	Checker(const Decorator<T>* decoratee, ostream* os = &cout, const Matrix<T>* answer = nullptr) : OutputDecorator<T>(decoratee, os), m_answer(answer) {}

	Matrix<T> operator ()(array<Matrix<T>, 2>& mats) const override {
		auto res = (*this->m_decoratee)(mats);

		if (this->m_answer == nullptr) {
			return res;
		}

		double norm = 0.0;
		if (res.size() != this->m_answer->size()) {
			norm = -1.0;
		}
		else {
			for (size_t i = 0; i < res.size(0); ++i) {
				for (size_t j = 0; j < res.size(1); ++j) {
					double temp = res[i][j] - (*this->m_answer)[i][j];
					norm += temp * temp;
				}
			}
		}

		if (norm < -1e-10) {
			(*this->m_ostream) << "EXTREMELY NOT OK ";
		}
		else if (norm > 1e-10) {
			(*this->m_ostream) << "NOT OK ";
		}
		else {
			(*this->m_ostream) << "OK ";
		}

		return res;
	}

	void set_answer(const Matrix<T>* answer) { this->m_answer = answer; }

private:
	const Matrix<T>* m_answer;
};
