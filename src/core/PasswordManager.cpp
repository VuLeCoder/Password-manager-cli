#include "./../../include/core/PasswordManager.h"
#include "./../../include/storage/Storage.h"
#include "./../../include/utils/Constants.h"
#include "./../../include/cli/Console.h"

#include <iostream>
#include <limits>

// === === private === ===
Account* PasswordManager::findAccount(const std::string& service) {
    for(auto& account : accounts) {
        if(account.getService() == service) {
            return &account;
        }
    }
    return nullptr;
}

const Account* PasswordManager::findAccount(const std::string& service) const {
    for(const auto& account : accounts) {
        if(account.getService() == service) {
            return &account;
        }
    }
    return nullptr;
}

bool PasswordManager::save() {
    return Storage::save(accounts, Constants::VAULT_DB);
}

Account PasswordManager::inputAccount(const std::string& service) {
    std::string username;
    std::string password;
    std::string note;

    std::cout << "Username: ";
    std::getline(std::cin, username);

    std::cout << "Password: ";
    std::getline(std::cin, password);

    std::cout << "Note: ";
    std::getline(std::cin, note);

    return Account(
        service,
        username,
        password,
        note
    );
}


// === === public === ===
PasswordManager::PasswordManager() {
    accounts = Storage::load(Constants::VAULT_DB);
}

void PasswordManager::list() const
{
    if(accounts.empty()) {
        std::cout << "No accounts.\n";
        return;
    }

    std::vector<std::string> headers = {"Service", "Username"};
    std::vector<std::vector<std::string>> rows;

    for(const auto& account : accounts) {
        rows.push_back({
            account.getService(),
            account.getUsername()
        });
    }

    Console::printTable(headers, rows);
}

void PasswordManager::add(const std::string& service) {
    Account* existsAcc = findAccount(service);
    if(existsAcc != nullptr) {
        std::cout
            << "Service '"
            << service
            << "' already exists.\n";
        
        std::cout << "Do you want to update it? (y/n): "; 
        char choice; 
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if(choice != 'y' && choice != 'Y') {
            std::cout << "Command cancelled.\n";
            return;
        }
        
        update(service);
        return;
    }
    
    Account acc = inputAccount(service);
    accounts.emplace_back(acc);

    if(save()) {
        std::cout << "Account saved.\n";
        return;
    }
    std::cout << "Failed to save account\n";
}

void PasswordManager::update(const std::string& service) {
    Account* existsAcc = findAccount(service);
    if(existsAcc == nullptr) {
        std::cout << "Account not found.\n";
        return;
    }

    Account acc = inputAccount(service);
    existsAcc->setUsername(acc.getUsername());
    existsAcc->setPassword(acc.getPassword());
    existsAcc->setNote(acc.getNote());

    if(save()) {
        std::cout << "Account updated.\n";
        return;
    }
    std::cout << "Failed to update account\n";
}

void PasswordManager::get(const std::string& service) const {
    const Account* account = findAccount(service);
    if(account == nullptr) {
        std::cout << "Account not found.\n";
        return;
    }

    std::cout << "Service : " << account->getService() << "\n";
    std::cout << "Username: " << account->getUsername() << "\n";
    std::cout << "Password: " << account->getPassword() << "\n";
    std::cout << "Note    : " << account->getNote() << "\n";

    std::cout << "\nCopy password to clipboard? (y/n): ";
    char choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 'y' || choice == 'Y') {
        if (Console::copyToClipboard(account->getPassword())) {
            std::cout << "Password copied to clipboard!\n";
        } else {
            std::cout << "Failed to copy password.\n";
        }
    }
}

void PasswordManager::remove(const std::string& service) {
    for (
        auto it = accounts.begin();
        it != accounts.end();
        ++it
    ) {
        if(it->getService() == service) {
            accounts.erase(it);

            if(save()) {
                std::cout << "Account deleted.\n";
                return;
            }
            std::cout << "Failed to delete account\n";
            return;
        }
    }

    std::cout << "Account not found.\n";
}
