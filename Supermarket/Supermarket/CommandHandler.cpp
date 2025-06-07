#include "CommandHandler.h"
#include "Cashier.h"
#include "Manager.h"

CommandHandler::CommandHandler(UserManager& userManager)
	: userManager_(userManager)
{
}

void CommandHandler::dispatch(const Vector<String>& inputs)
{
	if (inputs.empty())
	{
		return;
	}

	const String& command = inputs[this->COMMAND_INDEX];
	if (command == "login")
	{
		return login(inputs);
	}
	if (command == "logout")
	{
		return logout(inputs);
	}
	if (command == "register")
	{
		return addUser(inputs);
	}
}

void CommandHandler::login(const Vector<String>& inputs)
{
	if (inputs.size() != this->LOGIN_INPUT_SIZE)
	{
		std::cout << "Invalid inputs.\n";
		return;
	}

	Response res = this->userManager_.login(inputs[LOGIN_ID_INDEX].toNumber(),
		inputs[LOGIN_PASSWORD_INDEX]);

	if (!res.isSuccessful())
	{
		std::cout << res.getMessage() << '\n';
		return;
	}
	std::cout << "Logged in as " << 
		this->userManager_.getCurrentUser()->toString() << '\n';
}

void CommandHandler::logout(const Vector<String>& inputs)
{
	if (inputs.size() != this->LOGOUT_INPUT_SIZE)
	{
		std::cout << "Invalid inputs.\n";
		return;
	}

	Response res = this->userManager_.logout();
	std::cout << res.getMessage() << '\n';
}

void CommandHandler::addUser(const Vector<String>& inputs)
{
	if (inputs.size() != this->ADD_USER_INPUT_SIZE)
	{
		std::cout << "Invalid inputs.\n";
		return;
	}

	UserRole role = strToRole(inputs[this->ADD_USER_ROLE_INDEX]);
	switch (role)
	{
	case UserRole::Cashier:
		addCashier(inputs);
		break;
	case UserRole::Manager:
		addManager(inputs);
		break;
	default:
		std::cout << "Invalid role.\n";
		return;
	}
}

void CommandHandler::addCashier(const Vector<String>& inputs)
{
	if (inputs.size() != this->ADD_USER_INPUT_SIZE)
	{
		std::cout << "Invalid inputs.\n";
	}

	Cashier* cashier = new Cashier(this->userManager_.getNextUserId(),
		inputs[ADD_USER_FIRST_NAME_INDEX],
		inputs[ADD_USER_LAST_NAME_INDEX],
		inputs[ADD_USER_PHONE_NUMBER_INDEX],
		(unsigned short)inputs[ADD_USER_AGE_INDEX].toNumber(),
		inputs[ADD_USER_PASSWORD_INDEX]);

	Response res = this->userManager_.registerUser(cashier);

	if (!res.isSuccessful())
	{
		std::cout << res.getMessage() << '\n';
		return;
	}
	std::cout << "Cashier info " << cashier->toString() << '\n';
	std::cout << "Cashier registered, awaiting approval from a manager.\n";
}

void CommandHandler::addManager(const Vector<String>& inputs)
{
	if (inputs.size() != this->ADD_USER_INPUT_SIZE)
	{
		std::cout << "Invalid inputs.\n";
	}

	Manager* manager = new Manager(this->userManager_.getNextUserId(),
		inputs[ADD_USER_FIRST_NAME_INDEX],
		inputs[ADD_USER_LAST_NAME_INDEX],
		inputs[ADD_USER_PHONE_NUMBER_INDEX],
		(unsigned short)inputs[ADD_USER_AGE_INDEX].toNumber(),
		inputs[ADD_USER_PASSWORD_INDEX]);

	Response res = this->userManager_.registerUser(manager);

	if (!res.isSuccessful())
	{
		std::cout << res.getMessage() << '\n';
		return;
	}
	std::cout << "Manager info " << manager->toString() << '\n';
	std::cout << "Manager registered successfully!\n";
	std::cout << "Special code: " << manager->getSpecialCode() << '\n';
	std::cout << "Code: " << manager->getId() << "_special_code.txt\n";
}
