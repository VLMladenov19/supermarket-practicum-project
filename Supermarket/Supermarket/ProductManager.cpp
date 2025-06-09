#include "ProductManager.h"
#include "ProductByUnit.h"
#include "ProductByWeight.h"
#include "ProductConstants.h"
#include "CategoryConstants.h"
#include "GiftCardConstants.h"
#include "SingleCategoryGiftCard.h"
#include "MultipleCategoriesGiftCard.h"
#include "AllProductsGiftCard.h"

ProductManager::ProductManager()
{
	this->loadAll();
}

ProductManager::ProductManager(const ProductManager& other)
{
    this->copyFrom(other);
}

ProductManager& ProductManager::operator=(const ProductManager& other)
{
    if (this != &other)
    {
        this->free();
        this->copyFrom(other);
    }
    return *this;
}

ProductManager::~ProductManager()
{
    this->free();
}

Response ProductManager::addProduct(Product* product)
{
    this->products_.push_back(product);
    return this->uploadProducts();
}

Response ProductManager::addCategory(Category* category)
{
    this->categories_.push_back(category);
    return this->uploadCategories();
}

Response ProductManager::removeCategory(size_t id)
{
    size_t categoriesCount = this->categories_.size();
    for (size_t i = 0; i < categoriesCount; i++)
    {
        if (this->categories_[i]->getId() == id)
        {
            this->categories_.remove(i);
            return this->uploadCategories();
        }
    }
    return Response(false, "Invalid id.");
}

Response ProductManager::loadAll()
{
    Response res = this->loadProducts();
    if (!res.isSuccessful())
    {
        return res;
    }
    res = this->loadCategories();
    if (!res.isSuccessful())
    {
        return res;
    }
    res = this->loadGiftCards();
    if (!res.isSuccessful())
    {
        return res;
    }
    return Response(true, "All products, categories and gift cards loaded successfully.");
}

Response ProductManager::loadProducts()
{
    using namespace ProductConstants::File;
    std::ifstream is(PRODUCTS_FILE);

    if (!is.is_open())
    {
        std::ofstream createFile(PRODUCTS_FILE);
        if (!createFile.is_open())
        {
            return Response(false, "Failed to create products file.");
        }
        createFile.close();

        is.open(PRODUCTS_FILE);
        if (!is.is_open())
        {
            return Response(false, "Failed to open products file after creation.");
        }
    }

    this->products_.clear();

    size_t productsCount = 0;
    is >> productsCount;
    is.ignore();

    for (size_t i = 0; i < productsCount; i++)
    {
        String line;
        getline(is, line);
        if (line.empty())
        {
            continue;
        }

        Vector<String> tokens = line.split(':');
        ProductType type = 
            strToProductType(tokens[ProductConstants::DeserializeProduct::TYPE_INDEX]);

        Product* product = nullptr;
        switch (type)
        {
        case ProductType::ByUnit:
            product = new ProductByUnit();
            break;
        case ProductType::ByWeight:
            product = new ProductByWeight();
            break;
        default:
            delete product;
            return Response(false, "Invalid product in file.");
        }

        if (!product->deserializeFromTokens(tokens).isSuccessful())
        {
            delete product;
            continue;
        }
        this->products_.push_back(product);
    }

    is.close();

    return Response(true, "Products loaded successfully.");
}

