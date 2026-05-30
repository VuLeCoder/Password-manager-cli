#pragma once
#include "./Account.h"
#include <vector>

class PasswordManager {
private:
    std::vector<Account> accounts;

public:
    PasswordManager();

    void list(const std::string& categoryFilter = "") const;
    void add(const std::string& service);
    void update(const std::string& service);
    void get(const std::string& service) const;
    void remove(const std::string& service);

private:
    Account* findAccount(const std::string& service);
    const Account* findAccount(const std::string& service) const;
    bool save();
    Account inputAccount(const std::string& service);
};
