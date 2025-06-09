#pragma once

#include "User.h"
#include "Manager.h"
#include "Warning.h"

class Cashier : public User
{
public:
	Cashier();
	Cashier(size_t id, const String& firstName,
		const String& lastName, const String& phoneNumber,
		unsigned short age, const String& pwd);
	Cashier(size_t id, const String& firstName,
		const String& lastName, const String& phoneNumber,
		unsigned short age, const String& pwd, 
		size_t transactionsCount, const Vector<Warning> warnings);

	UserRole getRole() const override;
	size_t getTransactionsCount() const;
	const Vector<Warning> getWarnings() const;

	Manager* getAsManager() const;

	void addWarning(const Warning& warning);
	size_t getTotalWarningPoints() const;

	void incrementTransactionsCount();

	std::ofstream& serialize(std::ofstream& os) const override;
	std::ifstream& deserialize(std::ifstream& is) override;

	String toString() const override;

private:
	size_t transactionsCount_;
	Vector<Warning> warnings_;
};