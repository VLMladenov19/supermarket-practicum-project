#include "SingleCategoryGiftCard.h"
#include "GiftCardConstants.h"

SingleCategoryGiftCard::SingleCategoryGiftCard()
	: GiftCard(), categoryId_(0)
{
}

SingleCategoryGiftCard::SingleCategoryGiftCard(size_t id, 
	size_t discount, size_t categoryId)
	: GiftCard(id, discount), categoryId_(categoryId)
{
}

SingleCategoryGiftCard::SingleCategoryGiftCard(size_t id, 
	const String& code, size_t discount, size_t categoryId)
	: GiftCard(id, code, discount), categoryId_(categoryId)
{
}

GiftCardType SingleCategoryGiftCard::getType() const
{
	return GiftCardType::Single;
}

size_t SingleCategoryGiftCard::getCategoryId() const
{
	return this->categoryId_;
}

std::ofstream& SingleCategoryGiftCard::serialize(std::ofstream& os) const
{
	os << String::toString((size_t)this->getType()) << ':'
		<< String::toString(this->id_) << ':'
		<< this->code_ << ':'
		<< String::toString(this->categoryId_) << ':'
		<< String::toString(this->discount_) << '\n';
	return os;
}

std::ifstream& SingleCategoryGiftCard::deserialize(std::ifstream& is)
{
	using namespace GiftCardConstants::DeserializeSingleGiftCard;
	String line;
	getline(is, line);

	Vector<String> tokens = line.split(':');
	this->setId(tokens[ID_INDEX].toNumber());
	this->setCode(tokens[CODE_INDEX]);
	this->setCategoryId(tokens[CATEGORY_ID_INDEX].toNumber());
	this->setDiscount(tokens[DISCOUNT_INDEX].toNumber());

	return is;
}

String SingleCategoryGiftCard::toString() const
{
	String result = GiftCard::toString();

	result += " for <" + String::toString(this->categoryId_);
	result.push_back('>');

	return result;
}

void SingleCategoryGiftCard::setCategoryId(size_t categoryId)
{
	this->categoryId_ = categoryId;
}
