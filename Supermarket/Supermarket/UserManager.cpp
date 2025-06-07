#include "UserManager.h"
#include "Cashier.h"
#include "Manager.h"

UserManager::UserManager(const UserManager& other)
{
    this->copyFrom(other);
}

UserManager& UserManager::operator=(const UserManager& other)
{
    if (this != &other)
    {
        this->free();
        this->copyFrom(other);
    }
    return *this;
}

UserManager::~UserManager()
{
    this->freeUsers();
    this->freePendingUsers();
}

Response UserManager::login(size_t id, const String& pwd)
{
    if (this->currentUser)
    {
        return Response(false, "User already logged in.");
    }

    User* user = this->getUserById(id);

    if (!user)
    {
        return Response(false, "Invalid user id.");
    }

    if (!user->checkPassword(pwd))
    {
        return Response(false, "Wrong password.");
    }

    this->currentUser = user;
    return Response(true, "Login successful.");
}

Response UserManager::logout()
{
    if (!this->currentUser)
    {
        return Response(false, "User not logged in.");
    }

    this->currentUser = nullptr;
    return Response(true, "User logged out successfully.");
}

Response UserManager::registerUser(User* user)
{
    if (user->getRole() == UserRole::Cashier)
    {
        this->pendingUsers.push_back(user);
        this->uploadPendingUsers();
        return Response(true, "Cashier approval added successfully.");
    }
    if (user->getRole() == UserRole::Manager)
    {
        this->users.push_back(user);
        this->uploadUsers();
        return Response(true, "Manager added successfully.");
    }
    return Response(false, "Invalid role.");
}

Response UserManager::removeUser(size_t id)
{
    if (this->currentUser->getId() == id)
    {
        this->currentUser = nullptr;
    }

    size_t usersCount = this->users.size();
    for (size_t i = 0; i < usersCount; i++)
    {
        if (this->users[i]->getId() == id)
        {
            this->users.remove(i);
            return this->uploadUsers();
        }
    }

    return Response(false, "Invalid id.");
}

Response UserManager::approveCashier(size_t id, const String& specialCode)
{
    if (this->currentUser->getRole() != UserRole::Manager)
    {
        return Response(false, "Invalid access.");
    }
    Manager* manager = dynamic_cast<Manager*>(this->currentUser);

    if (!manager->compareSpecialCode(specialCode))
    {
        return Response(false, "Wrong special code.");
    }
    
    size_t pendingUsersCount = this->pendingUsers.size();
    for (size_t i = 0; i < pendingUsersCount; i++)
    {
        if (this->pendingUsers[i]->getId() == id)
        {
            this->users.push_back(this->pendingUsers[i]);
            this->pendingUsers.remove(i);
            return this->uploadAll();
        }
    }

    return Response(false, "Invalid id.");
}

Response UserManager::declinePending(size_t id, const String& specialCode)
{
    if (this->currentUser->getRole() != UserRole::Manager)
    {
        return Response(false, "Invalid access.");
    }
    Manager* manager = dynamic_cast<Manager*>(this->currentUser);

    if (!manager->compareSpecialCode(specialCode))
    {
        return Response(false, "Wrong special code.");
    }

    size_t pendingUsersCount = this->pendingUsers.size();
    for (size_t i = 0; i < pendingUsersCount; i++)
    {
        if (this->pendingUsers[i]->getId() == id)
        {
            this->pendingUsers.remove(i);
            return this->uploadPendingUsers();
        }
    }

    return Response(false, "Invalid id.");
}

Response UserManager::uploadAll()
{
    Response res = this->uploadUsers();
    if (!res.isSuccessful())
    {
        return res;
    }

    res = uploadPendingUsers();
    if (!res.isSuccessful())
    {
        return res;
    }

    return Response(true, "All users uploaded successfully.");
}

Response UserManager::uploadUsers()
{
    std::ofstream os(this->usersFile, std::ios::binary);

    if (!os.is_open())
    {
        return Response(false, "Failed to open users file.");
    }

    size_t usersCount = this->users.size();
    os.write((const char*)&usersCount, sizeof(usersCount));

    for (size_t i = 0; i < usersCount; i++)
    {
        User* user = this->users[i];

        if (user)
        {
            user->serialize(os);
        }
    }

    os.close();

    return Response(true, "Users file updated successfully.");
}