Response ProductManager::loadNewProducts(const String& filename)
{
    std::ifstream is(filename);

    if (!is.is_open())
    {
        return Response(false, "Failed to open " + filename + ".");
    }

    String line;
    while (getline(is, line))
    {
        if (line.empty())
            continue;

        Vector<String> tokens = line.split(':');

        if (tokens.size() == ProductConstants::NewProduct::INPUT_SIZE)
        {
            Category* category =
                this->getCategoryByName(
                    tokens[ProductConstants::NewProduct::CATEGORY_NAME_INDEX]);
            if (!category)
            {
                continue;
            }

            Product* newProduct = nullptr;
            ProductType type = 
                strToProductType(tokens[ProductConstants::NewProduct::TYPE_INDEX]);
            switch (type)
            {
            case ProductType::ByUnit:
                newProduct = new ProductByUnit(
                    this->getNextProductId(),
                    tokens[ProductConstants::NewProduct::NAME_INDEX],
                    category->getId(),
                    tokens[ProductConstants::NewProduct::PRICE_MINOR_INDEX]
                    .toNumber(),
                    tokens[ProductConstants::NewProduct::QUANTITY_INDEX]
                    .toNumber()
                );
                break;
            case ProductType::ByWeight:
                newProduct = new ProductByWeight(
                    this->getNextProductId(),
                    tokens[ProductConstants::NewProduct::NAME_INDEX],
                    category->getId(),
                    tokens[ProductConstants::NewProduct::PRICE_MINOR_INDEX]
                    .toNumber(),
                    tokens[ProductConstants::NewProduct::WEIGHT_INDEX]
                    .toNumber() / 1000.0
                );
                break;
            default:
                continue;
            }
            if (!newProduct)
            {
                delete newProduct;
                continue;
            }
            this->products_.push_back(newProduct);
        }
        if (tokens.size() == ProductConstants::IncreaseProduct::INPUT_SIZE)
        {
            if (tokens[CategoryConstants::NewCategory::ACTION_INDEX] == "CATEGORY")
            {
                Category* category = new Category(
                    this->getNextCategoryId(),
                    tokens[CategoryConstants::NewCategory::NAME_INDEX],
                    tokens[CategoryConstants::NewCategory::DESCRIPTION_INDEX]
                );
                Response res = this->addCategory(category);
                continue;
            }
            String productName =
                tokens[ProductConstants::IncreaseProduct::NAME_INDEX];
            Product* product = this->getProductByName(productName);

            if (!product)
            {
                continue;
            }

            ProductType type = 
                strToProductType(tokens[ProductConstants::IncreaseProduct::TYPE_INDEX]);
            if (type != product->getType())
            {
                continue;
            }
            switch (type)
            {
            case ProductType::ByUnit:
            {
                ProductByUnit* productByUnit = 
                    dynamic_cast<ProductByUnit*>(product);
                productByUnit->increaseQuantity(
                    tokens[ProductConstants::IncreaseProduct::QUANTITY_INDEX]
                    .toNumber());
            }
                break;
            case ProductType::ByWeight:
            {
                ProductByWeight* productByWeight = 
                    dynamic_cast<ProductByWeight*>(product);
                productByWeight->increaseWeight(
                    tokens[ProductConstants::IncreaseProduct::WEIGHT_INDEX]
                    .toNumber() / 1000.0);
            }
                break;
            }
        }
    }
    is.close();

    this->uploadProducts();
    return Response(true, "New products loaded and quantities updated.");
}

Response ProductManager::loadCategories()
{
    std::ifstream is(CategoryConstants::File::CATEGORIES_FILE);

    if (!is.is_open())
    {
        std::ofstream createFile(CategoryConstants::File::CATEGORIES_FILE);
        if (!createFile.is_open())
        {
            return Response(false, "Failed to create categories file.");
        }
        createFile.close();

        // Retry opening after creation
        is.open(CategoryConstants::File::CATEGORIES_FILE);
        if (!is.is_open())
        {
            return Response(false, "Failed to open categories file after creation.");
        }
    }

    this->categories_.clear();

    size_t categoriesCount = 0;
    is >> categoriesCount;
    is.ignore();

    for (size_t i = 0; i < categoriesCount; i++)
    {
        Category* category = new Category();

        if (category->deserialize(is).fail())
        {
            delete category;
            continue;
        }

        this->categories_.push_back(category);
    }

    return Response(true, "Categories loaded successfully.");
}

