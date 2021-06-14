#include <bits/stdc++.h>
#include "testlib.h"

using namespace std;

//command: ./gen n m p limits_value
int main(int argc, char* argv[]) {
    if (argc != 5) {
        cout << "Wrong command, Please enter n, m, p and limits_value" << endl;
        return 0;
    }
    fstream fout;
    fout.open("input.txt", ios::out);

    int n = stoi(argv[1]);
    int m = stoi(argv[2]);
    int p = stoi(argv[3]);

    if (min(min(n, m), p) < 1) {
        cout << "Wrong command, The size must be a positive integer" << endl;
        return 0;
    }
    int limits_value = stoi(argv[4]);

    if (limits_value <= 0) {
        cout << "Wrong command, limits_value must be a positive integer" << endl;
        return 0;
    }
    
    cout << "Gen matrix A with size " << n << " x " << m << endl;
    fout << n << " " << m << '\n';
    for (int i = 0; i < n; i++) {
        fout << rnd.next(1, limits_value);
        for (int j = 1; j < m; j++) fout << " " << rnd.next(1, limits_value);
        fout << '\n';
    }
    cout << "Gen matrix B with size " << m << " x " << p << endl;
    fout << m << " " << p << '\n';
    for (int i = 0; i < m; i++) {
        fout << rnd.next(1, limits_value);
        for (int j = 1; j < p; j++) fout << " " << rnd.next(1, limits_value);
        if (i != m - 1) fout << '\n';
    }
    fout.close();
    return 0;
}
