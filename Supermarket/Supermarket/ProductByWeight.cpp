#include "ProductByWeight.h"
#include "ProductConstants.h"

ProductByWeight::ProductByWeight()
	: Product(), weight_(0)
{
}

ProductByWeight::ProductByWeight(size_t id, const String& name,
	size_t categoryId, 
	size_t priceMinor, double weight)
	: Product(id, name, categoryId, priceMinor), weight_(weight)
{
}

ProductType ProductByWeight::getType() const
{
	return ProductType::ByWeight;
}

double ProductByWeight::getWeight() const
{
	return this->weight_;
}

void ProductByWeight::setWeight(double weight)
{
	this->weight_ = weight;
}

void ProductByWeight::increaseWeight(double amount)
{
	this->weight_ += amount;
}

Response ProductByWeight::decreaseWeight(double amount)
{
	if (this->weight_ < amount)
	{
		return Response(false, "Not enough weight in stock.");
	}

	this->weight_ -= amount;
	return Response(true, "Weight decreased successfully.");
}

std::ofstream& ProductByWeight::serialize(std::ofstream& os) const
{
	Product::serialize(os);

	os << (size_t)(this->weight_ * 100) << '\n';

	return os;
}

std::ifstream& ProductByWeight::deserialize(std::ifstream& is)
{
	using namespace ProductConstants::LoadProduct;
	String line;
	if (!getline(is, line))
	{
		return is;
	}

	Vector<String> tokens = line.split(':');
	if (tokens.size() != TOKENS_SIZE)
	{
		return is;
	}

	this->deserializeFromTokens(tokens);
	size_t weightInGrams = tokens[WEIGHT_INDEX].toNumber();
	this->setWeight(weightInGrams / 100.0);

	return is;
}

Response ProductByWeight::deserializeFromTokens(const Vector<String>& tokens)
{
	Product::deserializeFromTokens(tokens);

	this->setWeight(
		tokens[ProductConstants::LoadProduct::QUANTITY_INDEX]
		.toNumber() / 100.0);

	return Response(true, "Product values deserialized.");
}

String ProductByWeight::toString() const
{
	String productInfo = Product::toString();

	size_t weightInGrams = this->weight_ * 100;
	productInfo += " " + String::toString(weightInGrams / 100);
	productInfo.push_back('.');
	productInfo += String::toString(weightInGrams % 100);

	return productInfo;
}
