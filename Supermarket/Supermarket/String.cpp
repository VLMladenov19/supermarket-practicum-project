#pragma warning (push)
#pragma warning (disable : 4996) // using standart C funtion (strcpy)

#include "String.h"

String::String()
{
	this->capacity_ = 32;
	this->size_ = 0;
	this->data_ = new char[this->capacity_ + 1]();
}

String::String(const String& other)
{
	this->copyFrom(other);
}

String::String(const char* data)
{
	if (!data)
	{
		*this = String();
		return;
	}

	size_t dataSize = strlen(data);
	this->capacity_ = this->getNextPowerOfTwo(dataSize);
	this->size_ = dataSize;
	this->data_ = new char[this->capacity_ + 1];
	strcpy(this->data_, data);
}

String& String::operator=(const String& other)
{
	if (this != &other)
	{
		this->free();
		this->copyFrom(other);
	}
	return *this;
}

String& String::operator=(const char* data)
{
	if (*this != data && data)
	{
		this->free();
		this->copyFrom(data);
	}
	return *this;
}

String::String(String&& other) noexcept
	: data_(other.data_), size_(other.size_), capacity_(other.capacity_)
{
	other.data_ = nullptr;
	other.size_ = 0;
	other.capacity_ = 0;
}

String& String::operator=(String&& other) noexcept
{
	if (this != &other)
	{
		this->free();

		this->data_ = other.data_;
		this->size_ = other.size_;
		this->capacity_ = other.capacity_;

		other.data_ = nullptr;
		other.size_ = 0;
		other.capacity_ = 0;
	}
	return *this;
}

String::~String()
{
	this->free();
}

size_t String::size() const
{
	return this->size_;
}

size_t String::capacity() const
{
	return this->capacity_;
}

bool String::empty() const
{
	return (this->size_ == 0);
}

String String::toLower() const
{
	String result(*this);

	for (size_t i = 0; i < result.size_; i++)
	{
		if ('A' <= result.data_[i] && result.data_[i] <= 'Z')
		{
			result.data_[i] = result.data_[i] - 'A' + 'a';
		}
	}

	return result;
}

String String::toUpper() const
{
	String result(*this);

	for (size_t i = 0; i < result.size_; i++)
	{
		if ('a' <= result.data_[i] && result.data_[i] <= 'z')
		{
			result.data_[i] = result.data_[i] - 'a' + 'A';
		}
	}

	return result;
}

size_t String::toNumber() const
{
	if (this->size_ == 0)
	{
		return String::npos;
	}

	size_t result = 0;
	bool changed = false;
	for (size_t i = 0; i < this->size_; i++)
	{
		if (this->data_[i] < '0' || this->data_[i] > '9')
		{
			if (!changed)
			{
				return String::npos;
			}
			break;
		}
		result = result * 10 + (this->data_[i] - '0');
		changed = true;
	}

	return result;
}

String String::toString(size_t num)
{
	if (num == 0)
	{
		return String("0");
	}

	String result;
	while (num)
	{
		result.push_back(num % 10 + '0');
		num /= 10;
	}

	result.reverse();
	return result;
}

String& String::append(const String& other)
{
	*this += other;
	return *this;
}

String& String::append(const char* data)
{
	if (data)
	{
		return *this;
	}

	*this += data;
	return *this;
}

void String::push_back(char c)
{
	if (this->capacity_ <= this->size_ + 1)
	{
		this->resize(this->allocateCapacity(this->size_ + 1));
	}

	this->data_[this->size_] = c;
	this->size_++;
	this->data_[this->size_] = '\0';
}

void String::pop_back()
{
	if (this->size_ == 0)
	{
		return;
	}

	this->size_--;
	this->data_[this->size_] = '\0';
}

String& String::insert(size_t pos, const String& other)
{
	if (pos > this->size_)
	{
		return *this;
	}

	if (this->capacity_ <= this->size_ + other.size_)
	{
		this->resize(this->allocateCapacity(this->size_ + other.size_));
	}

	for (size_t i = this->size_; i > pos; i--)
	{
		this->data_[i + other.size_ - 1] = this->data_[i - 1];
	}

	for (size_t i = 0; i < other.size_; i++)
	{
		this->data_[i + pos] = other.data_[i];
	}

	this->size_ += other.size_;
	this->data_[this->size_] = '\0';

	return *this;
}

String& String::insert(size_t pos, const char* data)
{
	if (pos > this->size_ || !data)
	{
		return *this;
	}

	size_t strLen = strlen(data);
	size_t newSize = this->size_ + strLen;
	if (this->capacity_ <= newSize)
	{
		this->resize(this->allocateCapacity(newSize));
	}

	for (size_t i = this->size_; i > pos; i--)
	{
		this->data_[i + strLen - 1] = this->data_[i - 1];
	}

	for (size_t i = 0; i < strLen; i++)
	{
		this->data_[i + pos] = data[i];
	}

	this->size_ = newSize;
	this->data_[this->size_] = '\0';

	return *this;
}

String& String::insert(size_t pos, char ch)
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

	this->data_[pos] = ch;
	this->size_++;
	this->data_[this->size_] = '\0';

	return *this;
}

