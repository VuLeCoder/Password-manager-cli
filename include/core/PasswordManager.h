#pragma once
#include "./Account.h"
#include <vector>

class PasswordManager {
private:
    std::vector<Account> accounts;
    std::vector<std::string> categories;

public:
    PasswordManager();

    void list(const std::string& categoryFilter = "") const;
    void listCategories() const;
    void search(const std::string& query) const;
    void add(const std::string& service);
    void addCategory(const std::string& category);
    void removeCategory(const std::string& category);
    void update(const std::string& service);
    void get(const std::string& service, bool isHiddenPassword) const;
    void remove(const std::string& service);

private:
    Account* findAccount(const std::string& service);
    const Account* findAccount(const std::string& service) const;
    bool save();
    Account inputAccount(const std::string& service);
};
