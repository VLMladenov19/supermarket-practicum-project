#include "Program.h"
#include "Cashier.h"
#include "Manager.h"

Program::Program()
{
    loadUsers();
    loadPendingUsers();
}

void Program::run()
{
    std::cout << "Welcome to FMI Supermarket Management System 2024-2025!\n";
    std::cout << "Please choose an action:\n\n";

    loop();
    save();
}

Program::~Program()
{
    this->save();
    this->freeUsers();
    this->freePendingUsers();
}

void Program::save()
{
    this->uploadUsers();
    this->uploadPendingUsers();
}

Response Program::login(size_t id, const String& pwd)
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
        return Response(false, "Weong password.");
    }

    this->currentUser = user;
    return Response(true, "Login successful.");
}

Response Program::logout()
{
    if (!this->currentUser)
    {
        return Response(false, "User not logged in.");
    }

    this->currentUser = nullptr;    
    return Response(true, "User logged out successfully.");
}

Response Program::registerUser(User* user)
{
    if (user->getRole() == UserRole::Manager)
    {
        this->users.push_back(user);
        this->uploadUsers();
        return Response(true, "Manager added successfully.");
    }
    if (user->getRole() == UserRole::Cashier)
    {
        this->pendingUsers.push_back(user);
        this->uploadPendingUsers();
        return Response(true, "Cashier approval added successfully.");
    }
    return Response(false, "Invalid role.");
}

Response Program::removeUser(size_t id)
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

Response Program::uploadUsers()
{
    std::ofstream os(Constants::USERS_FILE, std::ios::binary);

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

Response Program::uploadPendingUsers()
{
    std::ofstream os(Constants::USERS_FILE, std::ios::binary);

    if (!os.is_open())
    {
        return Response(false, "Failed to open pending users file.");
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

    return Response(true, "Pending users file updated successfully.");
}

Response Program::loadUsers()
{
    std::ifstream is(Constants::USERS_FILE, std::ios::binary);

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
            user = new Cashier(0, "", "", "", 0, "");
            break;
        case UserRole::Manager:
            user = new Manager(0, "", "", "", 0, "");
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

Response Program::loadPendingUsers()
{
    std::ifstream is(Constants::USERS_FILE, std::ios::binary);

    if (!is.is_open())
    {
        return Response(false, "Failed to open pending users file.");
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
            user = new Cashier(0, "", "", "", 0, "");
            break;
        case UserRole::Manager:
            user = new Manager(0, "", "", "", 0, "");
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

size_t Program::getNextUserId()
{
    size_t lastId = this->users[this->users.size() - 1]->getId();
    return lastId == 0 ? 100 : lastId + 1;
}

User* Program::getUserById(size_t id) const
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

void Program::freeUsers()
{
    size_t usersCount = this->users.size();
    for (size_t i = 0; i < usersCount; i++)
    {
        delete this->users[i];
    }
}

void Program::freePendingUsers()
{
    size_t pendingUsersCount = this->pendingUsers.size();
    for (size_t i = 0; i < pendingUsersCount; i++)
    {
        delete this->pendingUsers[i];
    }
}
