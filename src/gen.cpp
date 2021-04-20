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
	ofstream ofs(dimension_collection);

	for (size_t i = 0; i < dimensions.size(); ++i) {
		ofs << "input\\input_" << std::to_string(i) << ".txt\n";
	}
}
*/