#include "Product.h"
#include "ProductConstants.h"

Product::Product()
	: id_(0), name_(), categoryId_(0), priceMinor_(0)
{
}

Product::Product(size_t id, const String& name,
	size_t categoryId, size_t priceMinor)
	: id_(id), name_(name), categoryId_(categoryId), priceMinor_(priceMinor)
{
}

size_t Product::getId() const
{
	return this->id_;
}

const String& Product::getName() const
{
	return this->name_;
}

size_t Product::getCategoryId() const
{
	return this->categoryId_;
}

size_t Product::getPriceMinor() const
{
	return this->priceMinor_;
}

void Product::setName(const String& name)
{
	this->name_ = name;
}

void Product::setCategoryId(size_t categoryId)
{
	this->categoryId_ = categoryId;
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
	case ProductType::ByUnit: return "ByUnit";
	case ProductType::ByWeight: return "ByWeight";
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

std::ofstream& Product::serialize(std::ofstream& os) const
{
	os << typeToStr(this->getType()) << ":"
		<< this->id_ << ":"
		<< this->name_ << ":"
		<< this->categoryId_ << ":"
		<< this->priceMinor_ << ":";

	return os;
}

Response Product::deserializeFromTokens(const Vector<String>& tokens)
{
	using namespace ProductConstants::LoadProduct;
	this->setId(tokens[ID_INDEX].toNumber());
	this->setName(tokens[NAME_INDEX]);
	this->setCategoryId(tokens[CATEGORY_ID_INDEX].toNumber());
	this->setPriceMinor(tokens[PRICE_MINOR_INDEX].toNumber());

	return Response(true, "Base values deserialized.");
}

String Product::toString() const
{
	String productInfo;

	productInfo.push_back('<');
	productInfo += String::toString(this->getId());
	productInfo += "> ";
	productInfo += typeToStr(this->getType()) + ": ";
	productInfo += name_ + " ";
	productInfo += categoryId_ + " ";
	productInfo += String::toString(priceMinor_);

	return productInfo;
}

void Product::setId(size_t id)
{
	this->id_ = id;
}
