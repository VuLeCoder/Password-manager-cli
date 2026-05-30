#include "./../include/PasswordManager.h"
#include "./../include/Storage.h"
#include <iostream>

constexpr const char* DB_PATH = "data/vault.db";

PasswordManager::PasswordManager() {
    accounts = Storage::load(DB_PATH);
}

void PasswordManager::add(const std::string& service) {
    std::string username;
    std::string password;
    std::string note;

    std::cout << "Username: ";
    std::getline(std::cin, username);

    std::cout << "Password: ";
    std::getline(std::cin, password);

    std::cout << "Note: ";
    std::getline(std::cin, note);

    accounts.emplace_back(
        service,
        username,
        password,
        note
    );

    Storage::save(accounts, DB_PATH);
    std::cout << "Saved.\n";
}

void PasswordManager::list() const
{
    if(accounts.empty()) {
        std::cout << "No accounts.\n";
        return;
    }

    for(const auto& account : accounts) {
        std::cout
            << "- "
            << account.getService()
            << "\n";
    }
}

void PasswordManager::get(const std::string& service) const {
    for(const auto& account : accounts) {
        if(account.getService() == service) {
            
            std::cout
                << "Service : "
                << account.getService()
                << "\n";

            std::cout
                << "Username: "
                << account.getUsername()
                << "\n";

            std::cout
                << "Password: "
                << account.getPassword()
                << "\n";

            std::cout
                << "Note    : "
                << account.getNote()
                << "\n";

            return;
        }
    }

    std::cout << "Account not found.\n";
}

void PasswordManager::remove(const std::string& service) {
    for (
        auto it = accounts.begin();
        it != accounts.end();
        ++it
    ) {
        if(it->getService() == service) {
            accounts.erase(it);

            Storage::save(
                accounts,
                DB_PATH
            );

            std::cout << "Deleted.\n";
            return;
        }
    }

    std::cout << "Account not found.\n";
}
