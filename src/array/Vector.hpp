#ifndef HEADER_GUARD_MY_VECTOR_HPP_INCLUDED
#define HEADER_GUARD_MY_VECTOR_HPP_INCLUDED

#include <new>

#include "../exception/MyException.hpp"

#define PROGRAM_POS __FILE__, __FUNCTION__, __LINE__

namespace MyVectorResizableDynamicArrayOfPointersRepresentation
{
	namespace MyException = MyExceptionCharStringRepresentation;

	namespace _detail
	{
		enum class Guard : size_t
		{
			DESTRUCTED = 0xDE6D3ECD,
			GUARD0     = 0x12052016,
			GUARD1     = 0xFEEDDCAD
		};
	}

	template <class T>
	class Vector
	{
	public:
		// Ctors && dtor:
			explicit Vector(size_t size, bool autoResize = false) :
				guard0_     (_detail::Guard::GUARD0),
				ptrs_       (size == 0 ? nullptr : new T*[size]()),
				size_       (size),
				autoResize_ (autoResize),
				guard1_     (_detail::Guard::GUARD1)
			{}

			Vector(const Vector& that) :
				guard0_     (_detail::Guard::GUARD0),
				ptrs_       (that.size_ == 0 ? nullptr : new T*[that.size_]()),
				size_       (that.size_),
				autoResize_ (that.autoResize_),
				guard1_     (_detail::Guard::GUARD1)
			{
				try { that.throwIfNotOk(); }
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Vector::Vector(const Vector&): Copying from messed up vector object", PROGRAM_POS, exc);
				}

				for (size_t i = 0; i < size_; ++i)
				{
					if (that.ptrs_[i] != nullptr) 
					{
						ptrs_[i] = new T();
						*ptrs_[i] = *that.ptrs_[i];
					}
					else ptrs_[i] = nullptr;
				}
			}

			Vector(Vector&& that) : Vector(0, false)
			{
				try { that.throwIfNotOk(); }
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Vector::Vector(Vector&&): Moving from messed up vector object", PROGRAM_POS, exc);
				}

