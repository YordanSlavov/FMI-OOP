#ifndef __ARRAY__H__
#define __ARRAY__H__

#include <iostream>

template<typename T>
class Array
{
	//essential data
private:
	T* arr;
	size_t currSize;
	size_t capacity;

	//Big 4
public:
	Array() :arr(nullptr), currSize(0), capacity(0) {};
	~Array();
	Array(const Array&);
	Array& operator=(const Array&);

	//move semantics
public:
	Array(Array&&);
	Array& operator=(Array&&);

	//bidirectional
public:
	class BidirectionalIterator
	{
		//our pointer to data
private:
		T* element;

		//life cycle
public:
	    BidirectionalIterator(T* pos = nullptr) :element(pos) {}; //will have element point to pos, if no position is specified, our iterator will be null
		
		//interface
public:
		//pointer arithmetic for operator[]
		//undefined behaviour in case of out of bounds element
		T& operator*() {return *element;};
		T* operator->() {return element;};

		//basically support of pointer arithmetic in the way we're used to
		BidirectionalIterator& operator++()
		{ 
			++element;
			return *this; 
		};
		BidirectionalIterator operator++(int dummy)
		{
			BidirectionalIterator temp(element);
			++element;
			return temp;
		};
		BidirectionalIterator& operator--()
		{ 
			--element;
			return *this; 
		};
		BidirectionalIterator operator--(int dummy)
		{
			BidirectionalIterator temp(element);
			--element;
			return temp;
		};

		//comparison operators
		bool operator==(const BidirectionalIterator& other)const {return element == other.element;};
		bool operator!=(const BidirectionalIterator& other)const {return element != other.element;};
	}; //Array<T>::BidirectionalIterator

	class RandomAcessIterator :public BidirectionalIterator
	{
		
	public:

		RandomAcessIterator(T* pos = nullptr) :BidirectionalIterator(pos) {};

		T& operator[](size_t idx) {return *(element + idx);};  

		RandomAcessIterator& operator+=(int num) 
		{
			if (num > 0)
			{
				while (num)
				{
					++element;
					--num;
				}
			}

			else if (num < 0)
			{
				while (num)
				{
					--element;
					++num;
				}
			}

			return *this;
		};
		RandomAcessIterator& operator-=(int num) 
		{
			return *this += -num;
		};

		int operator-(const RandomAcessIterator& other) const
		{
			return element - other.element;
		}

		int operator+(const RandomAcessIterator& other)const
		{
			return element + other.element;
		}

		RandomAcessIterator operator-(int num)
		{
			RandomAcessIterator temp(element);
			return temp -= num;
		}
		RandomAcessIterator operator+(int num)
		{
			RandomAcessIterator temp(element);
			return temp += num;
		}

		bool operator<(const RandomAcessIterator& other)const {return element < other.element;}; //just comparing their adresses
		bool operator<=(const RandomAcessIterator& other)const {return !(element > other.element);}; //<= is the opposite of >
		bool operator>(const RandomAcessIterator& other)const {return element > other.element;};
		bool operator>=(const RandomAcessIterator& other)const {return !(element < other.element);};//>= is the opposite of <
	}; //Array<T>::RandomAcessIterator

	//helpers
private:
	void resizeDown(size_t);
	void copy(const Array&);
	int partition(int, int);

	//interface
public:
	T& operator[](size_t);
	const T& operator[](size_t)const;
	void push_back(const T&);
	void pop_back();
	size_t size()const {return currSize;};
	void resize(size_t);
	bool empty()const {return currSize==0;}; 
	T& back();
	const T& back()const;
	void clear();
	void print()const;
	T& front();
	const T& front()const;
	bool binarySearch(const T&, size_t, size_t)const;
	void quickSort(int, int);

	//useful iterator interface
public:
	RandomAcessIterator begin() {return RandomAcessIterator(arr);};
	RandomAcessIterator end() {return RandomAcessIterator(arr + currSize);};

public:
	Array& operator+=(const Array&);
	Array& operator-=(const Array&);
	Array operator+(const Array&)const;
	Array operator-(const Array&)const;
	Array& operator*(int);
}; //Array<T>

template<typename T>
void Array<T>::resizeDown(size_t newSize)
{
	T* temp = arr;
	arr = new T[newSize];

	for (size_t i = 0; i < newSize; ++i) 
	{
		arr[i] = temp[i];
	}
	currSize = newSize;
	capacity = newSize;
	delete[]temp;
	temp = nullptr;
}

template<typename T>
Array<T>& Array<T>::operator-=(const Array<T>& other)
{
	for (size_t i = 0; i < currSize; ++i)
	{
		arr[i] -= other.arr[i];
	}
	
	return *this;
}

template<typename T>
Array<T>& Array<T>::operator+=(const Array<T>& other)
{
	for (size_t i = 0; i < currSize; ++i)
	{
		arr[i] += other.arr[i];
	}

	return *this;
}

template<typename T>
Array<T>& Array<T>::operator*(int lambda)
{
	for (size_t i = 0; i < currSize; ++i)
	{
		arr[i] *= lambda;
	}

	return *this;
}

