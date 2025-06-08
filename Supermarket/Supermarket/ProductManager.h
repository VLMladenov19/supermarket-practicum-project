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
	Response addCategory(Category* category);
	Response removeCategory(size_t id);

	Response loadAll();
	Response loadProducts();
	Response loadNewProducts(String filename);
	Response loadCategories();

	Response uploadProducts();
	Response uploadCategories();

	Product* getProductById(size_t id);
	Product* getProductByName(const String& name);
	Category* getCategoryById(size_t id);
	Category* getCategoryByName(const String& name);
	Vector<Product*> getProductsByCategoryId(size_t categoryId);

	size_t getNextProductId() const;
	size_t getNextCategoryId() const;

	const Vector<Product*> getProducts() const;
	const Vector<Category*> getCategories() const;

private:
	Vector<Product*> products_;
	Vector<Category*> categories_;

	void copyFrom(const ProductManager& other);
	void free();
	void freeProducts();
	void freeCategories();
};