#include "CommandHandler.h"
#include "CommandConstants.h"
#include "Cashier.h"
#include "Manager.h"

CommandHandler::CommandHandler(UserManager& userManager,
	ProductManager& productManager)
	: userManager_(userManager), productManager_(productManager)
{
}

void CommandHandler::dispatch(const Vector<String>& inputs)
{
	if (inputs.empty())
	{
		return;
	}

	const String& command = inputs[CommandConstants::COMMAND_INDEX];
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
	if (command == "leave")
	{
		return removeCurrentUser(inputs);
	}
	if (command == "list-user-data")
	{
		return listCurrentUserData(inputs);
	}
	if (command == "list-workers")
	{
		return listUsers(inputs);
	}
	if (command == "list-pending")
	{
		return listPending(inputs);
	}
	if (command == "approve")
	{
		return approvePending(inputs);
	}
	if (command == "decline")
	{
		return declinePending(inputs);
	}
	if (command == "promote-cashier")
	{
		return promoteCashier(inputs);
	}
	if (command == "fire-cashier")
	{
		return fireCashier(inputs);
	}
	if (command == "list-products")
	{
		return listProducts(inputs);
	}
	if (command == "load-products")
	{
		return loadNewProducts(inputs);
	}
}

void CommandHandler::login(const Vector<String>& inputs)
{
	using namespace CommandConstants::Login;
	if (inputs.size() != INPUT_SIZE)
	{
		std::cout << "Invalid inputs.\n";
		return;
	}

	Response res = this->userManager_.login(inputs[ID_INDEX].toNumber(),
		inputs[PASSWORD_INDEX]);

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
	if (inputs.size() != CommandConstants::Logout::INPUT_SIZE)
	{
		std::cout << "Invalid inputs.\n";
		return;
	}

	Response res = this->userManager_.logout();
	std::cout << res.getMessage() << '\n';
}

