#include "Product.h"

Product::Product(const String& name,
	const Category& category, size_t priceMinor)
	: name_(name), category_(category), priceMinor_(priceMinor)
{
}

const String& Product::getName() const
{
	return this->name_;
}

const Category& Product::getCategory() const
{
	return this->category_;
}

size_t Product::getPriceMinor() const
{
	return this->priceMinor_;
}

void Product::setName(const String& name)
{
	this->name_ = name;
}

void Product::setCategory(const Category& category)
{
	this->category_ = category;
}

void Product::setPriceMinor(size_t priceMinor)
{
	this->priceMinor_ = priceMinor;
}

String Product::getPriceString() const
{
	size_t beforeFloat = this->priceMinor_ / 100;
	size_t afterFloat = this->priceMinor_ % 100;

	String result = String::toString(beforeFloat);
	result.push_back('.');
	if (afterFloat < 10)
	{
		result.push_back('0');
	}
	result += String::toString(afterFloat);

	return result;
}

String typeToStr(ProductType type)
{
	switch (type)
	{
	case ProductType::ByUnit: return "By Unit";
	case ProductType::ByWeight: return "By Weight";
	default: return "None";
	}
}

ProductType strToType(const String& str)
{
	String lower = str.toLower();

	if (lower == "byunit" || lower == "unit") 
		return ProductType::ByUnit;
	if (lower == "byweight" || lower == "weight") 
		return ProductType::ByWeight;

	return ProductType::None;
}
