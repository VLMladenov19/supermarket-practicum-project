#pragma once

#include "../Utils/Vector.h"
#include "../Utils/String.h"
#include "../Managers/UserManager.h"
#include "../Managers/ProductManager.h"

class CommandHandler
{
public:
	CommandHandler(UserManager& userManager, 
		ProductManager& productManager);

	void dispatch(const Vector<String>& inputs);

	void login(const Vector<String>& inputs);
	void logout(const Vector<String>& inputs);
	void addUser(const Vector<String>& inputs);
	void removeCurrentUser(const Vector<String>& inputs);
	void fireCashier(const Vector<String>& inputs);
	void listCurrentUserData(const Vector<String>& inputs);
	void listUsers(const Vector<String>& inputs);
	void listPending(const Vector<String>& inputs);
	void approvePending(const Vector<String>& inputs);
	void declinePending(const Vector<String>& inputs);
	void promoteCashier(const Vector<String>& inputs);
	void warnCashier(const Vector<String>& inputs);
	void listWarnedCashiers(const Vector<String>& inputs);

	void addProduct(const Vector<String>& inputs);
	void deleteProduct(const Vector<String>& inputs);
	void addCategory(const Vector<String>& inputs);
	void editCategory(const Vector<String>& inputs);
	void deleteCategory(const Vector<String>& inputs);

	void listProducts(const Vector<String>& inputs);
	void listProductsByCategory(const Vector<String>& inputs);
	void listCategories(const Vector<String>& inputs);
	void listGiftCards(const Vector<String>& inputs);
	void loadNewProducts(const Vector<String>& inputs);
	void loadNewGiftCards(const Vector<String>& inputs);

	void listFeed(const Vector<String>& inputs);
	
	void sell(const Vector<String>& inputs);
	void listTransactions(const Vector<String>& inputs);

private:
	void addCashier(const Vector<String>& inputs);
	void addManager(const Vector<String>& inputs);

	UserManager& userManager_;
	ProductManager& productManager_;
};