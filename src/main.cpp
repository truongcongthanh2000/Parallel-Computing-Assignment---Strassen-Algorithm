#include "checker.h"
#include "gen.h"
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
Matrix<T> read_matrix(istream& is = cin) {
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
	const bool to_gen = argc > 1 && stoi(argv[1]) != 0;
	const Os op_sys = argc > 2 ? argv[2] == "Win" ? Os::Win : Os::Linux : Os::Linux;

	if (to_gen) {
		gen(op_sys);
	}

	const string input_collection(op_sys == Os::Linux? "./input/input_collection.txt": "input\\input_collection.txt");
	ifstream ifs(input_collection);

	vector<string> input_file_names;
	do {
		string line;
		if (!getline(ifs, line)) {
			break;
		}
		input_file_names.push_back(line);
	} while (true);


	auto now = time(0);
	string str_now(ctime(&now));
	str_now.pop_back();
	std::replace(str_now.begin(), str_now.end(), ' ', '_');
	std::replace(str_now.begin(), str_now.end(), ':', '_');


	NaiveSerial<int> naive_serial;
	NaiveOpenMp<int> naive_openmp;
	StrassenSerialEager<int, 16> strassen_serial_eager;
	StrassenSerialLazy<int, 16> strassen_serial_lazy;

	// str_now = "";
	ofstream ofs(op_sys == Os::Linux ? "./output/output_" + str_now + ".txt" : "output\\output_" + str_now + ".txt");
	Timer<int> timer(&naive_serial, &ofs);
	Checker<int> checker(&timer, &ofs);
	Printer<int> printer(&checker, &cout);

	for (const auto& file_name : input_file_names) {
		ifstream ifs(file_name);

		ofs << file_name << endl;

		array<Matrix<int>, 2> mats{ read_matrix(ifs), read_matrix(ifs) };
		auto res = read_matrix(ifs);

		ofs << mats[0].size(0) << " x " << mats[0].size(1) << " x " << mats[1].size(1) << " -> " << res.size(0) << " x " << res.size(1) << "\n" << endl;

		checker.set_answer(&res);

		ofs << "Naive Serial: ";
		timer.set_decoratee(&naive_serial);
		checker(mats);

		ofs << endl;

		ofs << "Naive OpenMP: ";
		timer.set_decoratee(&naive_openmp);
		checker(mats);

		ofs << endl;

		ofs << "Strassen Serial w Eager padding: ";
		timer.set_decoratee(&strassen_serial_eager);
		checker(mats);

		ofs << endl;

		ofs << "Strassen Serial w Lazy padding: ";
		timer.set_decoratee(&strassen_serial_lazy);
		checker(mats);

		ofs << "\n" << endl;
	}
}
