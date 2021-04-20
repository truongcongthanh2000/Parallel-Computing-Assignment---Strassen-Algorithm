#pragma once

#include "matrix.h"

#include <array>
#include <iostream>

using namespace std;


template <class T = int>
class Decorator {
public:
	virtual Matrix<T> operator ()(array<Matrix<T>, 2>& mats) const = 0;
};


template <class T = int>
class ProperDecorator : public Decorator<T> {
public:
	ProperDecorator(const Decorator<T>* decoratee) : m_decoratee(decoratee) {}

	void set_decoratee(const Decorator<T>* decoratee) { this->m_decoratee = decoratee; }

protected:
	const Decorator<T>* m_decoratee;
};


template <class T = int>
class OutputDecorator : public ProperDecorator<T> {
public:
	OutputDecorator(const Decorator<T>* decoratee, ostream* os = &cout) : ProperDecorator<T>(decoratee), m_ostream(os) {}

	void set_ostream(ostream* os) { this->m_ostream = os; }

protected:
	ostream* m_ostream;
};
