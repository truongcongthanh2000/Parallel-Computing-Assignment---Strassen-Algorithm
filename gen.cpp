#include <bits/stdc++.h>
#include "testlib.h"

using namespace std;

//command: ./gen n limits_value
int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Wrong command, Please enter n and limits_value" << endl;
        return 0;
    }
    fstream fout;
    fout.open("input.txt", ios::out);

    int n = stoi(argv[1]);
    int limits_value = stoi(argv[2]);

    fout << n << '\n';
    for (int i = 0; i < n; i++) {
        fout << rnd.next(1, limits_value);
        for (int j = 1; j < n; j++) fout << " " << rnd.next(1, limits_value);
        if (i != n - 1) fout << '\n';
    }
    fout.close();
    return 0;
}
