#include "ProductByUnit.h"
#include "ProductConstants.h"

ProductByUnit::ProductByUnit()
	: Product(), quantity_(0)
{
}

ProductByUnit::ProductByUnit(size_t id, const String& name,
	size_t categoryId, 
	size_t priceMinor, size_t quantity)
	: Product(id, name, categoryId, priceMinor), quantity_(quantity)
{
}

ProductType ProductByUnit::getType() const
{
	return ProductType::ByUnit;
}

size_t ProductByUnit::getQuantity() const
{
	return this->quantity_;
}

void ProductByUnit::setQuantity(size_t quantity)
{
	this->quantity_ = quantity;
}

void ProductByUnit::increaseQuantity(size_t amount)
{
	this->quantity_ += amount;
}

Response ProductByUnit::decreaseQuantity(size_t amount)
{
	if (this->quantity_ < amount)
	{
		return Response(false, "Not enough quantity in stock.");
	}

	this->quantity_ -= amount;
	return Response(true, "Quantity decreased successfully.");
}

std::ofstream& ProductByUnit::serialize(std::ofstream& os) const
{
	Product::serialize(os);

	os << this->quantity_ << '\n';

	return os;
}

std::ifstream& ProductByUnit::deserialize(std::ifstream& is)
{
	using namespace ProductConstants::DeserializeProduct;
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
	this->setQuantity(tokens[QUANTITY_INDEX].toNumber());

	return is;
}

Response ProductByUnit::deserializeFromTokens(const Vector<String>& tokens)
{
	Product::deserializeFromTokens(tokens);

	this->setQuantity(
		tokens[ProductConstants::DeserializeProduct::QUANTITY_INDEX]
		.toNumber());

	return Response(true, "Product values deserialized.");
}

String ProductByUnit::toString() const
{
	String productInfo = Product::toString();

	productInfo += " " + String::toString(this->quantity_);

	return productInfo;
}