Response ProductManager::loadGiftCards()
{
    std::ifstream is(GiftCardConstants::File::GIFT_CARDS_FILE);

    if (!is.is_open())
    {
        std::ofstream createFile(GiftCardConstants::File::GIFT_CARDS_FILE);
        if (!createFile.is_open())
        {
            return Response(false, "Failed to create gift cards file.");
        }
        createFile.close();

        // Retry opening after creation
        is.open(GiftCardConstants::File::GIFT_CARDS_FILE);
        if (!is.is_open())
        {
            return Response(false, "Failed to open gift cards file after creation.");
        }
    }

    this->giftCards_.clear();

    size_t giftCardsCount = 0;
    is >> giftCardsCount;
    is.ignore();

    for (size_t i = 0; i < giftCardsCount; i++)
    {
        GiftCard* giftCard = nullptr;
        String giftCardTypeStr;
        getline(is, giftCardTypeStr, ':');
        GiftCardType giftCardType = (GiftCardType)giftCardTypeStr.toNumber();

        switch (giftCardType)
        {
        case GiftCardType::All: giftCard = new AllProductsGiftCard(); break;
        case GiftCardType::Single: giftCard = new SingleCategoryGiftCard(); break;
        case GiftCardType::Multiple: giftCard = new MultipleCategoriesGiftCard(); break;
        }

        if (!giftCard || giftCard->deserialize(is).fail())
        {
            delete giftCard;
            continue;
        }

        this->giftCards_.push_back(giftCard);
    }

    return Response(true, "Gift cards loaded successfully.");
}

Response ProductManager::loadNewGiftCards(const String& filename)
{
    std::ifstream is(filename);

    if (!is.is_open())
    {
        return Response(false, "Failed to open " + filename + ".");
    }

    using namespace GiftCardConstants::NewGiftCard;
    String line;
    while (getline(is, line))
    {
        if (line.empty())
            continue;

        Vector<String> tokens = line.split(':');

        if (tokens.size() < MIN_INPUT_SIZE)
        {
            continue;
        }

        GiftCard* giftCard = nullptr;
        GiftCardType giftCardType = strToGiftCardType(tokens[TYPE_INDEX]);
        switch (giftCardType)
        {
        case GiftCardType::All:
            giftCard = new AllProductsGiftCard(
                this->getNextGiftCardId(),
                tokens[ALL_DISCOUNT_SIZE].toNumber()
            );
            break;
        case GiftCardType::Single:
        {
            String name = tokens[SINGLE_CATEGORY_NAME_INDEX];
            Category* category = this->getCategoryByName(name);

            if (category)
            {
                giftCard = new SingleCategoryGiftCard(
                    this->getNextGiftCardId(),
                    tokens[SINGLE_DISCOUNT_INDEX].toNumber(),
                    category->getId()
                );
            }
            break;
        }
        case GiftCardType::Multiple:
            Vector<size_t> categoryIds;
            size_t categoryIdsCount = tokens[MULTIPLE_COUNT_INDEX].toNumber();
            for (size_t i = 0; i < categoryIdsCount; i++)
            {
                String name = tokens[MULTIPLE_COUNT_INDEX + i + 1];
                Category* category = this->getCategoryByName(name);

                if (category)
                {
                    categoryIds.push_back(category->getId());
                }
            }
            size_t discount = tokens[tokens.size() - 1]
                .toNumber();
            giftCard = new MultipleCategoriesGiftCard(
                this->getNextGiftCardId(),
                discount,
                categoryIds
            );
            break;
        }
        if (!giftCard)
        {
            delete giftCard;
            continue;
        }
        this->giftCards_.push_back(giftCard);
    }
    is.close();

    this->uploadGiftCards();
    return Response(true, "New gift cards loaded.");
}

Response ProductManager::uploadProducts()
{
    std::ofstream os(ProductConstants::File::PRODUCTS_FILE, std::ios::binary);

    if (!os.is_open())
    {
        return Response(false, "Failed to open products file.");
    }

    size_t productsCount = this->products_.size();
    os << productsCount << '\n';

    for (size_t i = 0; i < productsCount; i++)
    {
        Product* product = this->products_[i];

        if (product)
        {
            product->serialize(os);
        }
    }

    os.close();

    return Response(true, "Users file updated successfully.");
}

Response ProductManager::uploadCategories()
{
    std::ofstream os(CategoryConstants::File::CATEGORIES_FILE, std::ios::binary);

    if (!os.is_open())
    {
        return Response(false, "Failed to open categories file.");
    }

    size_t categoriesCount = this->categories_.size();
    os << categoriesCount << '\n';

    for (size_t i = 0; i < categoriesCount; i++)
    {
        Category* category = this->categories_[i];

        if (category)
        {
            category->serialize(os);
        }
    }

    os.close();

    return Response(true, "Users file updated successfully.");
}

