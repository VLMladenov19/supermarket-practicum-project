#include "User.h"

User::User()
    : id_(0), firstName_(), lastName_(), 
    phoneNumber_(), age_(0), password_()
{
}

User::User(size_t id, const String& firstName,
    const String& lastName, const String& phoneNumber, 
    unsigned short age, const String& pwd)
    : id_(id), firstName_(firstName), lastName_(lastName), 
    phoneNumber_(phoneNumber), age_(age), password_(pwd)
{
}

User::User(const User& other)
{
    this->copyFrom(other);
}

User& User::operator=(const User& other)
{
    if (this != &other)
    {
        this->copyFrom(other);
    }

    return *this;
}

User::User(User&& other) noexcept
    : id_(other.id_),
    firstName_(std::move(other.firstName_)),
    lastName_(std::move(other.lastName_)),
    phoneNumber_(std::move(other.phoneNumber_)),
    age_(other.age_),
    password_(std::move(other.password_))
{
    other.id_ = 0;
    other.age_ = 0;
}

User& User::operator=(User&& other) noexcept
{
    if (this != &other)
    {
        this->id_ = other.id_;
        this->firstName_ = std::move(other.firstName_);
        this->lastName_ = std::move(other.lastName_);
        this->phoneNumber_ = std::move(other.phoneNumber_);
        this->age_ = other.age_;
        this->password_ = std::move(other.password_);

        other.id_ = 0;
        other.age_ = 0;
    }
    return *this;
}

size_t User::getId() const
{
    return this->id_;
}

const String& User::getFirstName() const
{
    return this->firstName_;
}

const String& User::getLastName() const
{
    return this->lastName_;
}

const String& User::getPhoneNumber() const
{
    return this->phoneNumber_;
}

unsigned short User::getAge() const
{
    return this->age_;
}

void User::setFirstName(const String& fName)
{
    this->firstName_ = fName;
}

void User::setLastName(const String& lName)
{
    this->lastName_ = lName;
}

void User::setPhoneNumber(const String& phoneNumber)
{
    this->phoneNumber_ = phoneNumber;
}

void User::setAge(unsigned short age)
{
    this->age_ = age;
}

bool User::checkPassword(const String& pwd) const
{
    return this->password_ == pwd;
}

void User::changePassword(const String& currPwd, const String& newPwd)
{
    if (this->password_ == currPwd)
    {
        this->password_ = newPwd;
    }
}

std::ofstream& User::serialize(std::ofstream& os) const
{
    UserRole role = this->getRole();
    os.write((const char*)&role, sizeof(role));

    os.write((const char*)&this->id_, sizeof(this->id_));

    size_t fNameLen = this->firstName_.size();
    os.write((const char*)&fNameLen, sizeof(fNameLen));
    os.write((const char*)firstName_, fNameLen);

    size_t lNameLen = this->lastName_.size();
    os.write((const char*)&lNameLen, sizeof(lNameLen));
    os.write((const char*)lastName_, lNameLen);

    size_t emailLen = this->phoneNumber_.size();
    os.write((const char*)&emailLen, sizeof(emailLen));
    os.write((const char*)phoneNumber_, emailLen);

    os.write((const char*)&this->age_, sizeof(this->age_));

    size_t pwdLen = this->password_.size();
    os.write((const char*)&pwdLen, sizeof(pwdLen));
    os.write((const char*)password_, pwdLen);

    return os;
}

std::ifstream& User::deserialize(std::ifstream& is)
{
    // Id
    size_t id = 0;
    is.read((char*)&id, sizeof(id));
    this->setId(id);

    // First Name
    this->setFirstName(readString(is));

    // Last Name
    this->setLastName(readString(is));

    // Email
    this->setPhoneNumber(readString(is));

    // Age
    unsigned short age = 0;
    is.read((char*)&age, sizeof(age));
    this->setAge(age);

    // Password
    this->setPassword(readString(is));

    return is;
}

String User::toString()
{
    String userInfo;

    userInfo.push_back('<');
    userInfo += String::toString(this->getId());
    userInfo += "> ";
    userInfo += roleToStr(this->getRole()) + ": ";
    userInfo += firstName_ + " " + lastName_ + " ";
    userInfo += phoneNumber_ + " ";
    userInfo += String::toString(age_);

    return userInfo;
}

void User::setId(size_t id)
{
    this->id_ = id;
}

void User::setPassword(const String& pwd)
{
    this->password_ = pwd;
}

String User::readString(std::ifstream& is) const
{
    size_t len = 0;
    is.read((char*)&len, sizeof(len));

    char* buffer = new char[len + 1];
    is.read(buffer, len);
    buffer[len] = '\0';

    String result = buffer;
    delete[] buffer;

    return result;
}

void User::copyFrom(const User& other)
{
    this->id_ = other.id_;
    this->firstName_ = other.firstName_;
    this->lastName_ = other.lastName_;
    this->phoneNumber_ = other.phoneNumber_;
    this->age_ = other.age_;
    this->password_ = other.password_;
}

String roleToStr(UserRole role)
{
    switch (role)
    {
    case UserRole::Cashier: return "Cashier";
    case UserRole::Manager: return "Manager";
    default: return "None";
    }
}

UserRole strToRole(const String& str)
{
    String lower = str.toLower();

    if (lower == "cashier") return UserRole::Cashier;
    if (lower == "manager") return UserRole::Manager;

    return UserRole::None;
}