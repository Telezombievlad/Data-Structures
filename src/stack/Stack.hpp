#ifndef HEADER_GUARD_MY_STACK_HPP_INCLUDED
#define HEADER_GUARD_MY_STACK_HPP_INCLUDED

#include "../exception/MyException.hpp"
#include "../array/Vector.hpp"

#define PROGRAM_POS __FILE__, __FUNCTION__, __LINE__

namespace MyStackUsingVectorImplementation
{
	namespace MyVector = MyVectorResizableDynamicArrayOfPointersRepresentation;

	namespace MyException = MyExceptionCharStringRepresentation;

	template <class T>
	class Stack : private MyVector::Vector<T>
	{
	public:
		// Constructors && destructor:
			Stack() : 
				MyVector::Vector<T>(0, true),
				filled_ (0) 
			{}

			Stack(std::initializer_list<T> list) :
				MyVector::Vector<T>(list),
				filled_ (list.size())
			{}

			Stack(const Stack& that) : 
				MyVector::Vector<T>(that),
				filled_ (that.filled_)
			{}

			Stack(Stack&& that) :
				MyVector::Vector<T>(that),
				filled_ (that.filled_)
			{}

			~Stack() {}

		// Access: 
			Stack& push(const T& toPush)
			{
				try
				{
					MyVector::Vector<T>::set(filled_, toPush);

					++filled_;
				}
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Stack::push(): Can't push", PROGRAM_POS, exc);
				}

				return *this;
			}

			T& pop()
			{
				using Vect = MyVector::Vector<T>;

				if (filled_ == 0)
				{
					throw MyException::Exception("Stack::pop(): Can't pop from empty stack", PROGRAM_POS);
				}

				try
				{
					T& toReturn = Vect::rip(filled_ - 1);

					--filled_;

					/*if (filled_ == 0) Vect::resize(0);
					else */if (filled_ < Vect::size() / 4) Vect::resize(Vect::size() / 2);

					return toReturn;
				}
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Stack::pop(): Can't pop", PROGRAM_POS, exc);
				}
			}

			const T& head()
			{
				if (filled_ == 0)
				{
					throw MyException::Exception("Stack::head(): Empty steak has no head element", PROGRAM_POS);
				}

				return MyVector::Vector<T>::get(filled_ - 1);
			}

	private: 
		size_t filled_;
	};
}

#undef PROGRAM_POS

#endif /*HEADER_GUARD_MY_STACK_HPP_INCLUDED*/