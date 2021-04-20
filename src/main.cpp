#include "checker.h"
#include "matrix.h"
#include "naive_openmp.h"
#include "naive_serial.h"
#include "printer.h"
#include "timer.h"
#include "strassen_serial_eager.h"
#include "strassen_serial_lazy.h"

#include <algorithm>
#include <array>
#include <ctime>
#include <fstream>
#include <random>
#include <string>
#include <vector>

using namespace std;


template <class T = int>
Matrix<T> read_matrix(std::istream& is = std::cin) {
	size_t n, m;
	is >> n >> m;

	Matrix<T> res(n, m);
	for (size_t i = 0; i < res.size(0); ++i) {
		for (size_t j = 0; j < res.size(1); ++j) {
			is >> res[i][j];
		}
	}

	return res;
}


int main(int argc, char** argv) {
	const std::string input_collection("input\\input_collection.txt");
	std::ifstream ifs(input_collection);

	std::vector<std::string> input_file_names;
	do {
		std::string line;
		if (!std::getline(ifs, line)) {
			break;
		}
		input_file_names.push_back(line);
	} while (true);


	auto now = std::time(0);
	std::string str_now(std::ctime(&now));
	str_now.pop_back();
	std::replace(str_now.begin(), str_now.end(), ' ', '_');
	std::replace(str_now.begin(), str_now.end(), ':', '_');


	NaiveSerial<int> naive_serial;
	NaiveOpenMp<int> naive_openmp;
	StrassenSerialEager<int, 16> strassen_serial_eager;
	StrassenSerialLazy<int, 16> strassen_serial_lazy;

	str_now = "";
	std::ofstream ofs("output\\output_" + str_now + ".txt");
	Timer<int> timer(&naive_serial, &ofs);
	Checker<int> checker(&timer, &ofs);
	Printer<int> printer(&checker, &cout);

	for (const auto& file_name : input_file_names) {
		cout << file_name;
		ifstream ifs(file_name);
		array<Matrix<int>, 2> mats{ read_matrix(ifs), read_matrix(ifs) };
		auto res = read_matrix(ifs);

		checker.set_answer(&res);

		ofs << "Naive Serial: ";
		timer.set_decoratee(&naive_serial);
		checker(mats);

		ofs << "\n";

		ofs << "Naive OpenMP: ";
		timer.set_decoratee(&naive_openmp);
		checker(mats);

		ofs << "\n";

		ofs << "Strassen Serial w Eager padding: ";
		timer.set_decoratee(&strassen_serial_eager);
		checker(mats);

		ofs << "\n";

		ofs << "Strassen Serial w Lazy padding: ";
		timer.set_decoratee(&strassen_serial_lazy);
		checker(mats);

		ofs << "\n";
	}
}

// generation code

/*
#include "matrix.h"
#include "naive_openmp.h"
#include "printer.h"

#include <array>
#include <fstream>
#include <random>
#include <string>
#include <vector>

using namespace std;


int main(int argc, char** argv) {
	const string dimension_collection("input\\dimension_collection.txt");
	ifstream ifs(dimension_collection);

	vector<array<const size_t, 3>> dimensions;
	do {
		size_t n, m, p;
		if (!(ifs >> n >> m >> p)) {
			break;
		}
		dimensions.push_back({ n, m, p });
	} while (true);

	NaiveOpenMp<int> naive_openmp;
	Printer<int> printer(&naive_openmp);

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dist(1, 100);
	for (size_t i = 0; i < dimensions.size(); ++i) {
		ofstream ofs("input\\input_" + to_string(i) + ".txt");
		printer.set_ostream(&ofs);

		array<Matrix<int>, 2> mats;
		for (size_t j = 0; j < 2; ++j) {
			mats[j] = Matrix<int>(dimensions[i][j], dimensions[i][j + 1]);
			for (size_t k = 0; k < mats[j].size(0); ++k) {
				for (size_t g = 0; g < mats[j].size(1); ++g) {
					mats[j][k][g] = dist(gen);
				}
			}
		}

		printer(mats);
	}


	const string input_collection("input\\input_collection.txt");
	ofstream ofs(input_collection);

	for (size_t i = 0; i < dimensions.size(); ++i) {
		ofs << "input\\input_" << std::to_string(i) << ".txt\n";
	}
}
*/