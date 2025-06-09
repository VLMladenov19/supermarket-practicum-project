#pragma once

#include "GiftCard.h"

class MultipleCategoriesGiftCard : public GiftCard
{
public:
	MultipleCategoriesGiftCard();
	MultipleCategoriesGiftCard(size_t id,
		size_t discount, Vector<size_t> categoryIds);
	MultipleCategoriesGiftCard(size_t id, const String& code,
		size_t discount, Vector<size_t> categoryIds);

	GiftCardType getType() const override;
	const Vector<size_t> getCategoryIds() const;

	std::ofstream& serialize(std::ofstream& os) const override;
	std::ifstream& deserialize(std::ifstream& is) override;

	String toString() const override;

private:
	Vector<size_t> categoryIds_;

	void setCategoryIds(Vector<size_t> categoryIds);
};