#pragma once

#include "decorator.h"

#include <omp.h>

#include <iomanip>
#include <iostream>

using namespace std;


template <class T = int>
class Timer : public OutputDecorator<T> {
public:
	Timer(const Decorator<T>* decoratee, ostream* os = &cout) : OutputDecorator<T>(decoratee, os) {}

	Matrix<T> operator ()(array<Matrix<T>, 2>& mats) const override {
		auto start = omp_get_wtime();

		auto res = (*this->m_decoratee)(mats);

		auto end = omp_get_wtime();

		(*this->m_ostream) << std::fixed << std::setprecision(3) << (end - start) << " ";

		return res;
	}
};
