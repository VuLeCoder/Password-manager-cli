#include "./../../include/core/Account.h"

Account::Account(
    const SecureString& service,
    const SecureString& username,
    const SecureString& password,
    const SecureString& category,
    const SecureString& note
) {
    this->service = service;
    this->username = username;
    this->password = password;
    this->category = category;
    this->note = note;
}

Account::Account(const Account& other) {
    this->service = other.service;
    this->username = other.username;
    this->password = other.password;
    this->category = other.category;
    this->note = other.note;
}

const SecureString& Account::getService() const    { return service; }
const SecureString& Account::getUsername() const   { return username; }
const SecureString& Account::getPassword() const   { return password; }
const SecureString& Account::getCategory() const   { return category; }
const SecureString& Account::getNote() const       { return note; }

void Account::setService(const SecureString& service)    { this->service = service; }
void Account::setUsername(const SecureString& username)  { this->username = username; }
void Account::setPassword(const SecureString& password)  { this->password = password; }
void Account::setCategory(const SecureString& category)  { this->category = category; }
void Account::setNote(const SecureString& note)          { this->note = note; }
