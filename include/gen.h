#pragma once

#include "matrix.h"
#include "naive_openmp.h"

#include "printer.h"

#include "platform.h"

#include <array>
#include <fstream>
#include <random>
#include <string>
#include <vector>

using namespace std;


void gen(string os = "linux") {
	string dimension_collection_file_name;
	if (os == "linux") {
		dimension_collection_file_name = "./input/dimension_collection.txt";
	}
	else if (os == "windows") {
		dimension_collection_file_name = "input\\dimension_collection.txt";
	}


	ifstream ifs(dimension_collection_file_name);
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
		auto& file_name = file_names.at(i);
		if (os == "linux") {
			file_name = "./input/input_" + to_string(i) + ".txt";
		}
		else if (os == "windows") {
			file_name = "input\\input_" + to_string(i) + ".txt";
		}

		ofstream ofs(file_name);
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


	string input_collection_file_name;
	if (os == "linux") {
		input_collection_file_name = "./input/input_collection.txt";
	}
	else if (os == "windows") {
		input_collection_file_name = "input\\input_collection.txt";
	}

	ofstream ofs(input_collection_file_name);

	for (const auto& file_name : file_names) {
		ofs << file_name << "\n";
	}
}
