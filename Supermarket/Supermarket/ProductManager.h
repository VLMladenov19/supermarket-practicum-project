#pragma once

#include "Response.h"
#include "Vector.h"
#include "Product.h"
#include "Category.h"
#include "GiftCard.h"
#include "Transaction.h"

class ProductManager
{
public:
	ProductManager();
	ProductManager(const ProductManager& other);
	ProductManager& operator=(const ProductManager& other);
	~ProductManager();

	Response addProduct(Product* product);
	Response removeProduct(size_t id);
	Response addCategory(Category* category);
	Response removeCategory(size_t id);

	Response loadAll();
	Response loadProducts();
	Response loadNewProducts(const String& filename);
	Response loadCategories();
	Response loadGiftCards();
	Response loadNewGiftCards(const String& filename);
	Response loadTransactions();

	Response uploadProducts();
	Response uploadCategories();
	Response uploadGiftCards();
	Response uploadTransactions();

	Response addTransaction(Transaction* transaction);
	Response createReceipt(const Transaction* transaction);

	Product* getProductById(size_t id);
	Product* getProductByName(const String& name);
	Category* getCategoryById(size_t id);
	Category* getCategoryByName(const String& name);
	Vector<Product*> getProductsByCategoryId(size_t categoryId);
	GiftCard* getGiftCardById(size_t id);
	GiftCard* getGiftCardByCode(const String& code);

	size_t getNextProductId() const;
	size_t getNextCategoryId() const;
	size_t getNextGiftCardId() const;
	size_t getNextTransactionId() const;

	const Vector<Product*> getProducts() const;
	const Vector<Category*> getCategories() const;
	const Vector<GiftCard*> getGiftCards() const;
	const Vector<Transaction*> getTransaction() const;

private:
	Vector<Product*> products_;
	Vector<Category*> categories_;
	Vector<GiftCard*> giftCards_;
	Vector<Transaction*> transactions_;

	void copyFrom(const ProductManager& other);
	void free();
	void freeProducts();
	void freeCategories();
	void freeGiftCards();
	void freeTransactions();
};