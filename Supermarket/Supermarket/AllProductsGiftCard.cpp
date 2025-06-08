#include "AllProductsGiftCard.h"
#include "GiftCardConstants.h"

AllProductsGiftCard::AllProductsGiftCard()
	: GiftCard()
{
}

AllProductsGiftCard::AllProductsGiftCard(size_t id, size_t discount)
	: GiftCard(id, discount)
{
}

AllProductsGiftCard::AllProductsGiftCard(size_t id, 
	const String& code, size_t discount)
	: GiftCard(id, code, discount)
{
}

GiftCardType AllProductsGiftCard::getType() const
{
	return GiftCardType::All;
}

