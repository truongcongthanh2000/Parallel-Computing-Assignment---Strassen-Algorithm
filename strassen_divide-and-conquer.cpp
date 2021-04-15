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
    fout.open("output.txt", ios::out);
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
    vvi c(n, vi(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
    return c;
}   
vvi matrix_sub(vvi &a, vvi &b) {
    int n = (int)a.size();
    vvi c(n, vi(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c[i][j] = a[i][j] - b[i][j];
        }
    }
    return c;   
}   

//If the dimension matrix is ​​not in the standard 2^k form, re-normalize
void norm(int normSize, vvi &a) {
    int n = (int)a.size();
    int m = (int)a[0].size();

    for (int i = 0; i < n; i++) {
        for (int j = m; j < normSize; j++) a[i].push_back(0);
    }
    for (int i = n; i < normSize; i++) a.push_back(vi(normSize, 0));

    assert((int)a.size() == (int)a[0].size());
    int sz = (int)a.size();
    assert((sz & (sz - 1)) == 0);
    
}

vvi Strassen(vvi &a, vvi &b) {
    int n = (int)a.size();
    if (n == 2) return multiply(a, b);

    int newSize = n / 2;
    vvi a11, a12, a21, a22, b11, b12, b21, b22;
    a11 = a12 = a21 = a22 = b11 = b12 = b21 = b22 = vvi(newSize, vi(newSize, 0));

    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            a11[i][j] = a[i][j];  // top left
            a12[i][j] = a[i][j + newSize];  // top right
            a21[i][j] = a[i + newSize][j];  // bottom left
            a22[i][j] = a[i + newSize][j + newSize];  // bottom right

            b11[i][j] = b[i][j];  // top left
            b12[i][j] = b[i][j + newSize];  // top right
            b21[i][j] = b[i + newSize][j];  // bottom left
            b22[i][j] = b[i + newSize][j + newSize];  // bottom right
        }
    }

    vvi aResult = matrix_add(a11, a22); // a11 + a22
    vvi bResult = matrix_add(b11, b22); // b11 + b22

    // Calculating p1 to p7:
    vvi p1 = Strassen(aResult, bResult); // p1 = (a11 + a22) * (b11 + b22)

    aResult = matrix_add(a21, a22); // p2 = a21 + a22
    vvi p2 = Strassen(aResult, b11); // p2 = (a21 + a22) * b11

    bResult = matrix_sub(b12, b22); // b12 - b22
    vvi p3 = Strassen(a11, bResult); //p3 = a11 * (b12 - b22)

    bResult = matrix_sub(b21, b11); // b21 - b11
    vvi p4 = Strassen(a22, bResult); // p4 = a22 * (b21 - b11)

    aResult = matrix_add(a11, a12); // a11 + a12
    vvi p5 = Strassen(aResult, b22); // p5 = (a11 + a12) * b22

    aResult = matrix_sub(a21, a11); // a21 - a11
    bResult = matrix_add(b11, b12); // b11 + b12
    vvi p6 = Strassen(aResult, bResult); // p6 = (a22 - a11) * (b11 + b12)

    aResult = matrix_sub(a12, a22); // a12 - a22
    bResult = matrix_add(b21, b22); // b21 + b22
    vvi p7 = Strassen(aResult, bResult); //p7 = (a12 - a22) * (b21 + b22)

    // calculating c21, c21, c11 and c22:
    vvi c12 = matrix_add(p3, p5); // c12 = p3 + p5
    vvi c21 = matrix_add(p2, p4); // c21 = p2 + p4

    aResult = matrix_add(p1, p4); // p1 + p4
    bResult = matrix_add(aResult, p7); // p1 + p4 + p7
    vvi c11 = matrix_sub(bResult, p5); // c11 = p1 + p4 - p5 + p7

    aResult = matrix_add(p1, p3); // p1 + p3
    bResult = matrix_add(aResult, p6); // p1 + p3 + p6
    vvi c22 = matrix_sub(bResult, p2); // c22 = p1 - p2 + p3 + p6

    // Grouping the results obtained in a single matrix
    vvi c(n, vi(n, 0));
    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            c[i][j] = c11[i][j];
            c[i][j + newSize] = c12[i][j];
            c[i + newSize][j] = c21[i][j];
            c[i + newSize][j + newSize] = c22[i][j];
        }
    }

    return c;
}

int main(int argc, char* argv[]) {
    vvi a, b;
    load(a, b); //load matrix a and b

    //If the dimension matrix is ​​not in the standard 2^k form, re-normalize
    int n = (int)a.size();
    int m = (int)a[0].size();
    int p = (int)b[0].size();
    cout << "n, m, p = " << n << " " << m << " " << p << endl;

    int mx = max(max(n, m), p);
    int pw2 = 1;
    while (pw2 < mx) pw2 *= 2;
    
    cout << "size 2^k = " << pw2 << endl;
    norm(pw2, a);
    norm(pw2, b);

    vvi c1 = multiply(a, b); ///O(N^3)
    vvi c2 = Strassen(a, b); ///O(N^log2(7))

    assert(c1 == c2);

    //print result matrix a x b
    print(c2);

    return 0;
}