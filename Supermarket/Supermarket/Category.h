#pragma once

#include "String.h"

class Category
{
public:
	Category(String name, String description);
	
	size_t getId() const;
	const String& getName() const;
	const String& getDescription() const;

	void setName(const String& name);
	void setDescription(const String& description);

private:
	size_t id_;
	String name_;
	String description_;

	void setId(size_t id);
};