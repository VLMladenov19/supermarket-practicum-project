#pragma once

#include "Product.h"
#include "Response.h"

class ProductByWeight : public Product
{
public:
	ProductByWeight(const String& name, const Category& category,
		size_t priceMinor, double weight);

	ProductType getType() const override;

	double getWeight() const;

	void setWeight(double weight);

	void increaseWeight(double amount);
	Response decreaseWeight(double amount);

private:
	double weight_;
};