Response UserManager::uploadPendingUsers()
{
    std::ofstream os(this->pendingUsersFile, std::ios::binary);

    if (!os.is_open())
    {
        return Response(false, "Failed to open pending users file.");
    }

    size_t pendingUsersCount = this->pendingUsers.size();
    os.write((const char*)&pendingUsersCount, sizeof(pendingUsersCount));

    for (size_t i = 0; i < pendingUsersCount; i++)
    {
        User* user = this->pendingUsers[i];

        if (user)
        {
            user->serialize(os);
        }
    }

    os.close();

    return Response(true, "Pending users file updated successfully.");
}

Response UserManager::loadUsers()
{
    std::ifstream is(this->usersFile, std::ios::binary);

    if (!is.is_open())
    {
        return Response(false, "Failed to open users file.");
    }

    this->users.clear();

    size_t usersCount = 0;
    is.read((char*)&usersCount, sizeof(usersCount));

    for (size_t i = 0; i < usersCount; i++)
    {
        UserRole role = UserRole::None;
        is.read((char*)&role, sizeof(role));

        User* user = nullptr;
        switch (role)
        {
        case UserRole::Cashier:
            user = new Cashier();
            break;
        case UserRole::Manager:
            user = new Manager();
            break;
        default:
            return Response(false, "Invalid user in file.");
        }

        user->deserialize(is);
        this->users.push_back(user);
    }

    is.close();

    return Response(true, "Users loaded successfully.");
}

Response UserManager::loadPendingUsers()
{
    std::ifstream is(this->pendingUsersFile, std::ios::binary);

    if (!is.is_open())
    {
        return Response(false, "Failed to open pending users file.");
    }

    this->pendingUsers.clear();

    size_t usersCount = 0;
    is.read((char*)&usersCount, sizeof(usersCount));

    for (size_t i = 0; i < usersCount; i++)
    {
        UserRole role = UserRole::None;
        is.read((char*)&role, sizeof(role));

        User* user = nullptr;
        switch (role)
        {
        case UserRole::Cashier:
            user = new Cashier();
            break;
        case UserRole::Manager:
            user = new Manager();
            break;
        default:
            return Response(false, "Invalid user in file.");
        }

        user->deserialize(is);
        this->pendingUsers.push_back(user);
    }

    is.close();

    return Response(true, "Pending users loaded successfully.");
}

User* UserManager::getUserById(size_t id) const
{
    size_t usersCount = this->users.size();
    for (size_t i = 0; i < usersCount; i++)
    {
        if (this->users[i]->getId() == id)
        {
            return this->users[i];
        }
    }
    return nullptr;
}

User* UserManager::getPendingUserById(size_t id) const
{
    size_t pendingUsersCount = this->pendingUsers.size();
    for (size_t i = 0; i < pendingUsersCount; i++)
    {
        if (this->pendingUsers[i]->getId() == id)
        {
            return this->pendingUsers[i];
        }
    }
    return nullptr;
}

const Vector<User*>& UserManager::getUsers() const
{
    return this->users;
}

const Vector<User*>& UserManager::getPendingUsers() const
{
    return this->pendingUsers;
}

User* UserManager::getCurrentUser() const
{
    return this->currentUser;
}

size_t UserManager::getNextUserId()
{
    size_t usersCount = this->users.size();
    size_t pendingUsersCount = this->pendingUsers.size();

    size_t lastUserId = 100;
    if (usersCount != 0)
    {
        lastUserId = this->users[usersCount - 1]->getId();
    }
    size_t lastPendingUserId = 100;
    if (pendingUsersCount != 0)
    {
        lastPendingUserId = this->pendingUsers[pendingUsersCount - 1]->getId();
    }
    size_t lastId = std::max(lastUserId, lastPendingUserId);
    return lastId + 1;
}

void UserManager::freeUsers()
{
    size_t usersCount = this->users.size();
    for (size_t i = 0; i < usersCount; i++)
    {
        delete this->users[i];
    }
}

void UserManager::freePendingUsers()
{
    size_t pendingUsersCount = this->pendingUsers.size();
    for (size_t i = 0; i < pendingUsersCount; i++)
    {
        delete this->pendingUsers[i];
    }
}

void UserManager::copyFrom(const UserManager& other)
{
    this->users = other.users;
    this->pendingUsers = other.pendingUsers;
    this->currentUser = other.currentUser;
}

void UserManager::free()
{
    this->currentUser = nullptr;
    this->freeUsers();
    this->freePendingUsers();
}