Response ProductManager::uploadGiftCards()
{
    std::ofstream os(GiftCardConstants::File::GIFT_CARDS_FILE, std::ios::binary);

    if (!os.is_open())
    {
        return Response(false, "Failed to open gift cards file.");
    }

    size_t giftCardsCount = this->giftCards_.size();
    os << giftCardsCount << '\n';

    for (size_t i = 0; i < giftCardsCount; i++)
    {
        GiftCard* giftCard = this->giftCards_[i];

        if (giftCard)
        {
            giftCard->serialize(os);
        }
    }

    os.close();

    return Response(true, "Gift cards file updated successfully.");
}

Product* ProductManager::getProductById(size_t id)
{
    size_t productsCount = this->products_.size();
    for (size_t i = 0; i < productsCount; i++)
    {
        if (this->products_[i]->getId() == id)
        {
            return this->products_[i];
        }
    }
    return nullptr;
}

Product* ProductManager::getProductByName(const String& name)
{
    size_t productsCount = this->products_.size();
    for (size_t i = 0; i < productsCount; i++)
    {
        if (this->products_[i]->getName() == name)
        {
            return this->products_[i];
        }
    }
    return nullptr;
}

Category* ProductManager::getCategoryById(size_t id)
{
    size_t categoriesCount = this->categories_.size();
    for (size_t i = 0; i < categoriesCount; i++)
    {
        if (this->categories_[i]->getId() == id)
        {
            return this->categories_[i];
        }
    }
    return nullptr;
}

Category* ProductManager::getCategoryByName(const String& name)
{
    size_t categoriesCount = this->categories_.size();
    for (size_t i = 0; i < categoriesCount; i++)
    {
        if (this->categories_[i]->getName() == name)
        {
            return this->categories_[i];
        }
    }
    return nullptr;
}

Vector<Product*> ProductManager::getProductsByCategoryId(size_t categoryId)
{
    Vector<Product*> products;
    size_t productsCount = this->products_.size();
    for (size_t i = 0; i < productsCount; i++)
    {
        if (this->products_[i]->getCategoryId() == categoryId)
        {
            return products.push_back(this->products_[i]);
        }
    }
    return products;
}

size_t ProductManager::getNextProductId() const
{
    size_t maxId = ProductConstants::BASE_ID - 1;
    size_t productsCount = this->products_.size();
    for (size_t i = 0; i < productsCount; i++)
    {
        maxId = std::max(maxId, this->products_[i]->getId());
    }
    return maxId + 1;
}

size_t ProductManager::getNextCategoryId() const
{
    size_t maxId = CategoryConstants::BASE_ID - 1;
    size_t categoriesCount = this->categories_.size();
    for (size_t i = 0; i < categoriesCount; i++)
    {
        maxId = std::max(maxId, this->categories_[i]->getId());
    }
    return maxId + 1;
}

size_t ProductManager::getNextGiftCardId() const
{
    size_t maxId = GiftCardConstants::BASE_ID - 1;
    size_t giftCardsCount = this->giftCards_.size();
    for (size_t i = 0; i < giftCardsCount; i++)
    {
        maxId = std::max(maxId, this->giftCards_[i]->getId());
    }
    return maxId + 1;
}

const Vector<Product*> ProductManager::getProducts() const
{
    return this->products_;
}

const Vector<Category*> ProductManager::getCategories() const
{
    return this->categories_;
}

const Vector<GiftCard*> ProductManager::getGiftCards() const
{
    return this->giftCards_;
}

void ProductManager::copyFrom(const ProductManager& other)
{
    this->products_ = other.products_;
}

void ProductManager::free()
{
    this->freeProducts();
    this->freeCategories();
}

void ProductManager::freeProducts()
{
    size_t productsCount = this->products_.size();
    for (size_t i = 0; i < productsCount; i++)
    {
        delete this->products_[i];
    }
}

void ProductManager::freeCategories()
{
    size_t categoriesCount = this->categories_.size();
    for (size_t i = 0; i < categoriesCount; i++)
    {
        delete this->categories_[i];
    }
}

void ProductManager::freeGiftCards()
{
    size_t giftCardsCount = this->giftCards_.size();
    for (size_t i = 0; i < giftCardsCount; i++)
    {
        delete this->giftCards_[i];
    }
}
