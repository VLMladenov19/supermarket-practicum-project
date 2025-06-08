#pragma once

#include "Product.h"
#include "Response.h"

class ProductByUnit : public Product
{
public:
	ProductByUnit();
	ProductByUnit(size_t id, const String& name, 
		size_t categoryId, size_t priceMinor, size_t quantity);

	ProductType getType() const override;

	size_t getQuantity() const;

	void setQuantity(size_t quantity);

	void increaseQuantity(size_t amount);
	Response decreaseQuantity(size_t amount);

	std::ofstream& serialize(std::ofstream& os) const override;
	std::ifstream& deserialize(std::ifstream& is) override;
	Response deserializeFromTokens(const Vector<String>& tokens) override;
	
	String toString() const override;

private:
	size_t quantity_;
};