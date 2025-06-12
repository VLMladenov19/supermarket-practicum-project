#include "../GiftCards/GiftCard.h"
#include "../Utils/Random.h"
#include "../Constants/GiftCardConstants.h"

GiftCard::GiftCard()
	: id_(0), code_(), discount_(0)
{
}

GiftCard::GiftCard(size_t id, size_t discount)
	: id_(id), discount_(discount)
{
	this->code_ = this->generateCode();
}

GiftCard::GiftCard(size_t id, const String& code, size_t discount)
	: id_(id), code_(code), discount_(discount)
{
}

GiftCardType GiftCard::getType() const
{
	return GiftCardType::None;
}

size_t GiftCard::getId() const
{
	return this->id_;
}

const String& GiftCard::getCode() const
{
	return this->code_;
}

size_t GiftCard::getDiscount() const
{
	return this->discount_;
}

std::ofstream& GiftCard::serialize(std::ofstream& os) const
{
	os << String::toString((size_t)this->getType()) << ':'
		<< String::toString(this->id_) << ':'
		<< this->code_ << ':'
		<< String::toString(this->discount_) << '\n';
	return os;
}

std::ifstream& GiftCard::deserialize(std::ifstream& is)
{
	using namespace GiftCardConstants::DeserializeAllGiftCard;
	String line;
	getline(is, line);

	Vector<String> tokens = line.split(':');
	this->setId(tokens[ID_INDEX].toNumber());
	this->setCode(tokens[CODE_INDEX]);
	this->setDiscount(tokens[DISCOUNT_INDEX].toNumber());

	return is;
}

String GiftCard::toString() const
{
	String result;

	result.push_back('<');
	result += String::toString(this->id_) + "> ";
	result += giftCardTypeToStr(this->getType()) + ": ";
	result += this->code_ + ", Discount: ";
	result += String::toString(this->discount_ / 100.0) + "%";

	return result;
}

void GiftCard::setId(size_t id)
{
	this->id_ = id;
}

void GiftCard::setCode(const String& code)
{
	this->code_ = code;
}

void GiftCard::setDiscount(size_t discount)
{
	this->discount_ = discount;
}

String GiftCard::generateCode() const
{
	String code;

	code.push_back(Random::randomDigit());
	code.push_back(Random::randomUpper());

	if (this->id_ < 10)
	{
		code.push_back('0');
	}
	if (this->id_ < 100)
	{
		code.push_back('0');
	}
	code += String::toString(this->id_);
	code.push_back(Random::randomUpper());
	code.push_back(Random::randomDigit());

	return code;
}

String giftCardTypeToStr(GiftCardType type)
{
	switch (type)
	{
	case GiftCardType::Single: return "Single";
	case GiftCardType::Multiple: return "Multiple";
	case GiftCardType::All: return "All";
	default: return "None";
	}
}

GiftCardType strToGiftCardType(const String& str)
{
	String lower = str.toLower();

	if (lower == "single") return GiftCardType::Single;
	if (lower == "multiple") return GiftCardType::Multiple;
	if (lower == "all") return GiftCardType::All;

	return GiftCardType::None;
}
