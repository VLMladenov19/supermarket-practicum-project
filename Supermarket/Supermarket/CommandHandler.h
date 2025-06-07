#pragma once

#include "Vector.h"
#include "String.h"
#include "UserManager.h"

class CommandHandler
{
public:
	CommandHandler(UserManager& userManager);

	void dispatch(const Vector<String>& inputs);

	void login(const Vector<String>& inputs);
	void logout(const Vector<String>& inputs);
	void addUser(const Vector<String>& inputs);

private:
	void addCashier(const Vector<String>& inputs);
	void addManager(const Vector<String>& inputs);

	UserManager& userManager_;

	static constexpr size_t COMMAND_INDEX = 0;

	static constexpr size_t LOGIN_INPUT_SIZE = 3;
	static constexpr size_t LOGIN_ID_INDEX = 1;
	static constexpr size_t LOGIN_PASSWORD_INDEX = 2;

	static constexpr size_t LOGOUT_INPUT_SIZE = 1;

	static constexpr size_t ADD_USER_INPUT_SIZE = 7;
	static constexpr size_t ADD_USER_ROLE_INDEX = 1;
	static constexpr size_t ADD_USER_FIRST_NAME_INDEX = 2;
	static constexpr size_t ADD_USER_LAST_NAME_INDEX = 3;
	static constexpr size_t ADD_USER_PHONE_NUMBER_INDEX = 4;
	static constexpr size_t ADD_USER_AGE_INDEX = 5;
	static constexpr size_t ADD_USER_PASSWORD_INDEX = 6;

	static constexpr size_t REMOVE_USER_INPUT_SIZE = 2;
	static constexpr size_t REMOVE_USER_ID_INDEX = 1;
};