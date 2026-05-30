#include "./../../include/core/PasswordManager.h"
#include "./../../include/storage/Storage.h"
#include "./../../include/utils/Constants.h"
#include "./../../include/cli/Console.h"

#include <iostream>
#include <limits>
#include <algorithm>
#include <cctype>

static std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return s;
}

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
    std::string category = "";
    std::string note;

    std::cout << "Username: ";
    std::getline(std::cin, username);

    std::cout << "Password: ";
    std::getline(std::cin, password);

    if (categories.empty()) {
        std::cout << "Category (Enter new): ";
        std::getline(std::cin, category);
        if (!category.empty()) {
            addCategory(category);
        }
    } else {
        std::cout << "Select Category:\n";
        for (size_t i = 0; i < categories.size(); ++i) {
            std::cout << "  " << i + 1 << ". " << categories[i] << "\n";
        }
        std::cout << "  0. [Enter New Category]\n";
        std::cout << "Choice: ";
        
        std::string choiceStr;
        std::getline(std::cin, choiceStr);
        try {
            int choice = std::stoi(choiceStr);
            if (choice > 0 && choice <= (int)categories.size()) {
                category = categories[choice - 1];
            } else {
                std::cout << "Enter new category: ";
                std::getline(std::cin, category);
                if (!category.empty()) {
                    addCategory(category);
                }
            }
        } catch (...) {
            std::cout << "Enter new category: ";
            std::getline(std::cin, category);
            if (!category.empty()) {
                addCategory(category);
            }
        }
    }

    std::cout << "Note: ";
    std::getline(std::cin, note);

    return Account(
        service,
        username,
        password,
        category,
        note
    );
}


// === === public === ===
PasswordManager::PasswordManager() {
    accounts = Storage::load(Constants::VAULT_DB);
    categories = Storage::loadCategories(Constants::CATEGORY_DB);
}

void PasswordManager::listCategories() const {
    if (categories.empty()) {
        std::cout << "No categories found.\n";
        return;
    }

    std::cout << "--- Categories ---\n";
    for (size_t i = 0; i < categories.size(); ++i) {
        std::cout << i + 1 << ". " << categories[i] << "\n";
    }
}

void PasswordManager::addCategory(const std::string& category) {
    for (const auto& cat : categories) {
        if (toLower(cat) == toLower(category)) {
            std::cout << "Category '" << category << "' already exists.\n";
            return;
        }
    }

    categories.push_back(category);
    if (saveCats()) {
        std::cout << "Category '" << category << "' added.\n";
    } else {
        std::cout << "Failed to save category.\n";
    }
}

bool PasswordManager::saveCats() {
    return Storage::saveCategories(categories, Constants::CATEGORY_DB);
}

void PasswordManager::search(const std::string& query) const {
    if (accounts.empty()) {
        std::cout << "No accounts.\n";
        return;
    }

    std::string lQuery = toLower(query);
    std::vector<std::string> headers = {"Service", "Username", "Category"};
    std::vector<std::vector<std::string>> rows;

    for (const auto& account : accounts) {
        if (toLower(account.getService()).find(lQuery) != std::string::npos ||
            toLower(account.getUsername()).find(lQuery) != std::string::npos ||
            toLower(account.getCategory()).find(lQuery) != std::string::npos) 
        {
            rows.push_back({
                account.getService(),
                account.getUsername(),
                account.getCategory()
            });
        }
    }

    if (rows.empty()) {
        std::cout << "No matches found for '" << query << "'.\n";
        return;
    }

    Console::printTable(headers, rows);
}

void PasswordManager::list(const std::string& categoryFilter) const
{
    if(accounts.empty()) {
        std::cout << "No accounts.\n";
        return;
    }

    std::vector<std::string> headers = {"Service", "Username", "Category"};
    std::vector<std::vector<std::string>> rows;

    for(const auto& account : accounts) {
        if (!categoryFilter.empty() && toLower(account.getCategory()) != toLower(categoryFilter)) {
            continue;
        }

        rows.push_back({
            account.getService(),
            account.getUsername(),
            account.getCategory()
        });
    }

    if (rows.empty()) {
        if (!categoryFilter.empty()) {
            std::cout << "No accounts in category '" << categoryFilter << "'.\n";
        } else {
            std::cout << "No accounts.\n";
        }
        return;
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
    existsAcc->setCategory(acc.getCategory());
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
    std::cout << "Category: " << account->getCategory() << "\n";
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
