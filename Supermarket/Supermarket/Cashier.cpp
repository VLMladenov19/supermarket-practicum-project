#include "Cashier.h"

Cashier::Cashier(size_t id, const String& firstName, 
	const String& lastName, const String& phoneNumber, 
	unsigned short age, const String& pwd)
	: User(id, firstName, lastName, phoneNumber, age, pwd)
{
	this->transactionsCount_ = 0;
}

UserRole Cashier::getRole() const
{
	return UserRole::Cashier;
}

size_t Cashier::getTransactionsCount() const
{
	return this->transactionsCount_;
}

void Cashier::incrementTransactionsCount()
{
	this->transactionsCount_++;
}
