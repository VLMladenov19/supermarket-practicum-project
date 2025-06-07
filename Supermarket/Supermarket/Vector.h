#pragma once

#include <iostream>
#include <algorithm>

template<typename T>
class Vector
{
public:
	Vector();
	explicit Vector(size_t size);
	Vector(const Vector<T>& other);
	Vector<T>& operator=(const Vector<T>& other);
	Vector(Vector<T>&& other) noexcept;
	Vector<T>& operator=(Vector<T>&& other) noexcept;
	~Vector();

	size_t size() const;
	size_t capacity() const;
	bool empty() const;

	Vector<T>& push_back(const T& val);
	Vector<T>& push_back(T&& val);
	Vector<T>& insert(size_t pos, const T& val);
	Vector<T>& insert(size_t pos, T&& val);

	Vector<T>& pop_back();
	Vector<T>& remove(size_t pos);

	void clear();

	friend void swap(Vector<T>& lhs, Vector<T>& rhs);

	T& operator[](size_t pos);
	const T& operator[](size_t pos) const;
	bool operator==(const Vector<T>& other) const;
	bool operator!=(const Vector<T>& other) const;

	friend std::ostream& operator<<(std::ostream& os, const Vector<T>& vec);

private:
	T* data_;
	size_t size_;
	size_t capacity_;

	void resize(size_t capacity);
	size_t getNextPowerOfTwo(size_t num) const;
	size_t allocateCapacity(size_t size) const;

	void copyFrom(const Vector<T>& other);
	void free();
};

template<typename T>
inline Vector<T>::Vector() : Vector(4)
{
}

template<typename T>
inline Vector<T>::Vector(size_t size) : data_(nullptr), size_(0)
{
	this->resize(this->allocateCapacity(size));
}

template<typename T>
inline Vector<T>::Vector(const Vector<T>& other)
{
	this->copyFrom(other);
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(const Vector<T>& other)
{
	if (this != &other)
	{
		this->free();
		this->copyFrom(other);
	}

	return *this;
}

template<typename T>
inline Vector<T>::Vector(Vector<T>&& other) noexcept
	: data_(other.data_), size_(other.size_), capacity_(other.capacity_)
{
	other.data_ = nullptr;
	other.size_ = 0;
	other.capacity_ = 0;
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept
{
	if (this != &other)
	{
		this->free();

		this->data_ = other.data_;
		size_ = other.size_;
		capacity_ = other.capacity_;

		other.data_ = nullptr;
		other.size_ = 0;
		other.capacity_ = 0;
	}
	return *this;
}

template<typename T>
inline Vector<T>::~Vector()
{
	this->free();
}

template<typename T>
inline size_t Vector<T>::size() const
{
	return this->size_;
}

template<typename T>
inline size_t Vector<T>::capacity() const
{
	return this->capacity_;
}

template<typename T>
inline bool Vector<T>::empty() const
{
	return this->size_ == 0;
}

template<typename T>
inline Vector<T>& Vector<T>::push_back(const T& val)
{
	if (this->capacity_ <= this->size_ + 1)
	{
		this->resize(this->allocateCapacity(this->size_ + 1));
	}

	this->data_[this->size_] = val;
	this->size_++;

	return *this;
}

template<typename T>
inline Vector<T>& Vector<T>::push_back(T&& val)
{
	if (this->capacity_ <= this->size_ + 1)
	{
		this->resize(this->allocateCapacity(this->size_ + 1));
	}

	this->data_[this->size_] = std::move(val);
	this->size_++;

	return *this;
}

template<typename T>
inline Vector<T>& Vector<T>::insert(size_t pos, const T& val)
{
	if (pos > this->size_)
	{
		return *this;
	}

	if (this->capacity_ <= this->size_ + 1)
	{
		this->resize(this->allocateCapacity(this->size_ + 1));
	}

	for (size_t i = this->size_; i > pos; i--)
	{
		this->data_[i] = this->data_[i - 1];
	}

	this->data_[pos] = val;
	this->size_++;

	return *this;
}

template<typename T>
inline Vector<T>& Vector<T>::insert(size_t pos, T&& val)
{
	if (pos > this->size_)
	{
		return *this;
	}

	if (this->capacity_ <= this->size_ + 1)
	{
		this->resize(this->allocateCapacity(this->size_ + 1));
	}

	for (size_t i = this->size_; i > pos; i--)
	{
		this->data_[i] = this->data_[i - 1];
	}

	this->data_[pos] = std::move(val);
	this->size_++;

	return *this;
}

template<typename T>
inline Vector<T>& Vector<T>::pop_back()
{
	if (!this->empty())
	{
		this->size_--;
	}

	return *this;
}

template<typename T>
inline Vector<T>& Vector<T>::remove(size_t pos)
{
	if (this->size_ <= pos)
	{
		return *this;
	}

	for (size_t i = pos; i < this->size_ - 1; i++)
	{
		this->data_[i] = this->data_[i + 1];
	}

	this->size_--;

	return *this;
}

template<typename T>
inline void Vector<T>::clear()
{
	this->size_ = 0;
}

template<typename T>
inline T& Vector<T>::operator[](size_t pos)
{
	return this->data_[pos];
}

template<typename T>
inline const T& Vector<T>::operator[](size_t pos) const
{
	return this->data_[pos];
}

template<typename T>
inline bool Vector<T>::operator==(const Vector<T>& other) const
{
	if (this->size_ != other.size_)
	{
		return false;
	}

	for (size_t i = 0; i < this->size_; i++)
	{
		if (this->data_[i] != other.data_[i])
		{
			return false;
		}
	}

	return true;
}

template<typename T>
inline bool Vector<T>::operator!=(const Vector<T>& other) const
{
	return !(*this == other);
}

template<typename T>
inline void Vector<T>::resize(size_t capacity)
{
	if (capacity <= this->capacity_)
		return;

	this->capacity_ = capacity;

	T* newData = new T[capacity];
	for (size_t i = 0; i < this->size_; i++)
	{
		newData[i] = std::move(this->data_[i]);
	}

	delete[] this->data_;
	this->data_ = newData;
}

template<typename T>
inline size_t Vector<T>::getNextPowerOfTwo(size_t num) const
{
	int power = 1;
	while (power <= num)
		power <<= 1;
	return power;
}

template<typename T>
inline size_t Vector<T>::allocateCapacity(size_t size) const
{
	return std::max(this->getNextPowerOfTwo(size), 8llu);
}

template<typename T>
inline void Vector<T>::copyFrom(const Vector<T>& other)
{
	this->capacity_ = other.capacity_;
	this->size_ = other.size_;
	this->data_ = new T[this->capacity_];
	for (size_t i = 0; i < other.size_; i++)
	{
		this->data_[i] = other.data_[i];
	}
}

template<typename T>
inline void Vector<T>::free()
{
	delete[] this->data_;
	this->data_ = nullptr;
	this->size_ = 0;
	this->capacity_ = 0;
}

template<typename T>
void swap(Vector<T>& lhs, Vector<T>& rhs)
{
	std::swap(lhs.data_, rhs.data_);
	std::swap(lhs.size_, rhs.size_);
	std::swap(lhs.capacity_, rhs.capacity_);
}

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const Vector<T>& vec)
{
	os << "[";
	for (size_t i = 0; i < vec.size_; ++i)
	{
		os << vec.data_[i];
		if (i != vec.size_ - 1) os << ", ";
	}
	os << "]";
	return os;
}