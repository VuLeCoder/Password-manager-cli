#include "./../include/Account.h"

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

std::string Account::getService()    { return service; }
std::string Account::getUsername()   { return username; }
std::string Account::getPassword()   { return password; }
std::string Account::getNote()       { return note; }

void Account::setService(const std::string& service)    { this->service = service; }
void Account::setUsername(const std::string& username)  { this->username = username; }
void Account::setPassword(const std::string& password)  { this->password = password; }
void Account::setNote(const std::string& note)          { this->note = note; }
