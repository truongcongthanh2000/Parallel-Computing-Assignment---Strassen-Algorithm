#include <bits/stdc++.h>

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;

void load(vvi &a, vvi &b) {
    fstream finp;
    finp.open("input.txt", ios::in);
    int n, m, p;
    finp >> n >> m;

    a.resize(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int x;
            finp >> x;
            a[i].push_back(x);
            assert(x >= 1);
        }
    }

    finp >> n >> p;
    b.resize(m);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            int x;
            finp >> x;
            b[i].push_back(x);
            assert(x >= 1);
        }
    }

    finp.close();
}

void print(vvi &a) {
    fstream fout;
    fout.open("output2.txt", ios::out);
    fout << "size matrix " << (int)a.size() << " x " << (int)a[0].size() << '\n';
    for (vi v : a) {
        fout << v[0];
        for (int i = 1; i < (int)v.size(); i++) fout << " " << v[i];
        fout << '\n';        
    }

    fout.close();
}

vvi multiply(vvi &a, vvi &b) {
    int n = (int)a.size();
    int m = (int)a[0].size();
    assert((int)b.size() == m);
    int p = (int)b[0].size();
    vvi c(n, vi(p, 0));
    for (int k = 0; k < m; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < p; j++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return c;
}

vvi matrix_add(vvi &a, vvi &b) {
    int n = (int)a.size();
    int m = (int)a[0].size();
    assert((int)b.size() == n);
    assert((int)b[0].size() == m);
    vvi c(n, vi(m, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
    return c;
}   
vvi matrix_sub(vvi &a, vvi &b) {
    // cout << "a = " << endl;
    // for (auto v : a) {
    //     for (int x : v) cout << x << ' ';
    //     cout << '\n';
    // }
    // cout << "b = " << endl;
    // for (auto v : b) {
    //     for (int x : v) cout << x << ' ';
    //     cout << '\n';
    // }
    int n = (int)a.size();
    int m = (int)a[0].size();
    assert((int)b.size() == n);
    assert((int)b[0].size() == m);
    vvi c(n, vi(m, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            c[i][j] = a[i][j] - b[i][j];
        }
    }
    return c;   
}   

//If the dimension matrix is ​​not in the standard n x m form where n, m are even, re-normalize
void norm(vvi &a) {
    int n = (int)a.size();
    int m = (int)a[0].size();
    
    if (m & 1) {
        for (int i = 0; i < n; i++) a[i].push_back(0);
        m++;
    }
    if (n & 1) {
        a.push_back(vi(m, 0));
        n++;
    }
    
    assert(n % 2 == 0);
    assert(m % 2 == 0);
    assert((int)a.size() == n);
    for (int i = 0; i < n; i++) assert((int)a[i].size() == m);
}

void de_norm(int prev_n, int prev_m, vvi &a) {
    int n = (int)a.size();
    int m = (int)a[0].size();
    // cout << "Before: " << endl;
    // for (auto v : a) {
    //     for (int x : v) cout << x << ' ';
    //     cout << '\n';
    // }
    for (int i = 0; i < n; i++) {
        while ((int)a[i].size() > prev_m) a[i].pop_back();
    }
    while ((int)a.size() > prev_n) a.pop_back();
    assert((int)a.size() == prev_n);
    assert(prev_n > 0);
    assert(prev_m > 0);
    // cout << "After: " << endl;
    // for (auto v : a) {
    //     for (int x : v) cout << x << ' ';
    //     cout << '\n';
    // }
    for (int i = 0; i < prev_n; i++) assert((int)a[i].size() == prev_m);
}
vvi Strassen(vvi a, vvi b) {
    int prev_n = (int)a.size();
    int prev_m = (int)a[0].size();
    assert((int)b.size() == prev_m);
    int prev_p = (int)b[0].size();
    norm(a);
    norm(b);
    int n = (int)a.size();
    int m = (int)a[0].size();
    int p = (int)b[0].size();
    assert((int)b.size() == m);
    assert(n % 2 == 0);
    assert(m % 2 == 0);
    assert(p % 2 == 0);
    if (min(n, min(m, p)) == 2) {
        vvi c = multiply(a, b);
        de_norm(prev_n, prev_p, c);
        return c;
    }

    int next_n = n / 2;
    int next_m = m / 2;
    int next_p = p / 2;
    vvi a11, a12, a21, a22, b11, b12, b21, b22;
    a11 = a12 = a21 = a22 = vvi(next_n, vi(next_m, 0));
    b11 = b12 = b21 = b22 = vvi(next_m, vi(next_p, 0));

    for (int i = 0; i < next_n; i++) {
        for (int j = 0; j < next_m; j++) {
            a11[i][j] = a[i][j];  // top left
            a12[i][j] = a[i][j + next_m];  // top right
            a21[i][j] = a[i + next_n][j];  // bottom left
            a22[i][j] = a[i + next_n][j + next_m];  // bottom right
        }
    }
    for (int i = 0; i < next_m; i++) {
        for (int j = 0; j < next_p; j++) {
            b11[i][j] = b[i][j];  // top left
            b12[i][j] = b[i][j + next_p];  // top right
            b21[i][j] = b[i + next_m][j];  // bottom left
            b22[i][j] = b[i + next_m][j + next_p];  // bottom right
        }
    }

    vvi aResult = matrix_add(a11, a22); // a11 + a22 nm
    vvi bResult = matrix_add(b11, b22); // b11 + b22 mp

    // Calculating p1 to p7:
    vvi p1 = Strassen(aResult, bResult); // p1 = (a11 + a22) * (b11 + b22) np

    aResult = matrix_add(a21, a22); // a21 + a22 nm
    vvi p2 = Strassen(aResult, b11); // p2 = (a21 + a22) * b11 np

    bResult = matrix_sub(b12, b22); // b12 - b22 mp
    vvi p3 = Strassen(a11, bResult); //p3 = a11 * (b12 - b22) np

    bResult = matrix_sub(b21, b11); // b21 - b11 mp
    vvi p4 = Strassen(a22, bResult); // p4 = a22 * (b21 - b11) np

    aResult = matrix_add(a11, a12); // a11 + a12 nm
    vvi p5 = Strassen(aResult, b22); // p5 = (a11 + a12) * b22 np

    aResult = matrix_sub(a21, a11); // a21 - a11 nm
    bResult = matrix_add(b11, b12); // b11 + b12 mp
    vvi p6 = Strassen(aResult, bResult); // p6 = (a22 - a11) * (b11 + b12) np

    aResult = matrix_sub(a12, a22); // a12 - a22 nm
    bResult = matrix_add(b21, b22); // b21 + b22 mp
    vvi p7 = Strassen(aResult, bResult); //p7 = (a12 - a22) * (b21 + b22) np

    // calculating c21, c21, c11 and c22:
    vvi c12 = matrix_add(p3, p5); // c12 = p3 + p5 np + np
    vvi c21 = matrix_add(p2, p4); // c21 = p2 + p4 np + np

    aResult = matrix_add(p1, p4); // p1 + p4 np + np
    bResult = matrix_add(aResult, p7); // p1 + p4 + p7 np + np + np
    vvi c11 = matrix_sub(bResult, p5); // c11 = p1 + p4 - p5 + p7 np + np - np + np

    aResult = matrix_add(p1, p3); // p1 + p3
    bResult = matrix_add(aResult, p6); // p1 + p3 + p6
    vvi c22 = matrix_sub(bResult, p2); // c22 = p1 - p2 + p3 + p6

    // Grouping the results obtained in a single matrix
    vvi c(n, vi(p, 0));
    for (int i = 0; i < next_n; i++) {
        for (int j = 0; j < next_p; j++) {
            c[i][j] = c11[i][j];
            c[i][j + next_p] = c12[i][j];
            c[i + next_n][j] = c21[i][j];
            c[i + next_n][j + next_p] = c22[i][j];
        }
    }
    de_norm(prev_n, prev_p, c);
    return c;
}

int main(int argc, char* argv[]) {
    vvi a, b;
    load(a, b); //load matrix a and b

    int n = (int)a.size();
    int m = (int)a[0].size();
    int p = (int)b[0].size();
    cout << "n, m, p = " << n << " " << m << " " << p << endl;

    vvi c1 = multiply(a, b); ///O(N^3)

    vvi c2 = Strassen(a, b); ///O(N^log2(7))

    assert(c1 == c2);

    //print result matrix a x b
    print(c2);

    return 0;
}