#include "Manager.h"

#include <fstream>

Manager::Manager(size_t id, const String& firstName,
	const String& lastName, const String& phoneNumber,
	unsigned short age, const String& pwd)
	: User(id, firstName, lastName, phoneNumber, age, pwd)
{
	this->specialCode_ = this->generateSpecialCode();
	this->saveSpecialCode();
}

UserRole Manager::getRole() const
{
	return UserRole::Manager;
}

bool Manager::compareSpecialCode(const String& specialCode) const
{
	return this->specialCode_ == specialCode;
}

char Manager::randomDigit()
{
	return rand() % ('9' - '0' + 1) + '0';
}

char Manager::randomUpper()
{
	return rand() % ('Z' - 'A' + 1) + 'A';
}

char Manager::randomLower()
{
	return rand() % ('z' - 'a' + 1) + 'a';
}

String Manager::generateSpecialCode() const
{
	String specialCode;

	specialCode.push_back(Manager::randomDigit());
	specialCode.push_back(Manager::randomUpper());
	specialCode.push_back(Manager::randomUpper());
	specialCode.push_back(Manager::randomDigit());
	specialCode.push_back(Manager::randomDigit());
	specialCode.push_back(Manager::randomDigit());
	specialCode.push_back(Manager::randomLower());

	return specialCode;
}

void Manager::saveSpecialCode() const
{
	String fileName = String::toString(this->id_) + "_special_code.txt";
	std::ofstream os(fileName, std::ios::out);

	if (!os.is_open())
	{
		return;
	}

	os << this->specialCode_ << '\n';

	os.close();
}
