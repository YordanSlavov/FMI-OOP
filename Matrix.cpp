#include "Matrix.h"

Matrix::Matrix(size_t rows, size_t columns)
{
	if (rows == columns && rows!=0 && columns!=0)
	{
		matrix = new Array<int>[rows];
		for (size_t i = 0; i < rows; ++i)
		{
			matrix[i].resize(columns);
		}
		fillMatrix(rows, columns);
	}
	else
	{
		std::cout << "Inappropriate matrix dimensions.\n";
		matrix = nullptr;
	}
}

void Matrix::copy(const Matrix& other)
{
	size_t upTo = other.matrix->size();
	matrix = new Array<int>[upTo];
	for (size_t i = 0; i < upTo; ++i)
	{
		matrix[i] = other.matrix[i];
	}
}

void Matrix::clear()
{
	delete[]matrix;
	matrix = nullptr;
}

Matrix::~Matrix()
{
	clear();
}

Matrix::Matrix(const Matrix& other)
{
	copy(other);
}

Matrix& Matrix::operator=(const Matrix& other)
{
	if (this != &other)
	{
		clear();
		copy(other);
	}
	return *this;
}

Matrix::Matrix(Matrix&& other)
{
	matrix = other.matrix;
	other.matrix = nullptr;
}

Matrix& Matrix::operator=(Matrix&& other)
{
	if (matrix != nullptr)
	{
		clear();
	}
	matrix = other.matrix;
	other.matrix = nullptr;

	return *this;
}

Matrix Matrix::operator+(const Matrix& other)const
{
	size_t dimensionFirst = matrix->size();
	size_t dimensionSecond = other.matrix->size();
	Matrix result;
	if (dimensionFirst == dimensionSecond)
	{
		result.matrix = new Array<int>[dimensionFirst];
		for (size_t i = 0; i < dimensionFirst; ++i)
		{
			result.matrix[i].resize(dimensionFirst);
			result.matrix[i] = this->matrix[i] + other.matrix[i];
		}
	}
	
	return result;
}

Matrix Matrix::operator-(const Matrix& other)const
{
	size_t dimensionFirst = matrix->size();
	size_t dimensionSecond = other.matrix->size();
	Matrix result;
	if (dimensionFirst == dimensionSecond)
	{
		result.matrix = new Array<int>[dimensionFirst];
		for (size_t i = 0; i < dimensionFirst; ++i)
		{
			result.matrix[i].resize(dimensionFirst);
			result.matrix[i] = this->matrix[i] - other.matrix[i];
		}
	}

	return result;
}

Matrix& Matrix::operator*(int lambda)
{
	size_t upTo = matrix->size();
	for (size_t i = 0; i < upTo; ++i)
	{
		matrix[i].operator*(lambda);
	}

	return *this;
}

//very slow for large matrices due to
//multiple multiplications and copies
Matrix& Matrix::operator^(size_t power)
{
	if (power == 0) //returning E
	{
		for (size_t i = 0; i < matrix->size(); ++i)
		{
			for (size_t j = 0; j < matrix->size(); ++j)
			{
				if (i != j)
				{
					matrix[i][j] = 0;
				}
				else
				{
					matrix[i][j] = 1;
				}
			}
		}
		return *this;
	}
	if (power > 1)
	{
		while (power>1)
		{
			Matrix copyOfInitial(*this);
			*this = (*this).operator*(copyOfInitial);
			--power;
		}
	}

	return *this;
}
//O(n^3)
Matrix Matrix::operator*(const Matrix& other)const
{

	if (matrix->size() == other.matrix[0].size())
	{
		size_t dimension = matrix->size();
		Matrix result;
		result.matrix = new Array<int>[dimension];
		for (size_t i = 0; i < dimension; ++i)
		{
			result.matrix[i].resize(dimension);
			for (size_t j = 0; j < dimension; ++j)
			{
				result.matrix[i].push_back(0);
			}
		}
		

		for (size_t i = 0; i < dimension; ++i)
		{
			for (size_t j = 0; j < matrix[i].size(); ++j)
			{
				for (size_t t = 0; t < dimension; ++t)
				{
					result.matrix[i][j]+= this->matrix[i][t] * other.matrix[t][j];
				}
			}
		}

		return result;
	}
	else
	{
		std::cout << "Inappropriate matrix dimensions.\n";
	}
}

Matrix Matrix::transpose()const
{
	size_t dimension = matrix->size();
	Matrix result;
	result.matrix = new Array<int>[dimension];

	for (size_t i = 0; i < dimension; ++i)
	{
		for (size_t j = 0; j < dimension; ++j)
		{
			result.matrix[i].push_back(matrix[j][i]);
		}
	}
	return result;
}

void Matrix::print() const
{
	for (size_t i = 0; i < matrix->size(); ++i)
	{
		matrix[i].print();
	}
}

void Matrix::fillMatrix(size_t rows, size_t columns)
{
	int number;

	for (size_t i = 0; i < rows; ++i)
	{
		for (size_t j = 0; j < columns; ++j)
		{
			std::cin >> number;
			matrix[i].push_back(number);
		}
	}
}
