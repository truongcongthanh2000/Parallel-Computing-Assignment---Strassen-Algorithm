#include <stdio.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <assert.h>

using type = int; //if value >= 2^31, change type = long long;

// Run code: g++ -std=c++11 strassen_dac.cpp -o strassen_dac
//           ./strassen_dac THRESHOLD
template <typename T>
T **Allocate2DArray(int nRows, int nCols) {
    //(step 1) allocate memory for array of elements of column
    T **ppi = new T*[nRows];

    //(step 2) allocate memory for array of elements of each row and initial all value is 0
    T *curPtr = new T [nRows * nCols];

    std::fill(curPtr, curPtr + (nRows * nCols), 0);

    // Now point the pointers in the right place
    for (int i = 0; i < nRows; i++) {
        *(ppi + i) = curPtr;
        curPtr += nCols;
    }
    return ppi;
}

template <typename T>
void Free2DArray(T** Array)
{
    delete [] *Array;
    delete [] Array;
}

void multiply_leaf(int mfirst, int mlast, int nfirst, int nlast, int pfirst, int plast, type **A, type **B, type **C)
/*
  subroutine that uses the simple triple loop to multiply
  a submatrix from A with a submatrix from B and store the
  result in a submatrix of C.
*/
// mfirst, mlast; /* first and last+1 i index */
// nfirst, nlast; /* first and last+1 j index */
// pfirst, plast; /* first and last+1 k index */
{
	for (int i = mfirst; i < mlast; i++) {
		for (int j = nfirst; j < nlast; j++) {
			C[i][j] = 0;
			for (int k = pfirst; k < plast; k++){
				C[i][j] += A[i][k] * B[k][j];
			}
		}
    }
}

void convertSubMatrix(type **sub_matrix, int nRows, type **matrix, int mfirst, int nfirst) {
    for (int i = 0; i < nRows; i++) {
        sub_matrix[i] = &matrix[mfirst + i][nfirst];
    }
}

void addMatrix(type **target, int m, int n, type **a, type **b) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) target[i][j] = a[i][j] + b[i][j];
    }
}

void subMatrix(type **target, int m, int n, type **a, type **b) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) target[i][j] = a[i][j] - b[i][j];
    }
}

int THRESHOLD;

