#pragma once

#include "GiftCard.h"

class AllProductsGiftCard : public GiftCard
{
public:
	AllProductsGiftCard();
	AllProductsGiftCard(size_t id, size_t discount);
	AllProductsGiftCard(size_t id, const String& code, size_t discount);

	GiftCardType getType() const override;
};