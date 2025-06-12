#include "../Transaction/Transaction.h"
#include "../Constants/TransactionConstants.h"
#include "../GiftCards/AllProductsGiftCard.h"
#include "../GiftCards/SingleCategoryGiftCard.h"
#include "../GiftCards/MultipleCategoriesGiftCard.h"

Transaction::Transaction()
	: id_(), cashierId_(0), products_(), 
    quanities_(), giftCard_(), timestamp_(std::time(nullptr))
{
}

Transaction::Transaction(size_t id, size_t cashierId)
	: id_(id), cashierId_(cashierId), products_(), 
    quanities_(), giftCard_(0), timestamp_(std::time(nullptr))
{
}

size_t Transaction::getId() const
{
	return this->id_;
}

size_t Transaction::getCashierId() const
{
	return this->cashierId_;
}

Vector<Product*> Transaction::getProducts() const
{
	return this->products_;
}

Vector<size_t> Transaction::getQuantities() const
{
	return this->quanities_;
}

size_t Transaction::getRawTotalMinor() const
{
	size_t productsCount = this->products_.size();
	size_t totalPriceMinor = 0;
	for (size_t i = 0; i < productsCount; i++)
	{
		if (this->products_[i]->getType() == ProductType::ByUnit)
		{
			totalPriceMinor +=
				this->products_[i]->getPriceMinor() * this->quanities_[i];
		}
		if (this->products_[i]->getType() == ProductType::ByWeight)
		{
			totalPriceMinor +=
				this->products_[i]->getPriceMinor() * (this->quanities_[i] / 1000.0);
		}
	}
	return totalPriceMinor;
}

void Transaction::addProduct(Product* product, size_t quantity)
{
	this->products_.push_back(product);
	this->quanities_.push_back(quantity);
}

void Transaction::addGiftCard(GiftCard* giftCard)
{
    if (this->giftCard_)
        delete this->giftCard_;
	this->giftCard_ = giftCard;
}

size_t Transaction::getFinalPriceMinor() const
{
	size_t	rawTotal = this->getRawTotalMinor();
	if (!this->giftCard_)
	{
		return rawTotal;
	}
    
    size_t discountPercent = this->giftCard_->getDiscount();
    GiftCardType type = this->giftCard_->getType();

    switch (type)
    {
    case GiftCardType::None:
        return rawTotal;

    case GiftCardType::All:
        return rawTotal - (rawTotal * (discountPercent / 100)) / 100;

    case GiftCardType::Single:
    {
        SingleCategoryGiftCard* singleCard = 
            dynamic_cast<SingleCategoryGiftCard*>(this->giftCard_);
        if (!singleCard) return rawTotal;

        size_t categoryId = singleCard->getCategoryId();
        size_t discountedAmount = 0;
        size_t otherAmount = 0;

        for (size_t i = 0; i < products_.size(); i++)
        {
            Product* product = products_[i];
            size_t quantity = quanities_[i];

            if (product->getCategoryId() == categoryId)
            {
                size_t productPrice = product->getPriceMinor();
                size_t totalProductPrice = 0;
                if (product->getType() == ProductType::ByUnit)
                {
                    totalProductPrice = productPrice * quantity;
                }
                else if (product->getType() == ProductType::ByWeight)
                {
                    totalProductPrice = productPrice * (quantity / 1000.0);
                }
                discountedAmount += totalProductPrice;
            }
            else
            {
                size_t productPrice = product->getPriceMinor();
                size_t totalProductPrice = 0;
                if (product->getType() == ProductType::ByUnit)
                {
                    totalProductPrice = productPrice * quantity;
                }
                else if (product->getType() == ProductType::ByWeight)
                {
                    totalProductPrice = productPrice * (quantity / 1000.0);
                }
                otherAmount += totalProductPrice;
            }
        }

        size_t discountedFinal = 
            discountedAmount - (discountedAmount * (discountPercent / 100)) / 100;
        return discountedFinal + otherAmount;
    }

    case GiftCardType::Multiple:
    {
        MultipleCategoriesGiftCard* multiCard = 
            dynamic_cast<MultipleCategoriesGiftCard*>(this->giftCard_);
        if (!multiCard) 
        {
            return rawTotal;
        }

        Vector<size_t> categoryIds = multiCard->getCategoryIds();
        size_t categoryIdsCount = categoryIds.size();

        size_t discountedAmount = 0;
        size_t otherAmount = 0;

        for (size_t i = 0; i < products_.size(); i++)
        {
            Product* product = products_[i];
            size_t quantity = quanities_[i];
            size_t productCategory = product->getCategoryId();

            bool isDiscounted = false;
            for (size_t i = 0; i < categoryIdsCount; i++)
            {
                if (categoryIds[i] == productCategory)
                {
                    isDiscounted = true;
                    break;
                }
            }

            size_t productPrice = product->getPriceMinor();
            size_t totalProductPrice = 0;
            if (product->getType() == ProductType::ByUnit)
            {
                totalProductPrice = productPrice * quantity;
            }
            else if (product->getType() == ProductType::ByWeight)
            {
                totalProductPrice = productPrice * (quantity / 1000.0);
            }

            if (isDiscounted)
            {
                discountedAmount += totalProductPrice;
            }
            else
            {
                otherAmount += totalProductPrice;
            }
        }

        size_t discountedFinal = 
            discountedAmount - (discountedAmount * (discountPercent / 100)) / 100;
        return discountedFinal + otherAmount;
    }
    }

    return rawTotal;
}

