/*
#include "matrix.h"
#include "naive_openmp.h"
#include "printer.h"

#include <array>
#include <fstream>
#include <random>
#include <string>
#include <vector>


int main(int argc, char** argv) {
	const std::string dimension_collection("input\\dimension_collection.txt");
	std::ifstream ifs(dimension_collection);

	std::vector<std::array<const size_t, 3>> dimensions;
	do {
		size_t n, m, p;
		if (!(ifs >> n >> m >> p)) {
			break;
		}
		dimensions.push_back({ n, m, p });
	} while (true);

	NaiveOpenMp<int> naive_openmp;
	Printer<int> printer(naive_openmp);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(1, 100);
	for (size_t i = 0; i < dimensions.size(); ++i) {
		std::ofstream ofs("input\\input_" + std::to_string(i) + ".txt");
		printer.set_ostream(ofs);

		const auto& n = dimensions[i][0];
		const auto& m = dimensions[i][1];
		const auto& p = dimensions[i][2];

		Matrix<int> a(n, m);
		for (size_t j = 0; j < a.size(0); ++j) {
			for (size_t k = 0; k < a.size(1); ++k) {
				a[j][k] = dist(gen);
			}
		}

		Matrix<int> b(m, p);
		for (size_t j = 0; j < b.size(0); ++j) {
			for (size_t k = 0; k < b.size(1); ++k) {
				b[j][k] = dist(gen);
			}
		}

		printer(a, b);
	}


	const std::string input_collection("input\\input_collection.txt");
	std::ofstream ofs(dimension_collection);

	for (size_t i = 0; i < dimensions.size(); ++i) {
		ofs << "input\\input_" << std::to_string(i) << ".txt\n";
	}
}
*/