				swap(*this, that);
			}

			Vector(std::initializer_list<T> list) :
				guard0_     (_detail::Guard::GUARD0),
				ptrs_       ((list.size() == 0)? nullptr : new T*[list.size()]()),
				size_       (list.size()),
				autoResize_ (false),
				guard1_     (_detail::Guard::GUARD1)
			{
				auto j = list.begin();
				for (size_t i = 0; i < size_ && j != list.end(); ++i, ++j)
				{
					ptrs_[i] = new T();
					*ptrs_[i] = *j;
				}
			}

			~Vector()
			{
				if (guard0_ == _detail::Guard::DESTRUCTED) return;
				if (guard1_ == _detail::Guard::DESTRUCTED) return;

				guard0_ = _detail::Guard::DESTRUCTED;
				guard1_ = _detail::Guard::DESTRUCTED;

				for (size_t i = 0; i < size_; ++i)
				{
					if (ptrs_[i] != nullptr) delete ptrs_[i];
				}

				if (ptrs_ != nullptr) delete[] ptrs_;
				ptrs_ = nullptr;
			}

		// Operator=:
			Vector& operator=(const Vector& that)
			{
				try { that.throwIfNotOk(); }
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Vector::operator=(const Vector&): Invalid vector", PROGRAM_POS, exc);
				}

				try { that.throwIfNotOk(); }
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Vector::operator=(const Vector&): Copying from messed up vector object", PROGRAM_POS, exc);
				}

				Vector toSwap = that;

				swap(*this, toSwap);

				return *this;
			}

			Vector& operator=(Vector&& that)
			{
				if (&that == this) return *this;

				try { that.throwIfNotOk(); }
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Vector::operator=(Vector&&): Invalid vector", PROGRAM_POS, exc);
				}

				try { that.throwIfNotOk(); }
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Vector::operator=(Vector&&): Moving from messed up vector object", PROGRAM_POS, exc);
				}

				swap(*this, that);

				return *this;
			}

		// Access:
			const T& get(size_t index) const
			{
				try { throwIfNotOk(); }
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Vector::get(): Invalid vector", PROGRAM_POS, exc);
				}

				if (index >= size_)
				{
					throw MyException::Exception("Vector::get(): Access out of bounds", PROGRAM_POS);
				}

				if (ptrs_[index] == nullptr)
				{
					throw MyException::Exception("Vector::get(): Trying to access empty node", PROGRAM_POS);
				}

				return *ptrs_[index];
			}

			T& rip(size_t index)
			{
				try { throwIfNotOk(); }
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Vector::rip(): Invalid vector", PROGRAM_POS, exc);
				}

				if (index >= size_)
				{
					throw MyException::Exception("Vector::rip(): Access out of bounds", PROGRAM_POS);
				}

				if (ptrs_[index] == nullptr)
				{
					throw MyException::Exception("Vector::rip(): Trying to rip off somthing from empty node", PROGRAM_POS);
				}

				T& toReturn = *ptrs_[index];

				ptrs_[index] = nullptr;

				return toReturn;
			}

			Vector& set(size_t index, const T& val)
			{
				try { throwIfNotOk(); }
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Vector::set(): Invalid vector", PROGRAM_POS, exc);
				}

				if (index >= size_)
				{
					if (autoResize_) resize(index * 2);
					else throw MyException::Exception("Vector::set(): Access out of bounds", PROGRAM_POS);
				}

				if (ptrs_[index] == nullptr)
				{
					ptrs_[index] = new T(val);
				}
				else *ptrs_[index] = val;

				return *this;
			}

			// !!! Warning !!!
			// This may lead to bad (or even undefined) behavior if ptrs_[index] == nullptr
			// cause the object by ptrs_[index] is kinda invalid 
			T& operator[](size_t index)
			{
				try { throwIfNotOk(); }
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Vector::operator[]: Invalid vector", PROGRAM_POS, exc);
				}

				if (index >= size_)
				{
					if (autoResize_) resize(index * 2);
					else throw MyException::Exception("Vector::operator[]: Access out of bounds", PROGRAM_POS);
				}

				if (ptrs_[index] == nullptr)
				{
					if (autoResize_) ptrs_[index] = new T();
					else throw MyException::Exception("Vector::operator[]: Trying to access empty node", PROGRAM_POS);
				}

				return *ptrs_[index];
			}

		// Getters && setters:
			bool isAutoResizable() const
			{
				try { throwIfNotOk(); }
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Vector::isAutoResizable(): Invalid vector", PROGRAM_POS, exc);
				}

				return autoResize_;
			}

			Vector& setAutoResize(bool toSet = true)
			{
				try { throwIfNotOk(); }
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Vector::isAutoResizable(): Invalid vector", PROGRAM_POS, exc);
				}

				autoResize_ = toSet;

				return *this;
			}

			size_t size() const
			{
				try { throwIfNotOk(); }
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Vector::size(): Invalid vector", PROGRAM_POS, exc);
				}

				return size_;
			}

			bool emptyAt(size_t index) const
			{
				try { throwIfNotOk(); }
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Vector::emptyAt(): Invalid vector", PROGRAM_POS, exc);
				}

				return index >= size_ || ptrs_[index] == nullptr;
			}

		// Resize:
			Vector& resize(size_t newSize)
			{
				try { throwIfNotOk(); }
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Vector::resize(): Invalid vector", PROGRAM_POS, exc);
				}

				Vector newArr(newSize, true);

				for (size_t i = 0; i < newSize && i < size_; ++i)
				{
					newArr[i] = *ptrs_[i];
				}

				swap(*this, newArr);

				try { throwIfNotOk(); }
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Vector::resize(): Function broke vector state", PROGRAM_POS, exc);
				}

				return *this;
			}

			Vector& shrink()
			{
				try { throwIfNotOk(); }
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Vector::shrink(): Invalid vector", PROGRAM_POS, exc);
				}

				if (size_ == 0) return *this;

				size_t lastNode = size_ - 1;
				for (; 1 <= lastNode && lastNode < size_; --lastNode)
				{
					if (ptrs_[lastNode] != nullptr) break;
				}

				try { resize(lastNode + 1); }
				catch (MyException::Exception exc)
				{
					throw MyException::Exception("Vector::shrink(): Can't resize", PROGRAM_POS, exc);
				}

				return *this;
			}

		// Debugging:
			void throwIfNotOk() const
			{
				if (guard0_ == _detail::Guard::DESTRUCTED ||
					guard0_ == _detail::Guard::DESTRUCTED)
				{
					throw MyException::Exception("Vector::throwIfNotOk(): Vector is already destructed", PROGRAM_POS);
				}

				if (guard0_ != _detail::Guard::GUARD0 || guard1_ != _detail::Guard::GUARD1)
				{
					throw MyException::Exception("Vector::throwIfNotOk(): Vector's memory has been touched", PROGRAM_POS);
				}
			}

		// Swap:

			friend void swap(Vector& v0, Vector& v1)
			{
				std::swap(v0.guard0_    , v1.guard0_    );
				std::swap(v0.ptrs_      , v1.ptrs_      );
				std::swap(v0.size_      , v1.size_      );
				std::swap(v0.autoResize_, v1.autoResize_);
				std::swap(v0.guard1_    , v1.guard1_    );
			}

	private:
		// Variables:
			_detail::Guard guard0_;
			T** ptrs_;
			size_t size_;
			bool autoResize_;
			_detail::Guard guard1_;
	};
}

#undef PROGRAM_POS

#endif /*HEADER_GUARD_MY_VECTOR_HPP_INCLUDED*/