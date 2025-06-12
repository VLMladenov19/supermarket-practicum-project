#include "../Managers/UserManager.h"
#include "../Users/Cashier.h"
#include "../Users/Manager.h"

UserManager::UserManager()
{
    this->loadAll();
}

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
    if (this->currentUser_)
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

    this->currentUser_ = user;
    return Response(true, "Login successful.");
}

Response UserManager::logout()
{
    if (!this->currentUser_)
    {
        return Response(false, "User not logged in.");
    }

    this->currentUser_ = nullptr;
    return Response(true, "User logged out successfully.");
}

Response UserManager::registerUser(User* user)
{
    if (user->getRole() == UserRole::Cashier)
    {
        this->pendingUsers_.push_back(user);
        this->uploadPendingUsers();
        return Response(true, "Cashier approval added successfully.");
    }
    if (user->getRole() == UserRole::Manager)
    {
        this->users_.push_back(user);
        this->uploadUsers();
        return Response(true, "Manager added successfully.");
    }
    return Response(false, "Invalid role.");
}

Response UserManager::removeUser(size_t id)
{
    if (this->currentUser_->getId() == id)
    {
        this->currentUser_ = nullptr;
    }

    size_t usersCount = this->users_.size();
    for (size_t i = 0; i < usersCount; i++)
    {
        if (this->users_[i]->getId() == id)
        {
            this->users_.remove(i);
            return this->uploadUsers();
        }
    }

    return Response(false, "Invalid id.");
}

Response UserManager::approveCashier(size_t id, const String& specialCode)
{
    if (this->currentUser_->getRole() != UserRole::Manager)
    {
        return Response(false, "Invalid access.");
    }
    Manager* manager = dynamic_cast<Manager*>(this->currentUser_);

    if (!manager->compareSpecialCode(specialCode))
    {
        return Response(false, "Wrong special code.");
    }
    
    size_t pendingUsersCount = this->pendingUsers_.size();
    for (size_t i = 0; i < pendingUsersCount; i++)
    {
        if (this->pendingUsers_[i]->getId() == id)
        {
            this->users_.push_back(this->pendingUsers_[i]);
            this->pendingUsers_.remove(i);
            return this->uploadAll();
        }
    }

    return Response(false, "Invalid id.");
}

Response UserManager::declineCashier(size_t id, const String& specialCode)
{
    if (this->currentUser_->getRole() != UserRole::Manager)
    {
        return Response(false, "Invalid access.");
    }
    Manager* manager = dynamic_cast<Manager*>(this->currentUser_);

    if (!manager->compareSpecialCode(specialCode))
    {
        return Response(false, "Wrong special code.");
    }

    size_t pendingUsersCount = this->pendingUsers_.size();
    for (size_t i = 0; i < pendingUsersCount; i++)
    {
        if (this->pendingUsers_[i]->getId() == id)
        {
            this->pendingUsers_.remove(i);
            return this->uploadPendingUsers();
        }
    }

    return Response(false, "Invalid id.");
}

Response UserManager::promoteCashier(size_t id, const String& specialCode)
{
    if (this->currentUser_->getRole() != UserRole::Manager)
    {
        return Response(false, "Invalid access.");
    }
    Manager* manager = dynamic_cast<Manager*>(this->currentUser_);

    if (!manager->compareSpecialCode(specialCode))
    {
        return Response(false, "Wrong special code.");
    }

    size_t usersCount = this->users_.size();
    for (size_t i = 0; i < usersCount; i++)
    {
        if (this->users_[i]->getId() == id)
        {
            if (this->users_[i]->getRole() == UserRole::Manager)
            {
                return Response(false, "Already promoted.");
            }

            Cashier* cashier = dynamic_cast<Cashier*>(this->users_[i]);
            if (!cashier)
            {
                return Response(false, "User is not a Cashier.");
            }

            Manager* promoted = cashier->getAsManager();
            delete this->users_[i];
            this->users_[i] = promoted;
            return this->uploadUsers();
        }
    }

    return Response(false, "Invalid id.");
}

Response UserManager::fireCashier(size_t id, const String& specialCode)
{
    if (this->currentUser_->getRole() != UserRole::Manager)
    {
        return Response(false, "Invalid access.");
    }
    Manager* manager = dynamic_cast<Manager*>(this->currentUser_);

    if (!manager->compareSpecialCode(specialCode))
    {
        return Response(false, "Wrong special code.");
    }
    if (this->getUserById(id)->getRole() == UserRole::Manager)
    {
        return Response(false, "Can't fire manager.");
    }

    return this->removeUser(id);
}

