#include "MultipleCategoriesGiftCard.h"
#include "GiftCardConstants.h"

MultipleCategoriesGiftCard::MultipleCategoriesGiftCard()
    : GiftCard()
{
}

MultipleCategoriesGiftCard::MultipleCategoriesGiftCard(size_t id, 
    size_t discount, Vector<size_t> categoryIds)
    : GiftCard(id, discount), categoryIds_(categoryIds)
{
}

MultipleCategoriesGiftCard::MultipleCategoriesGiftCard(size_t id, 
    const String& code, size_t discount, Vector<size_t> categoryIds)
    : GiftCard(id, code, discount), categoryIds_(categoryIds)
{
}

GiftCardType MultipleCategoriesGiftCard::getType() const
{
    return GiftCardType::Multiple;
}

const Vector<size_t> MultipleCategoriesGiftCard::getCategoryIds() const
{
    return this->categoryIds_;
}

std::ofstream& MultipleCategoriesGiftCard::serialize(std::ofstream& os) const
{
    size_t categoryIdsCount = this->categoryIds_.size();
    os << String::toString(this->id_) << ':'
        << this->code_ << ':'
        << String::toString(categoryIdsCount) << ':';
    for (size_t i = 0; i < categoryIdsCount; i++)
    {
        os << String::toString(this->categoryIds_[i]) << ':';
    }
    os << String::toString(this->discount_) << '\n';
    return os;
}

std::ifstream& MultipleCategoriesGiftCard::deserialize(std::ifstream& is)
{
    using namespace GiftCardConstants::DeserializeMultipleGiftCard;
    String line;
    getline(is, line);

    Vector<String> tokens = line.split(':');
    this->setId(tokens[ID_INDEX].toNumber());
    this->setCode(tokens[CODE_INDEX]);

    Vector<size_t> categoryIds;
    size_t categoryIdsCount = tokens[CATEGORIES_COUNT_INDEX].toNumber();
    for (size_t i = 0; i < categoryIdsCount; i++)
    {
        size_t categoryId = tokens[CATEGORIES_COUNT_INDEX + i + 1].toNumber();
        categoryIds.push_back(categoryId);
    }
    this->setCategoryIds(categoryIds);

    this->setDiscount(tokens[tokens.size() - 1].toNumber());

    return is;
}

void MultipleCategoriesGiftCard::setCategoryIds(Vector<size_t> categoryIds)
{
    this->categoryIds_ = categoryIds;
}
