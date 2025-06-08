#pragma once

#include <fstream>
#include "String.h"

class Category
{
public:
	Category();
	Category(size_t id, String name, String description);
	
	size_t getId() const;
	const String& getName() const;
	const String& getDescription() const;

	void setName(const String& name);
	void setDescription(const String& description);

	virtual std::ofstream& serialize(std::ofstream& os) const;
	virtual std::ifstream& deserialize(std::ifstream& is);

	String toString() const;

private:
	size_t id_;
	String name_;
	String description_;

	void setId(size_t id);
};