template<typename T>
int Array<T>::partition(int leftBound, int rightBound)
{
	int idx = leftBound;
	T pivot = arr[leftBound];

	for (int i = leftBound+1; i <=rightBound; ++i)
	{
		if (arr[i] <= pivot)
		{
			++idx;
			std::swap(arr[i], arr[idx]);
		}
	}

	std::swap(arr[idx], arr[leftBound]);

	return idx;
}

template <typename T>
void Array<T>::quickSort(int leftBound, int rightBound)
{
	if (leftBound < rightBound)
	{
		int pivotIndex = partition(leftBound, rightBound);
		quickSort(leftBound, pivotIndex - 1);
		quickSort(pivotIndex + 1, rightBound);
	}
}

//works only on sorted arrays and relies on
//T type having defined <, > and == operators
template<typename T>
bool Array<T>::binarySearch(const T& value, size_t leftBound, size_t rightBound)const
{
	//input was off and element is clearly not in array
	if (leftBound > rightBound) 
		return false;

	size_t middle = (leftBound + rightBound)/ 2;
	if (arr[middle]==value)
		return true;
	if (arr[middle] > value)
		return binarySearch(value, leftBound, middle - 1);
	if (arr[middle] < value)
		return binarySearch(value, middle + 1, rightBound);
	
	return false;
}

template <typename T>
const T& Array<T>::front()const
{
	return *(Iterator(this->begin()));
}

template<typename T>
T& Array<T>::front()
{
	return *(Iterator(this->begin()));
}

template<typename T>
void Array<T>::print()const
{
	for (size_t i = 0; i < currSize; ++i)
	{
		std::cout << arr[i] << " ";
	}
	std::cout << "\n";
}

template<typename T>
Array<T> Array<T>::operator-(const Array<T>& other)const
{
	Array<T> result;

	if (currSize == other.currSize)
	{
		result.arr = new T[other.currSize];
		for (size_t i = 0; i < other.currSize; ++i)
		{
			result.arr[i] = arr[i] - other.arr[i];
		}
			result.currSize = other.currSize;
			result.capacity = other.currSize;
	}

	return result;
}

template<typename T>
Array<T> Array<T>::operator+(const Array<T>& other)const
{
	Array<T> result;

	if (currSize == other.currSize)
	{
		result.arr = new T[other.currSize];
		for (size_t i = 0; i < other.currSize; ++i)
		{
			result.arr[i] = arr[i] + other.arr[i];
		}
		result.currSize = other.currSize;
		result.capacity = other.currSize;
	}
	
	return result;
}

//no bounds checking for this/non-constant version of this
//unlike at function of std::vector operator[] doesn't check for bounds
//and anything outside of bounds is expected to cause undefined behaviour
template<typename T>
const T& Array<T>::operator[](size_t idx)const
{
	return arr[idx];
}

template<typename T>
T& Array<T>::operator[](size_t idx)
{
	return arr[idx];
}

template<typename T>
T& Array<T>::back()
{
	return arr[currSize-1];
}

template<typename T>
const T& Array<T>::back()const
{
	return arr[currSize - 1];
}

template<typename T>
void Array<T>::pop_back()
{
	if (currSize > 0)
		--currSize;
	else
		std::cout << "Container is already empty.\n";
}

template<typename T>
void Array<T>::clear()
{
	delete[]arr;
	currSize = 0;
	capacity = 0;
	arr = nullptr;
}

template<typename T>
void Array<T>::push_back(const T& val)
{
	if (currSize >= capacity)
	{
		resize((2 * capacity) + 1);
	}
	arr[currSize] = val;
	++currSize;
}

template<typename T>
Array<T>::~Array()
{
	clear();
}

template<typename T>
Array<T>::Array(const Array<T>& other)
{
	copy(other);
}

template<typename T>
Array<T>& Array<T>::operator=(const Array<T>& other)
{
	if (this != &other)
	{
		clear();
		copy(other);
	}
	return *this;
}

template<typename T>
void Array<T>::copy(const Array<T>& other)
{
	size_t size = other.size();
	arr = new T[size];
	for (size_t i = 0; i < size; ++i)
	{
		arr[i] = other[i];
	}
	currSize = other.currSize;
	capacity = other.capacity;
}

template<typename T>
void Array<T>::resize(size_t newSize)
{
	if (newSize < currSize)
	{
		resizeDown(newSize);
	}
	else
	{
		T* temp = arr;
		arr = new T[newSize];

		for (size_t i = 0; i < currSize; ++i) //so we can increase our size	
		{
			arr[i] = temp[i];
		}

		capacity = newSize;
		delete[]temp;
		temp = nullptr;
	}
}

template<typename T>
Array<T>& Array<T>::operator=(Array<T>&& other)
{
	if (arr != nullptr)
		clear();
	arr = other.arr;
	currSize = other.currSize;
	capacity = other.capacity;
	other.arr = nullptr;

	return *this;
}

template<typename T>
Array<T>::Array(Array<T>&& other)
{
	arr = other.arr;
	currSize = other.currSize;
	capacity = other.capacity;
	other.arr = nullptr;
}
#endif
