#include "Category.h"

Category::Category(String name, String description)
	: name_(name), description_(description)
{
}

const String& Category::getName() const
{
	return this->name_;
}

const String& Category::getDescription() const
{
	return this->description_;
}

void Category::setName(const String& name)
{
	this->name_ = name;
}

void Category::setDescription(const String& description)
{
	this->description_ = description;
}