std::time_t Transaction::getTimestamp() const
{
    return this->timestamp_;
}

String Transaction::getIdString() const
{
	String result;
	if (this->id_ < 10000)
	{
		result.push_back('0');
	}
	if (this->id_ < 1000)
	{
		result.push_back('0');
	}
	if (this->id_ < 100)
	{
		result.push_back('0');
	}
	if (this->id_ < 10)
	{
		result.push_back('0');
	}
	result += String::toString(this->id_);
	return result;
}

std::ofstream& Transaction::serialize(std::ofstream& os) const 
{
    os.write((const char*)&this->id_, sizeof(this->id_));
    os.write((const char*)&this->cashierId_, sizeof(this->cashierId_));
    os.write((const char*)&this->timestamp_, sizeof(this->timestamp_));

    size_t productsCount = this->products_.size();
    os.write((const char*)&productsCount, sizeof(productsCount));

    for (size_t i = 0; i < productsCount; i++)
    {
        size_t productId = this->products_[i]->getId();
        size_t quantity = this->quanities_[i];

        os.write((const char*)&productId, sizeof(productId));
        os.write((const char*)&quantity, sizeof(quantity));
    }

    bool hasGiftCard = (this->giftCard_ != nullptr);
    os.write((const char*)&hasGiftCard, sizeof(hasGiftCard));

    if (hasGiftCard)
    {
        size_t giftCardId = this->giftCard_->getId();
        os.write((const char*)&giftCardId, sizeof(giftCardId));
    }

    return os;
}

std::ifstream& Transaction::deserialize(std::ifstream& is,
    const Vector<Product*>& products,
    const Vector<GiftCard*>& giftCards) 
{
    is.read((char*)&this->id_, sizeof(this->id_));
    is.read((char*)&this->cashierId_, sizeof(this->cashierId_));
    is.read((char*)&this->timestamp_, sizeof(this->timestamp_));

    size_t productsCount = 0;
    is.read((char*)&productsCount, sizeof(productsCount));

    this->products_.clear();
    this->quanities_.clear();

    for (size_t i = 0; i < productsCount; i++)
    {
        size_t productId = 0;
        size_t quantity = 0;
        is.read((char*)&productId, sizeof(productId));
        is.read((char*)&quantity, sizeof(quantity));

        Product* productPtr = nullptr;
        for (size_t j = 0; j < products.size(); j++)
        {
            if (products[j]->getId() == productId)
            {
                productPtr = products[j];
                break;
            }
        }

        if (productPtr)
        {
            this->products_.push_back(productPtr);
            this->quanities_.push_back(quantity);
        }
    }

    bool hasGiftCard = false;
    is.read((char*)&hasGiftCard, sizeof(hasGiftCard));

    if (hasGiftCard)
    {
        size_t giftCardId = 0;
        is.read((char*)&giftCardId, sizeof(giftCardId));

        this->giftCard_ = nullptr;
        for (size_t i = 0; i < giftCards.size(); i++)
        {
            if (giftCards[i]->getId() == giftCardId)
            {
                this->giftCard_ = giftCards[i];
                break;
            }
        }
    }
    else
    {
        this->giftCard_ = nullptr;
    }

    return is;
}

String Transaction::toString() const
{
    String dateTime = this->timestampToString();

    double finalTotal = this->getFinalPriceMinor() / 100.0;

    String result;
    result += "Cashier ID: " + String::toString(cashierId_) + "\n";
    result += "Final Total: " + String::toString(finalTotal) + " BGN\n";
    result += "Date: " + dateTime + "\n";
    result += "Receipt: receipt_" + this->getIdString() + ".txt\n";

    return result;
}

String Transaction::timestampToString() const
{
    std::time_t now = this->timestamp_;
    std::tm localTime;
    localtime_s(&localTime, &now);
    char timeBuffer[100];
    std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &localTime);
    return timeBuffer;
}

void Transaction::free()
{
	this->products_.clear();
	this->giftCard_ = nullptr;
}
