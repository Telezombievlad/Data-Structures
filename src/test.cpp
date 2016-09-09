#include "stack/Stack.hpp"
#include <iostream>
#include <cstdlib>

namespace MyStack = MyStackUsingVectorImplementation;
namespace MyException = MyExceptionCharStringRepresentation;



int main()
{
	try
	{
		MyStack::Stack<double> st0{2.28, 13.37, 4.2, 6.66};

		std::cout << st0.pop() << std::endl;
		std::cout << st0.pop() << std::endl;
		std::cout << st0.pop() << std::endl;
		st0.push(7.77);
		std::cout << st0.pop() << std::endl;
		std::cout << st0.head() << std::endl;	
	}
	catch (MyException::Exception ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (std::exception ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unexpected exception" << std::endl;
	}

	return EXIT_SUCCESS;
}