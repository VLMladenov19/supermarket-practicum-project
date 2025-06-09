#pragma once

#include <fstream>

#include "String.h"

enum class UserRole
{
	None,
	Cashier,
	Manager
};

String roleToStr(UserRole role);
UserRole strToRole(const String& str);

class User
{
public:
	User();
	User(size_t id, const String& firstName,
		const String& lastName, const String& phoneNumber,
		unsigned short age, const String& pwd);
	User(const User& other);
	User& operator=(const User& other);
	User(User&& other) noexcept;
	User& operator=(User&& other) noexcept;
	virtual ~User() = default;

	virtual UserRole getRole() const = 0;
	size_t getId() const;
	const String& getFirstName() const;
	const String& getLastName() const;
	const String& getPhoneNumber() const;
	unsigned short getAge() const;

	void setFirstName(const String& firstName);
	void setLastName(const String& lastName);
	void setPhoneNumber(const String& phoneNumber);
	void setAge(unsigned short age);

	bool checkPassword(const String& pwd) const;
	void changePassword(const String& currPwd, const String& newPwd);

	virtual std::ofstream& serialize(std::ofstream& os) const;
	virtual std::ifstream& deserialize(std::ifstream& is);

	virtual String toString() const;

protected:
	size_t id_;
	String firstName_;
	String lastName_;
	String phoneNumber_;
	unsigned short age_;
	String password_;

	void setId(size_t id);
	void setPassword(const String& pwd);

	String readString(std::ifstream& is) const;

	void copyFrom(const User& other);
};