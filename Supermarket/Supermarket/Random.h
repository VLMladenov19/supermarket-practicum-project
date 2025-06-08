#pragma once

#include <cstdlib>

class Random
{
public:
	static char randomDigit()
	{
		return rand() % ('9' - '0' + 1) + '0';
	}
	static char randomUpper()
	{
		return rand() % ('Z' - 'A' + 1) + 'A';
	}
	static char randomLower()
	{
		return rand() % ('z' - 'a' + 1) + 'a';
	}
};