String& String::erase(size_t pos, size_t len)
{
	if (this->size_ <= pos)
	{
		return *this;
	}

	if (pos + len > this->size_)
	{
		len = this->size_ - pos;
	}

	for (size_t i = pos; i + len < this->size_; ++i)
	{
		this->data_[i] = this->data_[i + len];
	}

	this->size_ -= len;
	this->data_[this->size_] = '\0';

	return *this;
}

void String::clear()
{
	this->size_ = 0;
	this->data_[0] = '\0';
}

String& String::replace(size_t pos, size_t len, const String& other)
{
	this->erase(pos, len);
	this->insert(pos, other);
	return *this;
}

String& String::replace(size_t pos, size_t len, const char* data)
{
	return this->replace(pos, len, String(data));
}

size_t String::find(const String& str, size_t pos) const
{
	if (str.size_ == 0 || this->size_ <= pos)
		return String::npos;

	for (size_t i = pos; i <= this->size_ - str.size_; i++)
	{
		size_t j = 0;

		while (j < str.size_ && this->data_[i + j] == str.data_[j])
		{
			j++;
		}

		if (j == str.size_)
			return i;
	}

	return String::npos;
}

size_t String::find(const char* str, size_t pos) const
{
	if (!str)
	{
		return String::npos;
	}

	return find(String(str), pos);
}

String String::substring(size_t pos, size_t len) const
{
	if (pos >= this->size_ || this->empty())
	{
		return String();
	}

	if (pos + len > this->size_ || len == String::npos)
	{
		len = this->size_ - pos;
	}

	String result;
	for (size_t i = 0; i < len; i++)
	{
		result.push_back(this->data_[pos + i]);
	}
	return result;
}

Vector<String> String::split(char delimeter) const
{
	Vector<String> result;
	size_t start = 0;
	for (size_t i = 0; i < this->size_; i++)
	{
		if (this->data_[i] == delimeter ||
			this->data_[i] == '\0')
		{
			if (i > start)
			{
				result.push_back(this->substring(start, i - start));
			}
			start = i + 1;
		}
	}
	if (start < this->size_)
	{
		result.push_back(this->substring(start));
	}

	return result;
}

String& String::reverse()
{
	for (size_t i = 0; i < this->size_ / 2; i++)
	{
		char temp = this->data_[i];
		this->data_[i] = this->data_[this->size_ - i - 1];
		this->data_[this->size_ - i - 1] = temp;
	}
	return *this;
}

void String::resize(size_t capacity)
{
	this->capacity_ = capacity;

	char* newData = new char[capacity + 1];
	strcpy(newData, this->data_);

	delete[] this->data_;
	this->data_ = newData;
}

size_t String::getNextPowerOfTwo(size_t num) const
{
	int power = 1;
	while (power <= num)
		power <<= 1;
	return power;
}

size_t String::allocateCapacity(size_t size) const
{
	return std::max(this->getNextPowerOfTwo(size), 8llu);
}

void String::copyFrom(const String& other)
{
	// delete[] this->data_;
	this->capacity_ = other.capacity_;
	this->size_ = other.size_;
	this->data_ = new char[this->capacity_ + 1];
	strcpy(this->data_, other.data_);
}

void String::copyFrom(const char* data)
{
	if (!data)
	{
		this->copyFrom(String());
		return;
	}

	delete[] this->data_;
	size_t dataLen = strlen(data);
	this->capacity_ = this->getNextPowerOfTwo(dataLen);
	this->size_ = dataLen;
	this->data_ = new char[this->capacity_ + 1];
	strcpy(this->data_, data);
}

void String::free()
{
	delete[] this->data_;
	this->data_ = nullptr;
}

String& String::operator+=(const String& other)
{
	if (this->capacity_ <= this->size_ + other.size_)
	{
		this->resize(this->allocateCapacity(this->size_ + other.size_));
	}

	strcat(this->data_, other.data_);
	this->size_ += other.size_;

	return *this;
}

String& String::operator+=(char other)
{
	this->push_back(other);

	return *this;
}

char& String::operator[](size_t pos)
{
	return this->data_[pos];
}

const char& String::operator[](size_t pos) const
{
	return this->data_[pos];
}

String::operator const char* () const
{
	return this->data_;
}

bool String::operator==(const String& other) const
{
	return strcmp(this->data_, other.data_) == 0;
}

bool String::operator==(const char* data) const
{
	if (!data)
	{
		return false;
	}

	return strcmp(this->data_, data) == 0;
}

bool String::operator!=(const String& other) const
{
	return !(*this == other);
}

bool String::operator!=(const char* data) const
{
	return !(*this == data);
}

std::istream& getline(std::istream& is, String& str, char delim)
{
	str.clear();

	char ch;
	while (is.get(ch))
	{
		if (ch == delim)
			break;
		str.push_back(ch);
	}
	str.push_back('\0');

	return is;
}

std::istream& getline(std::istream& is, String& str)
{
	return getline(is, str, '\n');
}

void swap(String& lhs, String& rhs)
{
	String temp = lhs;
	lhs = rhs;
	rhs = temp;
}

String operator+(const String& lhs, const String& rhs)
{
	String result(lhs);
	result += rhs;
	return result;
}

String operator+(const String& lhs, char rhs)
{
	String result(lhs);
	result.push_back(rhs);
	return result;
}

String operator+(char lhs, const String& rhs)
{
	String result;
	result.push_back(lhs);
	result += rhs;
	return result;
}

#pragma warning (pop)