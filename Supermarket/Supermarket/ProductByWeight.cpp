#include "ProductByWeight.h"

ProductByWeight::ProductByWeight(const String& name, 
	const Category& category, 
	size_t priceMinor, double weight)
	: Product(name, category, priceMinor), weight_(weight)
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
