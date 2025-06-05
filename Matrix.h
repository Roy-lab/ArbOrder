//
// Created by Halberg, Spencer on 6/4/25.
//

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <map>
#include <string>

using namespace std;


class Matrix {
public:
	Matrix();
	Matrix(int k);

	int setElement(int i, int j, double const value);
	double getElement(int i, int j);

	int swapRows(int i, int j);
	int swapCols(int i, int j);
	void reorderCols(const map<int, int>* colPerm);
	void reorderRows(const map<int, int>* rowPerm);
	void writeMatrix(string outfile);
private:
	int m_K;
	vector<vector<double>> m_Matrix;
};



#endif //MATRIX_H
