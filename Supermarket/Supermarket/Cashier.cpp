#include "Cashier.h"

Cashier::Cashier()
	: User(), transactionsCount_(0), warnings_()
{
}

Cashier::Cashier(size_t id, const String& firstName,
	const String& lastName, const String& phoneNumber, 
	unsigned short age, const String& pwd)
	: User(id, firstName, lastName, phoneNumber, age, pwd), 
	transactionsCount_(0), warnings_()
{
}

Cashier::Cashier(size_t id, const String& firstName, 
	const String& lastName, const String& phoneNumber, 
	unsigned short age, const String& pwd, 
	size_t transactionsCount, const Vector<Warning> warnings)
	: User(id, firstName, lastName, phoneNumber, age, pwd),
	transactionsCount_(transactionsCount), warnings_(warnings)
{
}

UserRole Cashier::getRole() const
{
	return UserRole::Cashier;
}

size_t Cashier::getTransactionsCount() const
{
	return this->transactionsCount_;
}

const Vector<Warning> Cashier::getWarnings() const
{
	return this->warnings_;
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

void Cashier::addWarning(const Warning& warning)
{
	this->warnings_.push_back(warning);
}

size_t Cashier::getTotalWarningPoints() const
{
	size_t points = 0;
	size_t warningsCount = this->warnings_.size();
	for (size_t i = 0; i < warningsCount; i++)
	{
		points += this->warnings_[i].getPoints();
	}
	return points;
}

void Cashier::incrementTransactionsCount()
{
	this->transactionsCount_++;
	if (!(transactionsCount_ % 3))
	{
		this->warnings_.remove(0);
	}
}

std::ofstream& Cashier::serialize(std::ofstream& os) const
{
	User::serialize(os);

	os.write((const char*)&this->transactionsCount_, 
		sizeof(this->transactionsCount_));

	size_t warningsCount = this->warnings_.size();
	os.write((const char*)&warningsCount, sizeof(warningsCount));
	for (size_t i = 0; i < warningsCount; ++i)
	{
		this->warnings_[i].serialize(os);
	}

	return os;
}

std::ifstream& Cashier::deserialize(std::ifstream& is)
{
	User::deserialize(is);

	is.read((char*)&this->transactionsCount_,
		sizeof(this->transactionsCount_));

	size_t warningCount = 0;
	is.read((char*)&warningCount, sizeof(warningCount));

	this->warnings_.clear();
	for (size_t i = 0; i < warningCount; ++i)
	{
		Warning warning;
		warning.deserialize(is);
		this->warnings_.push_back(warning);
	}

	return is;
}

String Cashier::toString() const
{
	String userInfo = User::toString();

	userInfo += ", Total Warnings: ";
	userInfo += String::toString(this->warnings_.size()) + " (";
	userInfo += String::toString(this->getTotalWarningPoints()) + " points)";

	return userInfo;
}