Response UserManager::warnCashier(size_t id, size_t points)
{
    if (this->currentUser_->getRole() != UserRole::Manager)
    {
        return Response(false, "Invalid access.");
    }

    size_t usersCount = this->users_.size();
    for (size_t i = 0; i < usersCount; i++)
    {
        if (this->users_[i]->getId() == id)
        {
            if (this->users_[i]->getRole() == UserRole::Manager)
            {
                return Response(false, "Can not warn manager.");
            }

            Cashier* cashier = dynamic_cast<Cashier*>(this->users_[i]);
            if (!cashier)
            {
                return Response(false, "User is not a Cashier.");
            }

            Warning warning(
                this->currentUser_->getId(),
                points
            );
            cashier->addWarning(warning);
            return this->uploadUsers();
        }
    }

    return Response(false, "Invalid id.");
}

Response UserManager::loadAll()
{
    Response res = this->loadUsers();
    if (!res.isSuccessful())
    {
        return res;
    }

    res = loadPendingUsers();
    if (!res.isSuccessful())
    {
        return res;
    }

    return Response(true, "All users loaded successfully.");
}

Response UserManager::loadUsers()
{
    std::ifstream is(this->USERS_FILE, std::ios::binary);

    if (!is.is_open())
    {
        return Response(false, "Failed to open users file.");
    }

    this->users_.clear();

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

        if (user->deserialize(is).fail())
        {
            delete user;
            continue;
        }
        this->users_.push_back(user);
    }

    is.close();

    return Response(true, "Users loaded successfully.");
}

Response UserManager::loadPendingUsers()
{
    std::ifstream is(this->PENDING_USERS_FILE, std::ios::binary);

    if (!is.is_open())
    {
        return Response(false, "Failed to open pending users file.");
    }

    this->pendingUsers_.clear();

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

        if (user->deserialize(is).fail())
        {
            delete user;
            continue;
        }
        this->pendingUsers_.push_back(user);
    }

    is.close();

    return Response(true, "Pending users loaded successfully.");
}

Response UserManager::uploadAll() const
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

Response UserManager::uploadUsers() const
{
    std::ofstream os(this->USERS_FILE, std::ios::binary);

    if (!os.is_open())
    {
        return Response(false, "Failed to open users file.");
    }

    size_t usersCount = this->users_.size();
    os.write((const char*)&usersCount, sizeof(usersCount));

    for (size_t i = 0; i < usersCount; i++)
    {
        User* user = this->users_[i];

        if (user)
        {
            user->serialize(os);
        }
    }

    os.close();

    return Response(true, "Users file updated successfully.");
}

Response UserManager::uploadPendingUsers() const
{
    std::ofstream os(this->PENDING_USERS_FILE, std::ios::binary);

    if (!os.is_open())
    {
        return Response(false, "Failed to open pending users file.");
    }

    size_t pendingUsersCount = this->pendingUsers_.size();
    os.write((const char*)&pendingUsersCount, sizeof(pendingUsersCount));

    for (size_t i = 0; i < pendingUsersCount; i++)
    {
        User* user = this->pendingUsers_[i];

        if (user)
        {
            user->serialize(os);
        }
    }

    os.close();

    return Response(true, "Pending users file updated successfully.");
}

User* UserManager::getUserById(size_t id) const
{
    size_t usersCount = this->users_.size();
    for (size_t i = 0; i < usersCount; i++)
    {
        if (this->users_[i]->getId() == id)
        {
            return this->users_[i];
        }
    }
    return nullptr;
}

User* UserManager::getPendingUserById(size_t id) const
{
    size_t pendingUsersCount = this->pendingUsers_.size();
    for (size_t i = 0; i < pendingUsersCount; i++)
    {
        if (this->pendingUsers_[i]->getId() == id)
        {
            return this->pendingUsers_[i];
        }
    }
    return nullptr;
}

const Vector<User*>& UserManager::getUsers() const
{
    return this->users_;
}

const Vector<User*>& UserManager::getPendingUsers() const
{
    return this->pendingUsers_;
}

User* UserManager::getCurrentUser() const
{
    return this->currentUser_;
}

size_t UserManager::getNextUserId() const
{
    size_t maxId = this->BASE_USER_ID - 1;
    size_t usersCount = this->users_.size();
    for (size_t i = 0; i < usersCount; i++)
    {
        maxId = std::max(maxId, this->users_[i]->getId());
    }
    size_t pendingUsersCount = this->pendingUsers_.size();
    for (size_t i = 0; i < pendingUsersCount; i++)
    {
        maxId = std::max(maxId, this->pendingUsers_[i]->getId());
    }
    return maxId + 1;
}

void UserManager::freeUsers()
{
    size_t usersCount = this->users_.size();
    for (size_t i = 0; i < usersCount; i++)
    {
        delete this->users_[i];
    }
}

void UserManager::freePendingUsers()
{
    size_t pendingUsersCount = this->pendingUsers_.size();
    for (size_t i = 0; i < pendingUsersCount; i++)
    {
        delete this->pendingUsers_[i];
    }
}

void UserManager::copyFrom(const UserManager& other)
{
    this->users_ = other.users_;
    this->pendingUsers_ = other.pendingUsers_;
    this->currentUser_ = other.currentUser_;
}

void UserManager::free()
{
    this->currentUser_ = nullptr;
    this->freeUsers();
    this->freePendingUsers();
}
