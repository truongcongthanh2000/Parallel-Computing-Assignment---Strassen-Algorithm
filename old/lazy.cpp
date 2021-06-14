#include<iostream>
#include<vector>
#include<assert.h>
#include<fstream>
#include<time.h>
using namespace std;

typedef vector<vector<int>> matrix;
void load(matrix &a, matrix &b) {
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
matrix Mul(matrix &a, matrix &b){
    int colSize_a = a.at(0).size();
    int rowSize_a = a.size();
    int colSize_b = b.at(0).size();
    int rowSize_b = b.size();
    assert(colSize_a == rowSize_b);
    matrix c;
    vector<int> el;
    int e = 0;
    for(int i = 0; i < rowSize_a ; i++){
        el.clear();
        for(int j = 0; j < colSize_b ; j++ ){
            e = 0;
            for(int t = 0; t < rowSize_b ; t++){
                e += a.at(i).at(t)*b.at(t).at(j);
            }
            el.push_back(e);
        }
        c.push_back(el);
    }
    return c;
}
matrix Add(matrix &a, matrix &b){
    int colSize_a = a.at(0).size();
    int rowSize_a = a.size();
    int colSize_b = a.at(0).size();
    int rowSize_b = b.size();
    assert(colSize_a == colSize_b && rowSize_a == rowSize_b);
    matrix c;
    vector<int> el;
    for(int i=0;i<rowSize_a;i++){
        el.clear();
        for(int j=0;j<colSize_a;j++){
            el.push_back(a.at(i).at(j)+b.at(i).at(j));
        }
        c.push_back(el);
    }
    return c;
}
matrix Sub(matrix &a, matrix &b){
    int colSize_a = a.at(0).size();
    int rowSize_a = a.size();
    int colSize_b = a.at(0).size();
    int rowSize_b = b.size();
    assert(colSize_a == colSize_b && rowSize_a == rowSize_b);
    matrix c;
    vector<int> el;
    for(int i=0;i<rowSize_a;i++){
        el.clear();
        for(int j=0;j<colSize_a;j++){
            el.push_back(a.at(i).at(j)-b.at(i).at(j));
        }
        c.push_back(el);
    }
    return c;
}
void printMat(matrix &a){
    int rows = a.size();
    for(int i = 0; i < rows ; i++){
        int cols = a.at(i).size();
        for(int j = 0; j < cols ; j++)
            cout << a.at(i).at(j) << " ";
        cout << endl;
    }
}
void norm(matrix &a){
    int colSize_a = a.at(0).size();
    int rowSize_a = a.size();
    if(colSize_a%2!=0){
        for(int i=0;i<rowSize_a;i++)
            a.at(i).push_back(0);
        colSize_a++;
    }
    if(rowSize_a%2!=0){
        vector<int> el;
        for(int i=0;i<colSize_a;i++){
            el.push_back(0);
        }
        a.push_back(el);
    }
}
void de_norm(int prev_n, int prev_m, matrix &a) {
    int rowSize_a = a.size();
    int colSize_a = a.at(0).size();
    
    for (int i = 0; i < rowSize_a; i++) {
        while ((int)a[i].size() > prev_m) a[i].pop_back();
    }
    while ((int)a.size() > prev_n) a.pop_back();
    assert((int)a.size() == prev_n);
    assert(prev_n > 0);
    assert(prev_m > 0);
    for (int i = 0; i < prev_n; i++) assert((int)a[i].size() == prev_m);
}
matrix Strassen(matrix a, matrix b){
    int prevColA = a.at(0).size();
    int prevRowA = a.size();
    int prevColB = b.at(0).size();
    int prevRowB = b.size();
    assert(prevColA == prevRowB);
    norm(a);
    norm(b);
    int rowSize_a = a.size();
    int colSize_a = a.at(0).size();
    int rowSize_b = b.size();
    int colSize_b = b.at(0).size();
    if(min(rowSize_a,min(colSize_a,colSize_b))==2){
        matrix c = Mul(a,b);
        de_norm(prevRowA,prevColB,c);
        return c;
    }
    int rowA = rowSize_a/2;
    int m = colSize_a/2;
    int colB = colSize_b/2;
    matrix a11, a12, a21, a22, b11, b12, b21, b22;
    a11 = a12 = a21 = a22 = matrix(rowA, vector<int>(m,0));
    b11 = b12 = b21 = b22 = matrix(m, vector<int>(colB,0));
    for (int i = 0; i < rowA; i++) {
        for (int j = 0; j < m; j++) {
            a11[i][j] = a[i][j];  // top left
            a12[i][j] = a[i][j + m];  // top right
            a21[i][j] = a[i + rowA][j];  // bottom left
            a22[i][j] = a[i + rowA][j + m];  // bottom right
        }
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < colB; j++) {
            b11[i][j] = b[i][j];  // top left
            b12[i][j] = b[i][j + colB];  // top right
            b21[i][j] = b[i + m][j];  // bottom left
            b22[i][j] = b[i + m][j + colB];  // bottom right
        }
    }
    matrix aResult = Add(a11, a22); 
    matrix bResult = Add(b11, b22); 
    matrix p1 = Strassen(aResult, bResult); 
    aResult = Add(a21, a22); 
    matrix p2 = Strassen(aResult, b11); 
    bResult = Sub(b12, b22); 
    matrix p3 = Strassen(a11, bResult); 
    bResult = Sub(b21, b11); 
    matrix p4 = Strassen(a22, bResult); 
    aResult = Add(a11, a12); 
    matrix p5 = Strassen(aResult, b22); 
    aResult = Sub(a21, a11); 
    bResult = Add(b11, b12); 
    matrix p6 = Strassen(aResult, bResult); 
    aResult = Sub(a12, a22); 
    bResult = Add(b21, b22); 
    matrix p7 = Strassen(aResult, bResult); 
    matrix c12 = Add(p3, p5); 
    matrix c21 = Add(p2, p4); 
    aResult = Add(p1, p4); 
    bResult = Add(aResult, p7); 
    matrix c11 = Sub(bResult, p5); 
    aResult = Add(p1, p3); 
    bResult = Add(aResult, p6); 
    matrix c22 = Sub(bResult, p2); 
    matrix c(rowSize_a , vector<int>(colSize_b, 0));
    for (int i = 0; i < rowA; i++) {
        for (int j = 0; j < colB; j++) {
            c[i][j] = c11[i][j];
            c[i][j + colB] = c12[i][j];
            c[i + rowA][j] = c21[i][j];
            c[i + rowA][j + colB] = c22[i][j];
        }
    }
    de_norm(prevRowA, prevColB, c);
    return c;

}
int main(){
    matrix a,b;
    load(a,b);
    int n = (int)a.size();
    int m = (int)a[0].size();
    int p = (int)b[0].size();
    cout << "n, m, p = " << n << " " << m << " " << p << endl;
    clock_t start = clock();
    matrix c1 = Strassen(a, b);
    clock_t end = clock();
    cout << "Time elapsed for normal multiple: "<< (double)(((double) (end - start)) / CLOCKS_PER_SEC )<< endl;
    printMat(c1);
    start = clock();
    matrix c2 = Strassen(a, b);
    end = clock();
    cout << "Time elapsed for Strassen algorithm: "<< (double)(((double) (end - start)) / CLOCKS_PER_SEC )<< endl;
    printMat(c2);
    return 0;
}
