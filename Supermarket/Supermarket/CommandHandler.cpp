#include "CommandHandler.h"
#include "CommandConstants.h"
#include "Cashier.h"
#include "Manager.h"
#include "Logger.h"

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
		if (inputs.size() == CommandConstants::ListProducts::INPUT_SIZE)
		{
			return listProducts(inputs);
		}
		if (inputs.size() == CommandConstants::ListProductsByCategory::INPUT_SIZE)
		{
			return listProductsByCategory(inputs);
		}
	}
	if (command == "list-categories")
	{
		return listCategories(inputs);
	}
	if (command == "load-products")
	{
		return loadNewProducts(inputs);
	}
	if (command == "list-feed")
	{
		return listFeed(inputs);
	}
	if (command == "add-category")
	{
		return addCategory(inputs);
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
	String logMessage = "User (ID: " +
		String::toString(this->userManager_.getCurrentUser()->getId()) +
		") logged in";
	Logger::log(logMessage);
}

void CommandHandler::logout(const Vector<String>& inputs)
{
	if (inputs.size() != CommandConstants::Logout::INPUT_SIZE)
	{
		std::cout << "Invalid inputs.\n";
		return;
	}
	
	String logMessage = "User(ID: " +
		String::toString(this->userManager_.getCurrentUser()->getId());
	Response res = this->userManager_.logout();
	std::cout << res.getMessage() << '\n';
	logMessage += (res.isSuccessful()) ? " logged out." : " failed to log out.";
	Logger::log(logMessage);
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
	String logMessage = currentUser->toString();
	Response res = this->userManager_.removeUser(currentUser->getId());
	if (!res.isSuccessful())
	{
		logMessage += " attempted to leave job, but failed.";
		Logger::log(logMessage);
		std::cout << res.getMessage() << '\n';
		return;
	}
	std::cout << "Worker successfully left job.\n";

	logMessage += " successfully left the job.";
	Logger::log(logMessage);
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

	size_t cashierId = inputs[CASHIER_ID_INDEX].toNumber();
	String logMessage = "Manager (ID: " + String::toString(currentUser->getId()) +
		") approved pending cashier with ID: " + String::toString(cashierId);
	Logger::log(logMessage);
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

	size_t cashierId = inputs[CASHIER_ID_INDEX].toNumber();
	String logMessage = "Manager (ID: " + String::toString(currentUser->getId()) +
		") declined pending cashier with ID: " + String::toString(cashierId);
	Logger::log(logMessage);
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

	size_t cashierId = inputs[CASHIER_ID_INDEX].toNumber();
	String logMessage = "Manager (ID: " + String::toString(currentUser->getId()) +
		") promoted cashier with ID: " + String::toString(cashierId);
	Logger::log(logMessage);
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

	size_t cashierId = inputs[CASHIER_ID_INDEX].toNumber();
	String logMessage = "Manager (ID: " + String::toString(currentUser->getId()) +
		") fired cashier with ID: " + String::toString(cashierId);
	Logger::log(logMessage);
}

void CommandHandler::addCategory(const Vector<String>& inputs)
{
	using namespace CommandConstants::AddCategory;
	User* currentUser = this->userManager_.getCurrentUser();
	if (!currentUser || currentUser->getRole() != UserRole::Manager)
	{
		std::cout << "Access denied.\n";
		return;
	}
	size_t inputSize = inputs.size();
	if (inputSize < INPUT_MIN_SIZE)
	{
		std::cout << "Invalid inputs.\n";
		return;
	}

	String categoryName = inputs[NAME_INDEX];
	String categoryDesc;
	for (size_t i = DESCRIPTION_INDEX; i < inputSize; i++)
	{
		if (i > DESCRIPTION_INDEX)
		{
			categoryDesc.push_back(' ');
		}
		categoryDesc += inputs[i];
	}


	Category* category = new Category(
		this->productManager_.getNextCategoryId(),
		categoryName, 
		categoryDesc
	);
	Response res = this->productManager_.addCategory(category);
	if (!res.isSuccessful())
	{
		std::cout << res.getMessage() << '\n';
		return;
	}
	std::cout << "Category \"" << categoryName << "\" added successfully!\n";
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

void CommandHandler::listProductsByCategory(const Vector<String>& inputs)
{
	using namespace CommandConstants::ListProductsByCategory;
	if (inputs.size() != INPUT_SIZE)
	{
		std::cout << "Invalid inputs.\n";
		return;
	}
	Vector<Product*> products = this->productManager_.getProducts();
	size_t productsCount = products.size();
	size_t categoryId = inputs[CATEGORY_ID_INDEX].toNumber();
	bool hasProductsFromCategory = false;
	for (size_t i = 0; i < productsCount; i++)
	{
		if (products[i]->getCategoryId() == categoryId)
		{
			std::cout << products[i]->toString() << '\n';
			hasProductsFromCategory = true;
		}
	}
	if (!hasProductsFromCategory)
	{
		std::cout << "No products!\n";
	}
}

void CommandHandler::listCategories(const Vector<String>& inputs)
{
	if (inputs.size() != CommandConstants::ListCategories::INPUT_SIZE)
	{
		std::cout << "Invalid inputs.\n";
		return;
	}
	Vector<Category*> categories = this->productManager_.getCategories();
	size_t categoriesCount = categories.size();
	if (!categoriesCount)
	{
		std::cout << "No products!\n";
		return;
	}
	for (size_t i = 0; i < categoriesCount; i++)
	{
		std::cout << categories[i]->toString() << '\n';
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
	String logMsg = "Manager " + currentUser->getFirstName() + " loaded products from " + fileName;
	Logger::log(logMsg);
}

void CommandHandler::listFeed(const Vector<String>& inputs)
{
	std::ifstream is("files/feed_log.txt");
	if (!is.is_open()) {
		std::cout << "Could not open feed log file.";
		return;
	}

	String line;
	while (getline(is, line)) {
		std::cout << line << '\n';
	}

	is.close();
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

	String logMessage = "Cashier registered: " + cashier->toString() +
		" (awaiting approval)";
	Logger::log(logMessage);
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

	String logMessage = "Manager registered: " + manager->toString() +
		", special code: " + manager->getSpecialCode() +
		" (written to file " + String::toString(manager->getId()) + "_special_code.txt)";
	Logger::log(logMessage);
}
