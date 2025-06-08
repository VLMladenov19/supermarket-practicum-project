#include "Category.h"
#include "ProductConstants.h"

Category::Category()
	: id_(0), name_(), description_()
{
}

Category::Category(size_t id, String name, String description)
	: id_(id), name_(name), description_(description)
{
}

size_t Category::getId() const
{
	return this->id_;
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

std::ofstream& Category::serialize(std::ofstream& os) const  
{  
	os << String::toString(this->id_) << ":"
		<< this->name_ << ":"
		<< this->description_ << '\n';
    return os;  
}

std::ifstream& Category::deserialize(std::ifstream& is)
{
	using namespace ProductConstants::LoadCategory;
	String line;
	getline(is, line);

	Vector<String> tokens = line.split(':');
	this->setId(tokens[ID_INDEX].toNumber());
	this->setName(tokens[NAME_INDEX]);
	this->setDescription(tokens[DESCRIPTION_INDEX]);

	return is;
}

String Category::toString() const
{
	String result;

	result.push_back('<');
	result += String::toString(this->id_) + "> ";
	result += this->name_ + ": ";
	result += this->description_;

	return result;
}

void Category::setId(size_t id)
{
	this->id_ = id;
}
