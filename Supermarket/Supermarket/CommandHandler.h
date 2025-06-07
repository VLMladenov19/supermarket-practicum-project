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
	void removeCurrentUser(const Vector<String>& inputs);
	void listCurrentUserData(const Vector<String>& inputs);
	void listUsers(const Vector<String>& inputs);
	void listPending(const Vector<String>& inputs);
	void approvePending(const Vector<String>& inputs);
	void declinePending(const Vector<String>& inputs);

private:
	void addCashier(const Vector<String>& inputs);
	void addManager(const Vector<String>& inputs);

	UserManager& userManager_;
};