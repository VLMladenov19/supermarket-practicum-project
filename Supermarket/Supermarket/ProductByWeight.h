#pragma once

#include "Product.h"
#include "Response.h"

class ProductByWeight : public Product
{
public:
	ProductByWeight();
	ProductByWeight(size_t id, const String& name, 
		size_t categoryId, size_t priceMinor, double weight);

	ProductType getType() const override;

	double getWeight() const;

	void setWeight(double weight);

	void increaseWeight(double amount);
	Response decreaseWeight(double amount);

	std::ofstream& serialize(std::ofstream& os) const override;
	std::ifstream& deserialize(std::ifstream& is) override;
	Response deserializeFromTokens(const Vector<String>& tokens) override;

	String toString() const override;

private:
	double weight_;
};
