#pragma once

#include "Response.h"
#include "String.h"
#include "User.h"

class UserManager
{
public:
	UserManager() = default;
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

	Response uploadAll();
	Response uploadUsers();
	Response uploadPendingUsers();

	Response loadUsers();
	Response loadPendingUsers();

	User* getUserById(size_t id) const;
	User* getPendingUserById(size_t id) const;

	const Vector<User*>& getUsers() const;
	const Vector<User*>& getPendingUsers() const;
	User* getCurrentUser() const;
	size_t getNextUserId();

private:
	Vector<User*> users;
	Vector<User*> pendingUsers;
	User* currentUser = nullptr;

	void freeUsers();
	void freePendingUsers();

	void copyFrom(const UserManager& other);
	void free();

	const String usersFile = "files/users.bin";
	const String pendingUsersFile = "files/pending_users.bin";
};