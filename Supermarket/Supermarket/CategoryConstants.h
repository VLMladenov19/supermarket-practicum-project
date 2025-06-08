#pragma once

namespace CategoryConstants
{
	const size_t BASE_ID = 100;

	namespace File
	{
		const String CATEGORIES_FILE = "files/categories.txt";
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