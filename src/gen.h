#pragma once

#include "matrix.h"
#include "naive_openmp.h"
#include "printer.h"

#include <array>
#include <fstream>
#include <random>
#include <string>
#include <vector>

using namespace std;

enum class Os {
	Win,
	Linux
};


void gen(Os op_sys = Os::Linux) {
	const string dimension_collection(op_sys == Os::Linux ? "./input/dimension_collection.txt" : "input\\dimension_collection.txt");
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
	vector<string> file_names(dimensions.size());
	for (size_t i = 0; i < dimensions.size(); ++i) {
		file_names[i] = (op_sys == Os::Linux ? "./input/input_" + to_string(i) + ".txt" : "input\\input_" + to_string(i) + ".txt");
		ofstream ofs(file_names[i]);

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


	const string input_collection(op_sys == Os::Linux ? "./input/input_collection.txt" : "input\\input_collection.txt");
	ofstream ofs(input_collection);

	for (size_t i = 0; i < dimensions.size(); ++i) {
		ofs << file_names[i] << "\n";
	}
}
