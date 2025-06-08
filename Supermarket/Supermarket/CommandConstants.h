#pragma once

namespace CommandConstants
{
	const size_t COMMAND_INDEX = 0;

	namespace Login
	{
		const size_t INPUT_SIZE = 3;
		const size_t ID_INDEX = 1;
		const size_t PASSWORD_INDEX = 2;
	}

	namespace Logout
	{
		const size_t INPUT_SIZE = 1;
	}

	namespace AddUser
	{
		const size_t INPUT_SIZE = 7;
		const size_t ROLE_INDEX = 1;
		const size_t FIRST_NAME_INDEX = 2;
		const size_t LAST_NAME_INDEX = 3;
		const size_t PHONE_NUMBER_INDEX = 4;
		const size_t AGE_INDEX = 5;
		const size_t PASSWORD_INDEX = 6;
	}

	namespace RemoveCurrentUser
	{
		const size_t INPUT_SIZE = 1;
	}

	namespace ListUserData
	{
		const size_t INPUT_SIZE = 1;
	}

	namespace ListUsers
	{
		const size_t INPUT_SIZE = 1;
	}

	namespace ManageCashier
	{
		const size_t INPUT_SIZE = 3;
		const size_t CASHIER_ID_INDEX = 1;
		const size_t SPECIAL_CODE_INDEX = 2;
	}

	namespace AddCategory
	{
		const size_t INPUT_MIN_SIZE = 2;
		const size_t NAME_INDEX = 1;
		const size_t DESCRIPTION_INDEX = 2;
	}

	namespace DeleteCategory
	{
		const size_t INPUT_SIZE = 2;
		const size_t CATEGORY_ID_INDEX = 1;
	}

	namespace ListProducts
	{
		const size_t INPUT_SIZE = 1;
	}

	namespace ListProductsByCategory
	{
		const size_t INPUT_SIZE = 2;
		const size_t CATEGORY_ID_INDEX = 1;
	}

	namespace ListCategories
	{
		const size_t INPUT_SIZE = 1;
	}

	namespace LoadProducts
	{
		const size_t INPUT_SIZE = 2;
		const size_t FILE_NAME_INDEX = 1;
	}

	namespace LoadGiftCards
	{
		const size_t INPUT_SIZE = 2;
		const size_t FILE_NAME_INDEX = 1;
	}
}