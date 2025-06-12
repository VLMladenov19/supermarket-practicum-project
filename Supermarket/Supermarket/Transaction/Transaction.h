#pragma once

#include <ctime>

#include "../Utils/Vector.h"
#include "../Products/Product.h"
#include "../GiftCards/GiftCard.h"

class Transaction
{
public:
	Transaction();
	Transaction(size_t id, size_t cashiedId);

	size_t getId() const;
	size_t getCashierId() const;
	Vector<Product*> getProducts() const;
	Vector<size_t> getQuantities() const;
	size_t getRawTotalMinor() const;

	void addProduct(Product* product, size_t quantity);
	void addGiftCard(GiftCard* giftCard);

	size_t getFinalPriceMinor() const;
	std::time_t getTimestamp() const;
	String getIdString() const;

	std::ofstream& serialize(std::ofstream& os) const;
	std::ifstream& deserialize(std::ifstream& is,
		const Vector<Product*>& products,
		const Vector<GiftCard*>& giftCards);

	String toString() const;
	String timestampToString() const;

private:
	size_t id_;
	size_t cashierId_;
	Vector<Product*> products_;
	Vector<size_t> quanities_;
	GiftCard* giftCard_;
	std::time_t timestamp_;

	void free();
};