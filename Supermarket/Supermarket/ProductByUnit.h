#pragma once

#include "Product.h"
#include "Response.h"

class ProductByUnit : public Product
{
public:
	ProductByUnit(const String& name, const Category& category,
		size_t priceMinor, size_t quantity);

	ProductType getType() const override;

	size_t getQuantity() const;

	void setQuantity(size_t quantity);

	void increaseQuantity(size_t amount);
	Response decreaseQuantity(size_t amount);

private:
	size_t quantity_;
};