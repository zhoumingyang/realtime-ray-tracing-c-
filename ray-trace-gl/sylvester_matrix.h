#pragma once
#ifndef SYLMATRIX_H
#define SYLMATRIX_H
#include "sylvester_common.h"
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;
class Vector;
class Matrix {
public:
	Matrix();
	Matrix(int m, int n);
	Matrix(const vector<vector<float>>& v);
	Matrix(float** v, int m, int n);
	~Matrix();
	vector<vector<float>> allElements() const;
	vector<float> row(const int& i) const;
	vector<float> col(const int& j) const;
	void dimesions(int& rows, int& cols) const;
	int rows() const;
	int cols() const;
	bool eql(const Matrix& m) const;
	Matrix dup() const;
	Matrix map(float(*f)(float e, int i, int j));
	Matrix zero(const int& n) const;
	Matrix zero(const int& n, const int&m) const;
	Matrix identity(const int& n) const;
	Matrix Diagonal(const vector<float>& elements) const;
	bool isSameSizeAs(Matrix mat) const;
	Matrix add(Matrix mat) const;
	Matrix subtract(const Matrix& mat) const;
	bool canMultiplyFromLeft(const Matrix& mat) const;
	Matrix multiply(const Matrix& mat) const;
	Matrix multiply(const float& x) const;
	Vector multiply(const Vector& vec) const;
	Matrix minor(const int& startRow, const int& startCol, const int& nrows, const int& ncols) const;
	Matrix transpose() const;
	bool isSquare() const;
	float max() const;
	void indexOf(const float& x, int& i, int& j) const;
	Matrix toRightTriangular() const;
	Matrix toUpperTriangular() const;
	float determinant() const;
	float det() const;
	bool isSingular() const;
	float trace() const;
	float tr() const;
	int rank() const;
	int rk() const;
	Matrix augment(const Matrix& mat) const;
	Matrix inverse() const;
	Matrix inv() const;
	Matrix round()const;
	Matrix snapTo(float x) const;
	void setElements(Matrix mat);
	void setElements(vector<vector<float>> mat);
	Matrix I(int n) const;
	Matrix rotation(const float& theta, const Vector& axis) const;
	Matrix rotationX(const float& theta) const;
	Matrix rotationY(const float& theta) const;
	Matrix rotationZ(const float& theta) const;
	Matrix translation(const Vector& v) const;
	vector<float> flatten() const;
	Matrix ensure4x4() const;
private:
	vector<vector<float>> elements;
};
#endif // !SYLMATRIX_H
