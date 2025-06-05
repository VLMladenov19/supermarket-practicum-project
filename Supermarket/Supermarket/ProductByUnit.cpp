#include "ProductByUnit.h"

ProductByUnit::ProductByUnit(const String& name, 
	const Category& category, 
	size_t priceMinor, size_t quantity)
	: Product(name, category, priceMinor), quantity_(quantity)
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