// size matrix: A = m x p, B = p x n, C = m x n
void strassen(int m, int n, int p, type **A, type **B, type **C) {
    if (m == 1 || n == 1 || p == 1 || std::max(n, std::max(n, p)) <= THRESHOLD) {
        multiply_leaf(0, m, 0, n, 0, p, A, B, C);
        return;
    }

    int m2 = (m / 2) + (m % 2);
    int n2 = (n / 2) + (n % 2);
    int p2 = (p / 2) + (p % 2);

    type **M1 = Allocate2DArray< type >(m2, n2);
    type **M2 = Allocate2DArray< type >(m2, n2);
    type **M3 = Allocate2DArray< type >(m2, n2);
    type **M4 = Allocate2DArray< type >(m2, n2);
    type **M5 = Allocate2DArray< type >(m2, n2);
    type **M6 = Allocate2DArray< type >(m2, n2);
    type **M7 = Allocate2DArray< type >(m2, n2);

    type **wAM1 = Allocate2DArray< type >(m2, p2);
    type **wBM1 = Allocate2DArray< type >(p2, n2);
    type **wAM2 = Allocate2DArray< type >(m2, p2);
    type **wBM3 = Allocate2DArray< type >(p2, n2);
    type **wBM4 = Allocate2DArray< type >(p2, n2);
    type **wAM5 = Allocate2DArray< type >(m2, p2);
    type **wAM6 = Allocate2DArray< type >(m2, p2);
    type **wBM6 = Allocate2DArray< type >(p2, n2);
    type **wAM7 = Allocate2DArray< type >(m2, p2);
    type **wBM7 = Allocate2DArray< type >(p2, n2);

    type **A11 = new type*[m2];
    type **A12 = new type*[m2];
    type **A21 = new type*[m2];
    type **A22 = new type*[m2];

    type **B11 = new type*[p2];
    type **B12 = new type*[p2];
    type **B21 = new type*[p2];
    type **B22 = new type*[p2];

    type **C11 = new type*[m2];
    type **C12 = new type*[m2];
    type **C21 = new type*[m2];
    type **C22 = new type*[m2];

    convertSubMatrix(A11, m2, A,  0,  0);
    convertSubMatrix(A12, m2, A,  0, p2);
    convertSubMatrix(A21, m2, A, m2,  0);
    convertSubMatrix(A22, m2, A, m2, p2);

    convertSubMatrix(B11, p2, B,  0,  0);
    convertSubMatrix(B12, p2, B,  0, n2);
    convertSubMatrix(B21, p2, B, p2,  0);
    convertSubMatrix(B22, p2, B, p2, n2);

    convertSubMatrix(C11, m2, C,  0,  0);
    convertSubMatrix(C12, m2, C,  0, n2);
    convertSubMatrix(C21, m2, C, m2,  0);
    convertSubMatrix(C22, m2, C, m2, n2);


    {
	    // M1 = (A11 + A22)*(B11 + B22)
		addMatrix(wAM1, m2, p2, A11, A22);
		addMatrix(wBM1, p2, n2, B11, B22);
		strassen(m2, n2, p2, wAM1, wBM1, M1);
	}
    {
        //M2 = (A21 + A22)*B11
		addMatrix(wAM2, m2, p2, A21, A22);
		strassen(m2, n2, p2, wAM2, B11, M2);
    }
    {
        //M3 = A11*(B12 - B22)
		subMatrix(wBM3, p2, n2, B12, B22);
		strassen(m2, n2, p2, A11, wBM3, M3);
    }
    {
        //M4 = A22*(B21 - B11)
		subMatrix(wBM4, p2, n2, B21, B11);
		strassen(m2, n2, p2, A22, wBM4, M4);
    }
    {
        //M5 = (A11 + A12)*B22
		addMatrix(wAM5, m2, p2, A11, A12);
		strassen(m2, n2, p2, wAM5, B22, M5);
    }
    {
        //M6 = (A21 - A11)*(B11 + B12)
		subMatrix(wAM6, m2, p2, A21, A11);
		addMatrix(wBM6, p2, n2, B11, B12);
		strassen(m2, n2, p2, wAM6, wBM6, M6);
    }
    {
        //M7 = (A12 - A22)*(B21 + B22)
		subMatrix(wAM7, m2, p2, A12, A22);
		addMatrix(wBM7, p2, n2, B21, B22);
		strassen(m2, n2, p2, wAM7, wBM7, M7);
    }

    for (int i = 0; i < m2; i++) {
        for (int j = 0; j < n2; j++) {
            C11[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
            C12[i][j] = M3[i][j] + M5[i][j];
            C21[i][j] = M2[i][j] + M4[i][j];
            C22[i][j] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
        }
    }
    Free2DArray< type >(M1);
    Free2DArray< type >(M2);
    Free2DArray< type >(M3);
    Free2DArray< type >(M4);
    Free2DArray< type >(M5);
    Free2DArray< type >(M6);
    Free2DArray< type >(M7);

    Free2DArray< type >(wAM1);
    Free2DArray< type >(wBM1);
    Free2DArray< type >(wAM2);
    Free2DArray< type >(wBM3);
    Free2DArray< type >(wBM4);
    Free2DArray< type >(wAM5);
    Free2DArray< type >(wAM6);
    Free2DArray< type >(wBM6);
    Free2DArray< type >(wAM7);
    Free2DArray< type >(wBM7);

    delete[] A11; delete[] A12; delete[] A21; delete[] A22;
    delete[] B11; delete[] B12; delete[] B21; delete[] B22;
    delete[] C11; delete[] C12; delete[] C21; delete[] C22;
}

// find the optimal number of zero rows to pad given the number of rows in the matrix.
// Instad of padding to the next power of 2, pad to the least z = m * 2^k
// such that m < threshold; k is an integer
int findOptDim(int n, int convThreshold){
    int counter = 0;
    while (n > convThreshold){
        if (n%2 == 0)
            n /= 2;
        else
            n = (n+1)/2;
        counter ++;
    }
    int ans = n;
    while (counter--) ans *= 2;
    return ans;
}

void initPadding(type** &M, int old_m, int old_n, int new_m, int new_n){
    type **M_copy;
    M_copy = Allocate2DArray<type>(new_m, new_n);
    for (int i = 0; i < old_m; i++) {
        for (int j = 0; j < old_n; j++) {
            M_copy[i][j] = M[i][j];
        }
    }

    Free2DArray<type>(M);
    M = Allocate2DArray<type>(new_m, new_n);
    for (int i = 0; i < new_m; i++) {
        for (int j = 0; j < new_n; j++) {
            M[i][j] = M_copy[i][j];
        }
    }
    Free2DArray<type>(M_copy);
}

void removePadding(type** &M, int new_m, int new_n){
    type **M_copy;
    M_copy = Allocate2DArray<type>(new_m, new_n);
    for (int i = 0; i < new_m; i++) {
        for (int j = 0; j < new_n; j++) {
            M_copy[i][j] = M[i][j];
        }
    }

    Free2DArray<type>(M);
    M = Allocate2DArray<type>(new_m, new_n);
    for (int i = 0; i < new_m; i++) {
        for (int j = 0; j < new_n; j++) {
            M[i][j] = M_copy[i][j];
        }
    }
    Free2DArray<type>(M_copy);

}

// size matrix: A = m x p, B = p x n, C = m x n
void multiply(int m, int n, int p, type** &A, type** &B, type** &C) {
    int padding_m = findOptDim(m, THRESHOLD);
    int padding_n = findOptDim(n, THRESHOLD);
    int padding_p = findOptDim(p, THRESHOLD);
    
    int size = std::max(padding_m, std::max(padding_n, padding_m));

    initPadding(A, m, p, size, size);

    initPadding(B, p, n, size, size);

    initPadding(C, m, n, size, size);
    {
        strassen(size, size, size, A, B, C);
    }

    removePadding(A, m, p);
    removePadding(B, p, n);
    removePadding(C, m, n);
}

void load(type** &A, type** &B, int &m, int &n, int &p) {
    std::fstream finp;
    finp.open("input.txt", std::ios::in);
    finp >> m >> p;
    A = Allocate2DArray<type>(m, p);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) finp >> A[i][j];
    }
    finp >> p >> n;
    B = Allocate2DArray<type>(p, n);
    for (int i = 0; i < p; i++) {
        for (int j = 0; j < n; j++) finp >> B[i][j];
    }
    finp.close();
}

