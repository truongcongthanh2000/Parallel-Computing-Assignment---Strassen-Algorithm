#include "gen.h"

#include "checker.h"
#include "printer.h"
#include "timer.h"

#include "matrix.h"
#include "naive_openmp.h"
#include "naive_serial.h"
#include "strassen_serial_eager.h"
#include "strassen_serial_lazy.h"
#include "strassen_openmp_eager.h"
#include "strassen_openmp_lazy.h"

#include "platform.h"

#include <algorithm>
#include <array>
#include <ctime>
#include <fstream>
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
	vector<string> cmd_args(argv, argv + argc);
	const string os = get_platform_name();

	
	bool to_gen = false;
	bool to_run = true;
	bool to_specify_input = false;
	size_t n_run = 1;

	const auto is_prefix = [](const string& s1, const string& s2) -> bool {
		if (s1.size() > s2.size()) {
			return false;
		}

		for (size_t i = 0; i < s1.size(); ++i) {
			if (s1.at(i) != s2.at(i)) {
				return false;
			}
		}
		
		return true;
	};

	const string option_gen = "-g";
	const string option_gen_run = option_gen + "r";
	const string option_specify_input = "-i";
	const string option_multirun = "-n";

	for (size_t i = 0; i < cmd_args.size(); ++i) {
		const auto& cmd_arg = cmd_args.at(i);

		if (is_prefix(option_gen, cmd_arg)) {
			to_gen = true;
			to_run = is_prefix(option_gen_run, cmd_arg);
		}
		else if (is_prefix(option_specify_input, cmd_arg)) {
			to_specify_input = true;
		}
		else if (is_prefix(option_multirun, cmd_arg)) {
			n_run = stoi(cmd_args.at(i + 1));
			++i;
		}
	}


	if (to_gen) {
		gen(os);
	}


	if (to_run) {
		string input_collection_file_name;
		if (os == "linux") {
			input_collection_file_name = "./input/input_collection.txt";
		}
		else if (os == "windows") {
			input_collection_file_name = "input\\input_collection.txt";
		}


		vector<string> input_file_names(0);
		if (to_specify_input) {
			cout << "How many files will be specified? ";
			size_t n;
			cin >> n;

			string tmp;
			getline(cin, tmp);

			input_file_names.resize(n);
			input_file_names.shrink_to_fit();
			for (auto& file_name : input_file_names) {
				getline(cin, file_name);
			}
		}
		else {
			ifstream ifs(input_collection_file_name);
			do {
				string line;
				if (!getline(ifs, line)) {
					break;
				}
				input_file_names.push_back(line);
			} while (true);
		}


		auto now = time(0);
		string str_now(ctime(&now));
		str_now.pop_back();
		std::replace(str_now.begin(), str_now.end(), ' ', '_');
		std::replace(str_now.begin(), str_now.end(), ':', '_');

		string output_file_name;
		if (os == "linux") {
			output_file_name = "./output/output_" + str_now + ".txt";
		}
		else if (os == "windows") {
			output_file_name = "output\\output_" + str_now + ".txt";
		}

		ofstream ofs(output_file_name);

		NaiveSerial<int> naive_serial;
		NaiveOpenMp<int> naive_openmp;
		StrassenSerialEager<int, 16> strassen_serial_eager;
		StrassenSerialLazy<int, 16> strassen_serial_lazy;
		StrassenOpenMpEager<int, 16> strassen_openmp_eager;
		StrassenOpenMpLazy<int, 16> strassen_openmp_lazy;

		Timer<int> timer(&naive_serial, &ofs);
		Checker<int> checker(&timer, &ofs);
		Printer<int> printer(&checker, &cout);

		for (const auto& file_name : input_file_names) {
			ifstream ifs(file_name);

			ofs << file_name << endl;

			array<Matrix<int>, 2> mats{ read_matrix(ifs), read_matrix(ifs) };
			auto res = read_matrix(ifs);

			ifs.close();

			ofs << mats.at(0).size(0) << " x " << mats.at(0).size(1) << " x " << mats.at(1).size(1);
			ofs << " -> " << res.size(0) << " x " << res.size(1) << "\n" << endl;

			checker.set_answer(&res);

			for (size_t i = 0; i < n_run; ++i) {
				ofs << "Run " << i << ":" << endl;

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

				ofs << endl;

				ofs << "Strassen OpenMP w Eager padding: ";
				timer.set_decoratee(&strassen_openmp_eager);
				checker(mats);

				ofs << endl;

				ofs << "Strassen OpenMP w Lazy padding: ";
				timer.set_decoratee(&strassen_openmp_lazy);
				checker(mats);

				ofs << "\n" << endl;
			}
		}
	}
}
