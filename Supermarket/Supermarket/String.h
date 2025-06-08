#pragma once

#include <iostream>
#include "Vector.h"

class String
{
public:
	static const size_t npos = -1;

	String();
	String(const String& other);
	String(const char* data);
	String& operator=(const String& other);
	String& operator=(const char* data);
	String(String&& other) noexcept;
	String& operator=(String&& other) noexcept;

	size_t size() const;
	size_t capacity() const;
	bool empty() const;

	String toLower() const;
	String toUpper() const;
	size_t toNumber() const;
	static String toString(size_t num);
	static String toString(double num);

	String& append(const String& other);
	String& append(const char* data);
	void push_back(char c);
	void pop_back();

	String& insert(size_t pos, const String& other);
	String& insert(size_t pos, const char* data);
	String& insert(size_t pos, char ch);

	String& erase(size_t pos, size_t len);
	void clear();

	String& replace(size_t pos, size_t len, const String& other);
	String& replace(size_t pos, size_t len, const char* data);

	size_t find(const String& str, size_t pos = 0) const;
	size_t find(const char* str, size_t pos = 0) const;

	String substring(size_t pos, size_t len = String::npos) const;
	Vector<String> split(char delimeter = ' ') const;

	String& reverse();

	friend void swap(String& lhs, String& rhs);

	String& operator+=(const String& other);
	friend String operator+(const String& lhs, const String& rhs);
	String& operator+=(char other);
	friend String operator+(const String& lhs, char rhs);
	friend String operator+(char lhs, const String& rhs);
	char& operator[](size_t pos);
	const char& operator[](size_t pos) const;

	operator const char* () const;

	bool operator==(const String& other) const;
	bool operator==(const char* data) const;
	bool operator!=(const String& other) const;
	bool operator!=(const char* data) const;

	~String();

private:
	char* data_;
	size_t size_;
	size_t capacity_;

	void resize(size_t capacity);
	size_t getNextPowerOfTwo(size_t num) const;
	size_t allocateCapacity(size_t size) const;

	void copyFrom(const String& other);
	void copyFrom(const char* data);
	void free();
};

std::istream& getline(std::istream& is, String& str, char delim);
std::istream& getline(std::istream& is, String& str);