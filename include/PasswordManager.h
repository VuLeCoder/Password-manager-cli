#pragma once
#include <vector>
#include "./Account.h"

class PasswordManager {
private:
    std::vector<Account> accounts;

public:
    PasswordManager();

    void add(const std::string& service);
    void list() const;
    void get(const std::string& service) const;
    void remove(const std::string& service);
};
