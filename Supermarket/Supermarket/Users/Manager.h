#pragma once

#include "../Users/User.h"

class Manager : public User
{
public:
	Manager();
	Manager(size_t id, const String& firstName,
		const String& lastName, const String& phoneNumber,
		unsigned short age, const String& pwd);

	UserRole getRole() const override;
	const String& getSpecialCode() const;

	bool compareSpecialCode(const String& specialCode) const;

	std::ifstream& deserialize(std::ifstream& is) override;

private:
	String specialCode_;

	String generateSpecialCode() const;
	void saveSpecialCode() const;
};