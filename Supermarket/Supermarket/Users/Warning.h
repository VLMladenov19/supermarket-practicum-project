#pragma once

#include <fstream>

#include "../Utils/String.h"

class Warning
{
public:
	Warning();
	Warning(size_t senderId, size_t points);

	size_t getSenderId() const;
	size_t getPoints() const;

	std::ofstream& serialize(std::ofstream& os) const;
	std::ifstream& deserialize(std::ifstream& is);

private:
	size_t senderId_;
	size_t points_;

	void setSenderId(size_t senderId);
	void setPoints(size_t points);
};