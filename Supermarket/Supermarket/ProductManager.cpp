#include "ProductManager.h"
#include "ProductByUnit.h"
#include "ProductByWeight.h"
#include "ProductConstants.h"

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

Response ProductManager::loadAll()
{
    Response res = this->loadProducts();
    if (!res.isSuccessful())
    {
        return res;
    }

    return Response(true, "All products loaded successfully.");
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
            strToType(tokens[ProductConstants::LoadProduct::TYPE_INDEX]);

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

    return Response(true, "Users loaded successfully.");
}

Response ProductManager::loadNewProducts(String filename)
{
    std::ifstream is(filename);

    if (!is.is_open())
    {
        std::ofstream createFile(filename);
        if (!createFile.is_open())
        {
            return Response(false, "Failed to create new_products file.");
        }
        createFile.close();

        is.open(filename);
        if (!is.is_open())
        {
            return Response(false, "Failed to open new_products file after creation.");
        }
    }

    String line;
    while (getline(is, line))
    {
        if (line.empty())
            continue;

        Vector<String> tokens = line.split(':');

        if (tokens.size() == ProductConstants::NewProduct::INPUT_SIZE)
        {
            Product* newProduct = nullptr;
            ProductType type = 
                strToType(tokens[ProductConstants::NewProduct::TYPE_INDEX]);
            switch (type)
            {
            case ProductType::ByUnit:
                newProduct = new ProductByUnit(
                    this->getNextProductId(),
                    tokens[ProductConstants::NewProduct::NAME_INDEX],
                    tokens[ProductConstants::NewProduct::CATEGORY_ID_INDEX]
                    .toNumber(),
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
                    tokens[ProductConstants::NewProduct::CATEGORY_ID_INDEX]
                    .toNumber(),
                    tokens[ProductConstants::NewProduct::PRICE_MINOR_INDEX]
                    .toNumber(),
                    tokens[ProductConstants::NewProduct::WEIGHT_INDEX]
                    .toNumber() / 100.0
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
            size_t productId = 
                tokens[ProductConstants::IncreaseProduct::ID_INDEX]
                .toNumber();
            Product* product = this->getProductById(productId);

            if (!product)
            {
                continue;
            }

            ProductType type = 
                strToType(tokens[ProductConstants::IncreaseProduct::TYPE_INDEX]);
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
                    .toNumber() / 100.0);
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
    std::ifstream is("test.txt");
    return Response(true);
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

size_t ProductManager::getNextProductId() const
{
    size_t maxId = ProductConstants::NewProduct::BASE_PRODUCT_ID - 1;
    size_t productsCount = this->products_.size();
    for (size_t i = 0; i < productsCount; i++)
    {
        maxId = std::max(maxId, this->products_[i]->getId());
    }
    return maxId + 1;
}

const Vector<Product*> ProductManager::getProducts() const
{
    return this->products_;
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
