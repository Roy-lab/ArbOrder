//
// Created by Halberg, Spencer on 6/4/25.
//

#include "Matrix.h"
#include <stdexcept>
#include <string>
#include <fstream>
#include <iostream>


Matrix::Matrix()
{
	m_K = 0;
	m_Matrix = std::vector<std::vector<double>>();

}

Matrix::Matrix(int k)
{
	m_K = k;
	m_Matrix = std::vector<std::vector<double>>(k, std::vector<double>(k, 0));
}

int Matrix::setElement(int i, int j, double const value)
{
	if (i >= m_K || j >= m_K || i < 0 || j < 0) {
		throw std::out_of_range("Index out of range");
	}

	m_Matrix[i][j] = value;
	return 0;
}

double Matrix::getElement(int i, int j){
	if (i >= m_K || j >= m_K || i < 0 || j < 0)
	{
		throw std::out_of_range("Index out of range");
	}
	return m_Matrix[i][j];
}

int Matrix::swapRows(int row1, int row2) {
	if (row1 < m_K && row2 < m_K) {
		std::swap(m_Matrix[row1], m_Matrix[row2]);
	} else {
		throw std::out_of_range("Row index out of range");
	}
	return 0;
}


int Matrix::swapCols(int col1, int col2) {
	if (col1 < m_K && col2 < m_K) {
		// Iterate over each row and swap elements in col1 and col2
		for (int i = 0; i < m_K; ++i) {
			std::swap(m_Matrix[i][col1], m_Matrix[i][col2]);
		}
	} else {
		throw std::out_of_range("Column index out of range");
	}
	return 0;
}

void Matrix::reorderRows(const std::map<int, int>* rowPerm) {
	if (rowPerm == nullptr) {
		throw std::invalid_argument("Permutation map pointer cannot be null");
	}
	if (rowPerm->size() != m_K) {
		throw std::out_of_range("Permutation map size must match the number of rows");
	}
	std::vector<std::vector<double>> tempMatrix = m_Matrix;
	for (auto const& [oldIdx, newIdx] : *rowPerm) {
		if (oldIdx >= m_K || newIdx >= m_K) {
			throw std::out_of_range("Row index in permutation map is out of range");
		}
		tempMatrix[newIdx] = m_Matrix[oldIdx];
	}
	m_Matrix = tempMatrix;
}


void Matrix::reorderCols(const std::map<int, int>* colPerm) {
	if (colPerm == nullptr) {
		throw std::invalid_argument("Permutation map pointer cannot be null");
	}

	if (colPerm->size() != m_K) {
		throw std::out_of_range("Permutation map size must match the number of columns");
	}

	// Create a temporary matrix to store the reordered columns
	std::vector<std::vector<double>> tempMatrix(m_K, std::vector<double>(m_K, 0));

	for (int row = 0; row < m_K; ++row) {
		for (auto const& [oldIdx, newIdx] : *colPerm) {
			if (oldIdx >= m_K || newIdx >= m_K) {
				throw std::out_of_range("Column index in permutation map is out of range");
			}
			// Copy column oldIdx into newIdx for the current row
			tempMatrix[row][newIdx] = m_Matrix[row][oldIdx];
		}
	}

	// Replace the original matrix with the updated one
	m_Matrix = tempMatrix;
}


void Matrix::writeMatrix(string outfile)
{
	ofstream fout(outfile);
	if (!fout.is_open())
	{
		cout << "Could not open file for writing" << endl;
		return;
	}

	for (int i = 0; i < m_K; ++i)
	{
		for (int j = 0; j < m_K; ++j)
		{
			fout << m_Matrix[i][j];
			if (j < m_K - 1)
			{
				fout << "\t";
			}
		}
		fout << std::endl;
	}

	fout.close();
}
