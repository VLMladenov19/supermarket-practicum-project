#pragma once

#include "User.h"

class Cashier : public User
{
public:
	Cashier(size_t id, const String& firstName,
		const String& lastName, const String& phoneNumber,
		unsigned short age, const String& pwd);

	UserRole getRole() const override;
	size_t getTransactionsCount() const;

	void incrementTransactionsCount();

private:
	size_t transactionsCount_;
	// Vector<Warnings> warning_;
};