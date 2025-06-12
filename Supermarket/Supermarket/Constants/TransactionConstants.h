#pragma once

#include "../Utils/String.h"

namespace TransactionConstants
{
	const size_t BASE_ID = 1;

	namespace File
	{
		const String TRANSACTIONS_FILE = "files/transactions.bin";
		const String RECEIPT_BEGGINING_FILE = "files/receipts/receipt_";
		const String RECEIPT_END_FILE = ".txt";
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