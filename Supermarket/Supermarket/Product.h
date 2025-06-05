#pragma once

#include "String.h"
#include "Category.h"

enum class ProductType
{
	None,
	ByUnit,
	ByWeight
};

String typeToStr(ProductType type);
ProductType strToType(const String& str);

class Product
{
public:
	Product(const String& name, const Category& category, size_t priceMinor);

	virtual ProductType getType() const = 0;
	const String& getName() const;
	const Category& getCategory() const;
	size_t getPriceMinor() const;

	void setName(const String& name);
	void setCategory(const Category& category);
	void setPriceMinor(size_t priceMinor);
	
	String getPriceString() const;

protected:
	String name_;
	Category category_;
	size_t priceMinor_; // Price is stored in minor units for accuracy
};