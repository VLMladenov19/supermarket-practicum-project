#pragma once

#include "String.h"

namespace ProductConstants
{
	const size_t BASE_ID = 100;

	namespace File
	{
		const String PRODUCTS_FILE = "files/products.txt";
		const String CATEGORIES_FILE = "files/categories.txt";
	}

	namespace DeserializeProduct
	{
		const size_t TOKENS_SIZE = 6;
		const size_t TYPE_INDEX = 0;
		const size_t ID_INDEX = 1;
		const size_t NAME_INDEX = 2;
		const size_t CATEGORY_ID_INDEX = 3;
		const size_t PRICE_MINOR_INDEX = 4;
		const size_t QUANTITY_INDEX = 5;
		const size_t WEIGHT_INDEX = 5;
	}

	namespace NewProduct
	{
		const size_t INPUT_SIZE = 6;
		const size_t TYPE_INDEX = 1;
		const size_t NAME_INDEX = 2;
		const size_t CATEGORY_NAME_INDEX = 3;
		const size_t PRICE_MINOR_INDEX = 4;
		const size_t QUANTITY_INDEX = 5;
		const size_t WEIGHT_INDEX = 5;
	}

	namespace IncreaseProduct
	{
		const size_t INPUT_SIZE = 3;
		const size_t TYPE_INDEX = 0;
		const size_t NAME_INDEX = 1;
		const size_t QUANTITY_INDEX = 2;
		const size_t WEIGHT_INDEX = 2;
	}

	namespace NewCategory
	{
		const size_t INPUT_SIZE = 3;
		const size_t ACTION_INDEX = 0;
		const size_t NAME_INDEX = 1;
		const size_t DESCRIPTION_INDEX = 2;
	}

	namespace DeserializeCategory
	{
		const size_t TOKENS_SIZE = 3;
		const size_t ID_INDEX = 0;
		const size_t NAME_INDEX = 1;
		const size_t DESCRIPTION_INDEX = 2;
	}
}