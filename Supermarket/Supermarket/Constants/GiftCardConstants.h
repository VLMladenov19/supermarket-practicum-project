#pragma once

#include "../Utils/String.h"

namespace GiftCardConstants
{
	const size_t BASE_ID = 1;

	namespace File
	{
		const String GIFT_CARDS_FILE = "files/gift_cards.txt";
	}

	namespace DeserializeAllGiftCard
	{
		const size_t TOKENS_SIZE = 3;
		const size_t ID_INDEX = 0;
		const size_t CODE_INDEX = 1;
		const size_t DISCOUNT_INDEX = 2;
	}

	namespace DeserializeSingleGiftCard
	{
		const size_t TOKENS_SIZE = 4;
		const size_t ID_INDEX = 0;
		const size_t CODE_INDEX = 1;
		const size_t CATEGORY_ID_INDEX = 2;
		const size_t DISCOUNT_INDEX = 3;
	}

	namespace DeserializeMultipleGiftCard
	{
		const size_t TOKENS_MIN_SIZE = 4;
		const size_t ID_INDEX = 0;
		const size_t CODE_INDEX = 1;
		const size_t CATEGORIES_COUNT_INDEX = 2;
	}

	namespace NewGiftCard
	{
		const size_t MIN_INPUT_SIZE = 2;
		const size_t TYPE_INDEX = 0;

		const size_t ALL_INPUT_SIZE = 2;
		const size_t ALL_DISCOUNT_SIZE = 1;

		const size_t SINGLE_INPUT_SIZE = 3;
		const size_t SINGLE_CATEGORY_NAME_INDEX = 1;
		const size_t SINGLE_DISCOUNT_INDEX = 2;

		const size_t MULTIPLE_COUNT_INDEX = 1;
	}
}