void CommandHandler::addUser(const Vector<String>& inputs)
{
	using namespace CommandConstants::AddUser;
	if (inputs.size() != INPUT_SIZE)
	{
		std::cout << "Invalid inputs.\n";
		return;
	}

	UserRole role = strToRole(inputs[ROLE_INDEX]);
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

void CommandHandler::removeCurrentUser(const Vector<String>& inputs)
{
	if (inputs.size() != CommandConstants::RemoveCurrentUser::INPUT_SIZE)
	{
		std::cout << "Invalid inputs.\n";
		return;
	}
	User* currentUser = this->userManager_.getCurrentUser();
	if (!currentUser)
	{
		std::cout << "User not logged in.\n";
		return;
	}
	Response res = this->userManager_.removeUser(currentUser->getId());
	if (!res.isSuccessful())
	{
		std::cout << res.getMessage() << '\n';
		return;
	}
	std::cout << "Worker successfully left job.\n";
}

void CommandHandler::listCurrentUserData(const Vector<String>& inputs)
{
	if (inputs.size() != CommandConstants::ListUserData::INPUT_SIZE)
	{
		std::cout << "Invalid inputs.\n";
		return;
	}
	User* currentUser = this->userManager_.getCurrentUser();
	if (!currentUser)
	{
		std::cout << "User not logged in.\n";
		return;
	}

	std::cout << this->userManager_.getCurrentUser()->toString() << '\n';
}

void CommandHandler::listUsers(const Vector<String>& inputs)
{
	if (inputs.size() != CommandConstants::ListUserData::INPUT_SIZE)
	{
		std::cout << "Invalid inputs.\n";
		return;
	}
	Vector<User*> users = this->userManager_.getUsers();
	size_t usersCount = users.size();
	if (!usersCount)
	{
		std::cout << "No workers!\n";
		return;
	}
	for (size_t i = 0; i < usersCount; i++)
	{
		std::cout << users[i]->toString() << '\n';
	}
}

void CommandHandler::listPending(const Vector<String>& inputs)
{
	User* currentUser = this->userManager_.getCurrentUser();
	if (!currentUser || currentUser->getRole() != UserRole::Manager)
	{
		std::cout << "Access denied.\n";
		return;
	}
	if (inputs.size() != CommandConstants::ListUserData::INPUT_SIZE)
	{
		std::cout << "Invalid inputs.\n";
		return;
	}
	Vector<User*> pendingUsers = this->userManager_.getPendingUsers();
	size_t pendingUsersCount = pendingUsers.size();
	if (!pendingUsersCount)
	{
		std::cout << "No pending approvals!\n";
		return;
	}
	for (size_t i = 0; i < pendingUsersCount; i++)
	{
		std::cout << pendingUsers[i]->toString() << '\n';
	}
}

void CommandHandler::approvePending(const Vector<String>& inputs)
{
	using namespace CommandConstants::ManageCashier;
	User* currentUser = this->userManager_.getCurrentUser();
	if (!currentUser || currentUser->getRole() != UserRole::Manager)
	{
		std::cout << "Access denied.\n";
		return;
	}
	if (inputs.size() != INPUT_SIZE)
	{
		std::cout << "Invalid inputs.\n";
		return;
	}
	Response res = this->userManager_.approveCashier(
		inputs[CASHIER_ID_INDEX].toNumber(),
		inputs[SPECIAL_CODE_INDEX]);
	if (!res.isSuccessful())
	{
		std::cout << res.getMessage() << '\n';
		return;
	}
	std::cout << "Cashier approved successfully!\n";
}

void CommandHandler::declinePending(const Vector<String>& inputs)
{
	using namespace CommandConstants::ManageCashier;
	User* currentUser = this->userManager_.getCurrentUser();
	if (!currentUser || currentUser->getRole() != UserRole::Manager)
	{
		std::cout << "Access denied.\n";
		return;
	}
	if (inputs.size() != INPUT_SIZE)
	{
		std::cout << "Invalid inputs.\n";
		return;
	}
	Response res = this->userManager_.declineCashier(
		inputs[CASHIER_ID_INDEX].toNumber(),
		inputs[SPECIAL_CODE_INDEX]);
	if (!res.isSuccessful())
	{
		std::cout << res.getMessage() << '\n';
		return;
	}
	std::cout << "Cashier declined successfully!\n";
}

void CommandHandler::promoteCashier(const Vector<String>& inputs)
{
	using namespace CommandConstants::ManageCashier;
	User* currentUser = this->userManager_.getCurrentUser();
	if (!currentUser || currentUser->getRole() != UserRole::Manager)
	{
		std::cout << "Access denied.\n";
		return;
	}
	if (inputs.size() != INPUT_SIZE)
	{
		std::cout << "Invalid inputs.\n";
		return;
	}
	Response res = this->userManager_.promoteCashier(
		inputs[CASHIER_ID_INDEX].toNumber(),
		inputs[SPECIAL_CODE_INDEX]);
	if (!res.isSuccessful())
	{
		std::cout << res.getMessage() << '\n';
		return;
	}
	std::cout << "Cashier promoted successfully!\n";
}

void CommandHandler::fireCashier(const Vector<String>& inputs)
{
	using namespace CommandConstants::ManageCashier;
	User* currentUser = this->userManager_.getCurrentUser();
	if (!currentUser || currentUser->getRole() != UserRole::Manager)
	{
		std::cout << "Access denied.\n";
		return;
	}
	if (inputs.size() != INPUT_SIZE)
	{
		std::cout << "Invalid inputs.\n";
		return;
	}
	Response res = this->userManager_.fireCashier(
		inputs[CASHIER_ID_INDEX].toNumber(),
		inputs[SPECIAL_CODE_INDEX]);
	if (!res.isSuccessful())
	{
		std::cout << res.getMessage() << '\n';
		return;
	}
	std::cout << "Cashier fired successfully!\n";
}

void CommandHandler::listProducts(const Vector<String>& inputs)
{
	if (inputs.size() != CommandConstants::ListProducts::INPUT_SIZE)
	{
		std::cout << "Invalid inputs.\n";
		return;
	}
	Vector<Product*> products = this->productManager_.getProducts();
	size_t productsCount = products.size();
	if (!productsCount)
	{
		std::cout << "No products!\n";
		return;
	}
	for (size_t i = 0; i < productsCount; i++)
	{
		std::cout << products[i]->toString() << '\n';
	}
}

void CommandHandler::loadNewProducts(const Vector<String>& inputs)
{
	using namespace CommandConstants::LoadProducts;
	User* currentUser = this->userManager_.getCurrentUser();
	if (!currentUser || currentUser->getRole() != UserRole::Manager)
	{
		std::cout << "Access denied.\n";
		return;
	}
	if (inputs.size() != INPUT_SIZE)
	{
		std::cout << "Invalid inputs.\n";
		return;
	}

	String fileName = "files/" + inputs[FILE_NAME_INDEX];
	Response res = this->productManager_.loadNewProducts(fileName);
	if (!res.isSuccessful())
	{
		std::cout << res.getMessage() << '\n';
		return;
	}
	std::cout << "Products loaded successfully.\n";
}

void CommandHandler::addCashier(const Vector<String>& inputs)
{
	using namespace CommandConstants::AddUser;
	if (inputs.size() != INPUT_SIZE)
	{
		std::cout << "Invalid inputs.\n";
		return;
	}

	Cashier* cashier = new Cashier(this->userManager_.getNextUserId(),
		inputs[FIRST_NAME_INDEX],
		inputs[LAST_NAME_INDEX],
		inputs[PHONE_NUMBER_INDEX],
		(unsigned short)inputs[AGE_INDEX].toNumber(),
		inputs[PASSWORD_INDEX]);

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
	using namespace CommandConstants::AddUser;
	if (inputs.size() != INPUT_SIZE)
	{
		std::cout << "Invalid inputs.\n";
		return;
	}
	
	Manager* manager = new Manager(this->userManager_.getNextUserId(),
		inputs[FIRST_NAME_INDEX],
		inputs[LAST_NAME_INDEX],
		inputs[PHONE_NUMBER_INDEX],
		(unsigned short)inputs[AGE_INDEX].toNumber(),
		inputs[PASSWORD_INDEX]);

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
