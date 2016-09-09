#ifndef HEADER_GUARD_MY_ARRAY_HPP_INCLUDED
#define HEADER_GUARD_MY_ARRAY_HPP_INCLUDED

#include "../exception/MyException.hpp"

#define PROGRAM_POS __FILE__, __FUNCTION__, __LINE__


namespace MyArrayConstSizeStaticArrayRepresentation
{
	namespace MyException = MyExceptionCharStringRepresentation;

	namespace _detail
	{
		enum class Guard : size_t
		{
			DESTRUCTED = 0xDE6D3ECD,
			GUARD0 	   = 0x12052016,
			GUARD1     = 0xFEEDDCAD
		};
	}

	template <class T, size_t size_>
	class Array
	{
	public:
		// Ctors && dtor:
			Array() = delete;

			explicit Array(const T& defaultVal) :
				guard0_ (_detail::Guard::GUARD0),
				array_  (),
				guard1_ (_detail::Guard::GUARD1)
			{
				for (size_t i = 0; i < size_; ++i)
				{
					array_[i] = defaultVal;
				}
			}

			explicit Array(std::initializer_list<T> list) :
				guard0_ (_detail::Guard::GUARD0),
				array_  (),
				guard1_ (_detail::Guard::GUARD1)
			{
				if (list.size() != size_) 
				{
					throw MyException::Exception("Array::Array(initializer_list): size of initializer_list deffers from array size", PROGRAM_POS);
				}

				size_t i = 0;
				for (auto&& j = list.begin(); i < size_ && j != list.end(); ++i, ++j)
				{
					array_[i] = *j;
				}
			}

			Array(const Array& that) :
				guard0_ (that.guard0_),
				array_  (),
				guard1_ (that.guard1_)
			{
				try { that.throwIfNotOk(); }
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Array::Array(const Array&): Copying from messed up array object", PROGRAM_POS, exc);
				}

				for (size_t i = 0; i < size_; ++i)
				{
					array_[i] = that.array_[i];
				}	
			}

			Array(Array&& that)
			{
				try { that.throwIfNotOk(); }
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Array::Array(Array&&): Moving from messed up array object", PROGRAM_POS, exc);
				}

				std::swap(*this, that);
			}

			~Array()
			{
				if (guard0_ == _detail::Guard::DESTRUCTED) return;
				if (guard1_ == _detail::Guard::DESTRUCTED) return;

				guard0_ = _detail::Guard::DESTRUCTED;
				guard1_ = _detail::Guard::DESTRUCTED;
			}

		// Operator=:

			Array& operator=(const Array& that)
			{
				try { that.throwIfNotOk(); }
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Array::operator=(const Array&): Copying from messed up array object", PROGRAM_POS, exc);
				}

				for (size_t i = 0; i < size_; ++i)
				{
					array_[i] = that.array_[i];
				}
			}

			Array& operator=(Array&& that)
			{
				try { that.throwIfNotOk(); }
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Array::operator=(Array&&): Moving from messed up array object", PROGRAM_POS, exc);
				}

				std::swap(*this, that);

				return *this;
			}

		// Access:

			const T& get(size_t index) const
			{
				throwIfNotOk();

				if (index >= size_)
				{
					throw MyException::Exception("Array::get: Index out of bounds", PROGRAM_POS);
				}

				return array_[index];
			}

			Array& set(size_t index, const T& item)
			{
				throwIfNotOk();

				if (index >= size_)
				{
					throw MyException::Exception("Array::set: Index out of bounds", PROGRAM_POS);
				}

				array_[index] = item;

				return *this;
			}

			Array& set(size_t index, T&& item)
			{
				throwIfNotOk();

				if (index >= size_)
				{
					throw MyException::Exception("Array::set: Index out of bounds", PROGRAM_POS);
				}

				array_[index] = item;

				return *this;
			}

			T& operator[](size_t index)
			{
				throwIfNotOk();

				if (index >= size_)
				{
					throw MyException::Exception("Array::operator=: Index out of bounds", PROGRAM_POS);
				}

				return array_[index];
			}

		// Assertion:

			void throwIfNotOk() const
			{
				if (guard0_ == _detail::Guard::DESTRUCTED ||
					guard0_ == _detail::Guard::DESTRUCTED)
				{
					throw MyException::Exception("Array::throwIfNotOk(): Array is already destructed", PROGRAM_POS);
				}

				if (guard0_ != _detail::Guard::GUARD0 || guard1_ != _detail::Guard::GUARD1)
				{
					throw MyException::Exception("Array::throwIfNotOk(): Array's memory has been touched", PROGRAM_POS);
				}
			}

	private:
		// Variables:
			_detail::Guard guard0_;
			T array_[size_];
			_detail::Guard guard1_;
	};
}

#undef PROGRAM_POS

#endif /*HEADER_GUARD_MY_ARRAY_HPP_INCLUDED*/