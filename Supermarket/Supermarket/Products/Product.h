#pragma once

#include <fstream>

#include "../Utils/String.h"
#include "../Category/Category.h"
#include "../Utils/Response.h"

enum class ProductType
{
	None,
	ByUnit,
	ByWeight
};

String productTypeToStr(ProductType type);
ProductType strToProductType(const String& str);

class Product
{
public:
	Product();
	Product(size_t id, const String& name, 
		size_t categoryId, size_t priceMinor);

	virtual ProductType getType() const = 0;
	size_t getId() const;
	const String& getName() const;
	size_t getCategoryId() const;
	size_t getPriceMinor() const;
	String getPriceString() const;

	void setName(const String& name);
	void setCategoryId(size_t categoryId);
	void setPriceMinor(size_t priceMinor);

	virtual std::ofstream& serialize(std::ofstream& os) const = 0;
	virtual std::ifstream& deserialize(std::ifstream& is) = 0;
	virtual Response deserializeFromTokens(const Vector<String>& tokens);

	virtual String toString() const;

protected:
	size_t id_;
	String name_;
	size_t categoryId_;
	size_t priceMinor_; // Price is stored in minor units for accuracy

	void setId(size_t id);
};