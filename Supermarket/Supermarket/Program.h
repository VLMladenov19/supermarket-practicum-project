#pragma once

#include "Response.h"
#include "Vector.h"
#include "User.h"
#include "Product.h"

namespace Constants
{
	const char* USERS_FILE = "files/users.bin";
};

class Program
{
public:
	Program();

	void run();

	~Program();

private:
	void loop();
	void save();

	Response login(size_t id, const String& pwd);
	Response logout();
	Response registerUser(User* user);
	Response removeUser(size_t id);

	Response uploadUsers();
	Response uploadPendingUsers();

	Response loadUsers();
	Response loadPendingUsers();

	size_t getNextUserId();
	User* getUserById(size_t id) const;

	Vector<User*> users;
	Vector<User*> pendingUsers;
	User* currentUser = nullptr;
	Vector<Product*> products;

	void freeUsers();
	void freePendingUsers();
};