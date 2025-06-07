#pragma once

#include "User.h"

class Manager : public User
{
public:
	Manager();
	Manager(size_t id, const String& firstName,
		const String& lastName, const String& phoneNumber,
		unsigned short age, const String& pwd);

	UserRole getRole() const override;

	bool compareSpecialCode(const String& specialCode) const;

	std::ifstream& deserialize(std::ifstream& is) override;

private:
	String specialCode_;

	static char randomDigit();
	static char randomUpper();
	static char randomLower();

	String generateSpecialCode() const;
	void saveSpecialCode() const;
};