int main(int argc, char* argv[]) {
    THRESHOLD = std::atoi(argv[1]);

    type **A;
    type **B;
    type **C;
    type **C_ans;
    int m, n, p;
    load(A, B, m, n, p);
    
    C = Allocate2DArray<type>(m, n);
    C_ans = Allocate2DArray<type>(m, n);

    clock_t begin, end;
    begin = clock();

    multiply_leaf(0, m, 0, n, 0, p, A, B, C_ans);

    end = clock();

    std::cout << "Time naive algorithm O(N^3) = " << std::fixed << std::setprecision(10) << (double)(end - begin) / (double)CLOCKS_PER_SEC << std::endl;

    std::cerr << "Time naive algorithm O(N^3) = " << std::fixed << std::setprecision(10) << (double)(end - begin) / (double)CLOCKS_PER_SEC << std::endl;

    begin = clock();

    multiply(m, n, p, A, B, C);

    end = clock();

    std::cout << "Time divide and conquer algorithm O(N^log2(7)) = " << std::fixed << std::setprecision(10) << (double)(end - begin) / (double)CLOCKS_PER_SEC << std::endl;

    std::cerr << "Time divide and conquer algorithm O(N^log2(7)) = " << std::fixed << std::setprecision(10) << (double)(end - begin) / (double)CLOCKS_PER_SEC << std::endl;

    bool ok = true;
    for (int i = 0; i < m && ok; i++) {
        for (int j = 0; j < n; j++) {
            if (C[i][j] != C_ans[i][j]) {
                ok = false;
                break;
            }
        }
    }

    // std::cout << "Answer = " << std::endl;
    // for (int i = 0; i < m; i++) {
    //     for (int j = 0; j < n; j++) {
    //         std::cout << C_ans[i][j] << ' ';
    //     }
    //     std::cout << std::endl;
    // }

    // std::cout << "My answer = " << std::endl;
    // for (int i = 0; i < m; i++) {
    //     for (int j = 0; j < n; j++) {
    //         std::cout << C[i][j] << ' ';
    //     }
    //     std::cout << std::endl;
    // }
    if (!ok) {
        std::cout << "Wrong Answer!" << std::endl;
    }
    else {
        std::cout << "Correct Answer!" << std::endl;
    }

    Free2DArray<type> (A);
    Free2DArray<type> (B);
    Free2DArray<type> (C);
    Free2DArray<type> (C_ans);
}
