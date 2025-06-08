#pragma once

#include "Response.h"
#include "Vector.h"
#include "Product.h"

class ProductManager
{
public:
	ProductManager();
	ProductManager(const ProductManager& other);
	ProductManager& operator=(const ProductManager& other);
	~ProductManager();

	Response addProduct(Product* product);

	Response loadAll();
	Response loadProducts();
	Response loadNewProducts(String filename);
	Response loadCategories();

	Response uploadProducts();

	Product* getProductById(size_t id);
	Category* getCategoryById(size_t id);
	Category* getCategoryByName(const String& name);

	size_t getNextProductId() const;

	const Vector<Product*> getProducts() const;

private:
	Vector<Product*> products_;
	Vector<Category*> categories_;

	void copyFrom(const ProductManager& other);
	void free();
	void freeProducts();
	void freeCategories();
};