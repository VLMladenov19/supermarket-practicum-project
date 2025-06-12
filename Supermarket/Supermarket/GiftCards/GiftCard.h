#pragma once

#include <fstream>

#include "../Utils/String.h"

enum class GiftCardType
{
	None,
	Single,
	Multiple,
	All
};

String giftCardTypeToStr(GiftCardType type);
GiftCardType strToGiftCardType(const String& str);

class GiftCard
{
public:
	GiftCard();
	GiftCard(size_t id, size_t discount);
	GiftCard(size_t id, const String& code, size_t discount);

	virtual GiftCardType getType() const;
	size_t getId() const;
	const String& getCode() const;
	size_t getDiscount() const;

	virtual std::ofstream& serialize(std::ofstream& os) const;
	virtual std::ifstream& deserialize(std::ifstream& is);

	virtual String toString() const;

protected:
	size_t id_;
	String code_;
	size_t discount_;

	void setId(size_t id);
	void setCode(const String& code);
	void setDiscount(size_t discount);

	String generateCode() const;
};
