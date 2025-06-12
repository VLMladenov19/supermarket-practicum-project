#pragma once

#include "../GiftCards/GiftCard.h"

class SingleCategoryGiftCard : public GiftCard
{
public:
	SingleCategoryGiftCard();
	SingleCategoryGiftCard(size_t id, 
		size_t discount, size_t categoryId);
	SingleCategoryGiftCard(size_t id, const String& code, 
		size_t discount, size_t categoryId);

	GiftCardType getType() const override;
	size_t getCategoryId() const;

	std::ofstream& serialize(std::ofstream& os) const override;
	std::ifstream& deserialize(std::ifstream& is) override;

	String toString() const override;

private:
	size_t categoryId_;

	void setCategoryId(size_t categoryId);
};