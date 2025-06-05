#pragma once

#include "String.h"

class Category
{
public:
	Category(String name, String description);
	
	const String& getName() const;
	const String& getDescription() const;

	void setName(const String& name);
	void setDescription(const String& description);

private:
	String name_;
	String description_;
};