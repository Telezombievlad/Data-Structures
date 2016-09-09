#include "array/Vector.hpp"
#include <iostream>
#include <cstdlib>

namespace MyVector = MyVectorResizableDynamicArrayOfPointersRepresentation;
namespace MyException = MyExceptionCharStringRepresentation;

template <class T>
void printArray(MyVector::Vector<T> arr)
{
	arr.throwIfNotOk();

	for (size_t i = 0; i < arr.size(); ++i)
	{
		if (arr.emptyAt(i)) std::cout << "null" << " ";
		else std::cout << arr[i] << " ";
	}

	std::cout << std::endl;
}

int main()
{
	try
	{
		MyVector::Vector<double> arr0(5);

		printArray(arr0);

		MyVector::Vector<double> arr1{1.0, 2.0, 3.0, 5.0, 6.0};

		arr0 = std::move(arr1);
		
		printArray(arr0);

		arr0.resize(6);

		printArray(arr0);

		arr0.setAutoResize();

		arr0[5] = 2.28;

		printArray(arr0);
	}
	catch (MyException::Exception ex)
	{
		std::cout << ex.what() << std::endl;
	}

	return EXIT_SUCCESS;
}