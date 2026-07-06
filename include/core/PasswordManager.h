#pragma once
#include "Account.h"
#include <vector>

class PasswordManager {
private:
    std::vector<Account> accounts;
    std::vector<SecureString> categories;

public:
    PasswordManager();

    void list(const SecureString& categoryFilter = "") const;
    void listCategories() const;
    void search(const SecureString& query) const;
    void add(const SecureString& service);
    void addCategory(const SecureString& category);
    void removeCategory(const SecureString& category);
    void update(const SecureString& service);
    void get(const SecureString& service, bool isHiddenPassword) const;
    void remove(const SecureString& service);

private:
    Account* findAccount(const SecureString& service);
    const Account* findAccount(const SecureString& service) const;
    void save();
    Account inputAccount(const SecureString& service);
};
