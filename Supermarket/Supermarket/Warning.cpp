#include "Warning.h"

Warning::Warning()
	: senderId_(0), points_(0)
{
}

Warning::Warning(size_t senderId, size_t points)
	: senderId_(senderId), points_(points)
{
}

size_t Warning::getSenderId() const
{
	return this->senderId_;
}

size_t Warning::getPoints() const
{
	return this->points_;
}

std::ofstream& Warning::serialize(std::ofstream& os) const
{
	os.write((const char*)&this->senderId_, sizeof(this->senderId_));
	os.write((const char*)&this->points_, sizeof(this->points_));
	return os;
}

std::ifstream& Warning::deserialize(std::ifstream& is)
{
	is.read((char*)&this->senderId_, sizeof(this->senderId_));
	is.read((char*)&this->points_, sizeof(this->points_));
	return is;
}

void Warning::setSenderId(size_t senderId)
{
	this->senderId_ = senderId;
}

void Warning::setPoints(size_t points)
{
	this->points_ = points;
}
