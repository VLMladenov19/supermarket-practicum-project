#pragma once

#include "User.h"
#include "Manager.h"

class Cashier : public User
{
public:
	Cashier();
	Cashier(size_t id, const String& firstName,
		const String& lastName, const String& phoneNumber,
		unsigned short age, const String& pwd);

	UserRole getRole() const override;
	size_t getTransactionsCount() const;

	Manager* getAsManager() const;

	void incrementTransactionsCount();

	std::ofstream& serialize(std::ofstream& os) const override;
	std::ifstream& deserialize(std::ifstream& is) override;

private:
	size_t transactionsCount_;
	// Vector<Warnings> warning_;
};