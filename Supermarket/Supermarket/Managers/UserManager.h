#pragma once

#include "../Utils/Response.h"
#include "../Utils/String.h"
#include "../Users/User.h"

class UserManager
{
public:
	UserManager();
	UserManager(const UserManager& other);
	UserManager& operator=(const UserManager& other);
	~UserManager();

	Response login(size_t id, const String& pwd);
	Response logout();
	Response registerUser(User* user);
	Response removeUser(size_t id);

	Response approveCashier(size_t id, const String& specialCode);
	Response declineCashier(size_t id, const String& specialCode);
	Response promoteCashier(size_t id, const String& specialCode);
	Response fireCashier(size_t id, const String& specialCode);
	Response warnCashier(size_t id, size_t points);

	Response loadAll();
	Response loadUsers();
	Response loadPendingUsers();

	Response uploadAll() const;
	Response uploadUsers() const;
	Response uploadPendingUsers() const;

	User* getUserById(size_t id) const;
	User* getPendingUserById(size_t id) const;

	const Vector<User*>& getUsers() const;
	const Vector<User*>& getPendingUsers() const;
	User* getCurrentUser() const;
	size_t getNextUserId() const;

private:
	Vector<User*> users_;
	Vector<User*> pendingUsers_;
	User* currentUser_ = nullptr;

	void copyFrom(const UserManager& other);
	void free();
	void freeUsers();
	void freePendingUsers();

	const String USERS_FILE = "files/users.bin";
	const String PENDING_USERS_FILE = "files/pending_users.bin";
	const size_t BASE_USER_ID = 100;
};