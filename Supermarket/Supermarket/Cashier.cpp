#include "Cashier.h"

Cashier::Cashier()
	: User(), transactionsCount_(0)
{
}

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

Manager* Cashier::getAsManager() const
{
	return new Manager(
		this->id_,
		this->firstName_,
		this->lastName_,
		this->phoneNumber_,
		this->age_,
		this->password_
	);
}

void Cashier::incrementTransactionsCount()
{
	this->transactionsCount_++;
}

std::ofstream& Cashier::serialize(std::ofstream& os) const
{
	User::serialize(os);

	os.write((const char*)&this->transactionsCount_, 
		sizeof(this->transactionsCount_));
	return os;
}

std::ifstream& Cashier::deserialize(std::ifstream& is)
{
	User::deserialize(is);

	is.read((char*)&this->transactionsCount_,
		sizeof(this->transactionsCount_));
	return is;
}
