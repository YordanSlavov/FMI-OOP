#ifndef __MATRIX__H__
#define __MATRIX__H__

#include "Array.hpp"

class Matrix
{
	//pointer to an array of arrays of ints
	//will act as a n by  matrix
private:
	Array<int>* matrix;

	//Big 4
public:
	Matrix() :matrix(nullptr) {};
	~Matrix();
	Matrix(size_t, size_t); //allocating memory for a NxN matrix
	Matrix(const Matrix&);
	Matrix& operator=(const Matrix&);

	//move semantics
public:
	Matrix(Matrix &&);
	Matrix& operator=(Matrix&&);

	//helpers
private:
	void copy(const Matrix&);
	void clear();
	void fillMatrix(size_t, size_t);

	//addition, subtraction, multiplication by number/another matrix,
	//power raising
	//A, B - matrices
	//lambda is an int
	//k - natural number
public:
	Matrix operator+(const Matrix&)const; // A + B = C
	Matrix operator-(const Matrix&)const; // A - B = C
	Matrix operator*(const Matrix&)const; // A*B = C
	Matrix transpose()const; // A^t = B
	Matrix& operator*(int); // lambda*A
	Matrix& operator^(size_t); // A^k
	void print()const;
}; //Matrix

#endif
