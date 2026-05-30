#include "./../../include/core/Account.h"

Account::Account(
    const std::string& service,
    const std::string& username,
    const std::string& password,
    const std::string& note
) {
    this->service = service;
    this->username = username;
    this->password = password;
    this->note = note;
}

const std::string& Account::getService() const    { return service; }
const std::string& Account::getUsername() const   { return username; }
const std::string& Account::getPassword() const   { return password; }
const std::string& Account::getNote() const       { return note; }

void Account::setService(const std::string& service)    { this->service = service; }
void Account::setUsername(const std::string& username)  { this->username = username; }
void Account::setPassword(const std::string& password)  { this->password = password; }
void Account::setNote(const std::string& note)          { this->note = note; }
