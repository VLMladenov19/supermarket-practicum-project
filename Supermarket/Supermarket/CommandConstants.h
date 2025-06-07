#pragma once

namespace CommandConstants
{
	constexpr size_t COMMAND_INDEX = 0;

	namespace Login
	{
		constexpr size_t INPUT_SIZE = 3;
		constexpr size_t ID_INDEX = 1;
		constexpr size_t PASSWORD_INDEX = 2;
	}

	namespace Logout
	{
		constexpr size_t INPUT_SIZE = 1;
	}

	namespace AddUser
	{
		constexpr size_t INPUT_SIZE = 7;
		constexpr size_t ROLE_INDEX = 1;
		constexpr size_t FIRST_NAME_INDEX = 2;
		constexpr size_t LAST_NAME_INDEX = 3;
		constexpr size_t PHONE_NUMBER_INDEX = 4;
		constexpr size_t AGE_INDEX = 5;
		constexpr size_t PASSWORD_INDEX = 6;
	}

	namespace RemoveCurrentUser
	{
		constexpr size_t INPUT_SIZE = 1;
	}

	namespace ListUserData
	{
		constexpr size_t INPUT_SIZE = 1;
	}

	namespace ListUsers
	{
		constexpr size_t INPUT_SIZE = 1;
	}

	namespace ManagePending
	{
		constexpr size_t INPUT_SIZE = 3;
		constexpr size_t CASHIER_ID_INDEX = 1;
		constexpr size_t SPECIAL_CODE_INDEX = 2;
	}
}