#include "./../../include/core/PasswordManager.h"
#include "./../../include/storage/BinaryStorage.h"
#include "./../../include/utils/Constants.h"
#include "./../../include/cli/Console.h"
#include "./../../include/exception/StorageException.h"

#include "string/SecureString.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <cctype>

static SecureString toLower(SecureString s) {
    for(size_t i=0; i<s.size(); ++i) {
        s[i] = static_cast<char>(
            std::tolower(static_cast<unsigned char>(s[i]))
        );
    }
    return s;
}

// === === private === ===
Account* PasswordManager::findAccount(const SecureString& service) {
    for(auto& account : accounts) {
        if(account.getService() == service) {
            return &account;
        }
    }
    return nullptr;
}

const Account* PasswordManager::findAccount(const SecureString& service) const {
    for(const auto& account : accounts) {
        if(account.getService() == service) {
            return &account;
        }
    }
    return nullptr;
}

void PasswordManager::save() {
    BinaryStorage::save({accounts, categories}, Constants::VAULT_DB);
}

Account PasswordManager::inputAccount(const SecureString& service) {
    SecureString username;
    SecureString password;
    SecureString category = "";
    SecureString note;

    std::cout << Console::BOLD << "  Username " << Console::RESET << ": ";
    Console::readSecureInput(username);

    std::cout << Console::BOLD << "  Password " << Console::RESET << ": ";
    Console::readSecureHiddenInput(password);

    if (categories.empty()) {
        std::cout << Console::BOLD << "  Category " << Console::RESET << "(Enter new): ";
        Console::readSecureInput(category);

        if(!category.empty()) {
            addCategory(category);
        }
    } else {
        std::cout << Console::BOLD << "  Select Category" << Console::RESET << ":\n";
        for (size_t i = 0; i < categories.size(); ++i) {
            std::cout << "    " << Console::CYAN << i + 1 << ". " << Console::RESET << categories[i].view() << "\n";
        }
        std::cout << "    " << Console::CYAN << "0. " << Console::RESET << "[Enter New Category]\n";
        std::cout << "  " << Console::BOLD << "Choice" << Console::RESET << ": ";
        
        std::string choiceStr;
        std::getline(std::cin, choiceStr);
        try {
            int choice = std::stoi(choiceStr);
            if(choice > 0 && choice <= (int)categories.size()) {
                category = categories[choice - 1];
            } else {
                std::cout << "  " << Console::BOLD << "Enter new category" << Console::RESET << ": ";
                Console::readSecureInput(category);

                if(!category.empty()) {
                    addCategory(category);
                }
            }
        } catch (...) {
            std::cout << "  " << Console::BOLD << "Enter new category" << Console::RESET << ": ";
            Console::readSecureInput(category);
            
            if(!category.empty()) {
                addCategory(category);
            }
        }
    }

    std::cout << Console::BOLD << "  Note     " << Console::RESET << ": ";
    Console::readSecureInput(note);

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
    VaultData data = BinaryStorage::load(Constants::VAULT_DB);
    accounts = data.accounts;
    categories = data.categories;
}

void PasswordManager::listCategories() const {
    if (categories.empty()) {
        Console::printInfo("No categories found.");
        return;
    }

    Console::printHeader("Categories");
    for (size_t i = 0; i < categories.size(); ++i) {
        std::cout << "  " << Console::CYAN << i + 1 << ". " << Console::RESET << categories[i].view() << "\n";
    }
    std::cout << std::endl;
}

void PasswordManager::addCategory(const SecureString& category) {
    for (const auto& cat : categories) {
        if (toLower(cat) == toLower(category)) {

            SecureString msg("Category '");
            msg += category; msg += "' already exists.";
            Console::printWarning(msg.view());
            return;
        }
    }

    categories.push_back(category);
    try {
        save();
    } catch(const StorageException& e) {
        Console::printError("Failed to save category.");
    }

    SecureString msg("Category '");
    msg += category; msg += "' added.";
    Console::printSuccess(msg.view());
}

void PasswordManager::removeCategory(const SecureString& category) {
    auto it = std::find_if(categories.begin(), categories.end(), [&](const SecureString& cat) {
        return toLower(cat) == toLower(category);
    });

    if (it != categories.end()) {
        categories.erase(it);
        try {
            save();
        } catch(const StorageException& e) {
            Console::printError("Failed to save category.");
        }

        SecureString msg("Category '");
        msg += category; msg += "' removed.";
        Console::printSuccess(msg.view());

    } else {
        SecureString msg("Category '");
        msg += category; msg += "' not found.";
        Console::printSuccess(msg.view());
    }
}

void PasswordManager::search(const SecureString& query) const {
    if (accounts.empty()) {
        Console::printInfo("No accounts found.");
        return;
    }

    SecureString lQuery = toLower(query);
    std::vector<std::string> headers = {"Service", "Username", "Category"};
    std::vector<std::vector<SecureString>> rows;

    for (const auto& account : accounts) {
        if (toLower(account.getService()).find(lQuery) ||
            toLower(account.getUsername()).find(lQuery) ||
            toLower(account.getCategory()).find(lQuery)) 
        {
            rows.push_back({
                account.getService(),
                account.getUsername(),
                account.getCategory()
            });
        }
    }

    if (rows.empty()) {
        SecureString msg("No matches found for '");
        msg += query; msg += "'.";
        Console::printInfo(msg.view());
        
        return;
    }

    Console::printTable(headers, rows);
}

void PasswordManager::list(const SecureString& categoryFilter) const
{
    if(accounts.empty()) {
        Console::printInfo("No accounts found.");
        return;
    }

    std::vector<std::string> headers = {"Service", "Username", "Category"};
    std::vector<std::vector<SecureString>> rows;

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
            SecureString msg("No accounts in category '");
            msg += categoryFilter; msg += "'.";
            Console::printInfo(msg.view());
        } else {
            Console::printInfo("No accounts found.");
        }
        return;
    }

    Console::printTable(headers, rows);
}

void PasswordManager::add(const SecureString& service) {
    Account* existsAcc = findAccount(service);
    if(existsAcc != nullptr) {
        SecureString msg("Service '");
        msg += service; msg += "' already exists.";
        Console::printWarning(msg.view());
        
        std::cout << "Do you want to update it? (y/n): "; 
        char choice; 
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if(choice != 'y' && choice != 'Y') {
            Console::printInfo("Command cancelled.");
            return;
        }
        
        update(service);
        return;
    }
    
    Account acc = inputAccount(service);
    accounts.emplace_back(acc);

    try {
        save();
    } catch(const StorageException& e) {
        Console::printError("Failed to save account.");
    }
    Console::printSuccess("Account saved.");
}

void PasswordManager::update(const SecureString& service) {
    Account* existsAcc = findAccount(service);
    if(existsAcc == nullptr) {
        Console::printError("Account not found.");
        return;
    }

    Account acc = inputAccount(service);
    existsAcc->setUsername(acc.getUsername());
    existsAcc->setPassword(acc.getPassword());
    existsAcc->setCategory(acc.getCategory());
    existsAcc->setNote(acc.getNote());

    try {
        save();
    } catch(const StorageException& e) {
        Console::printError("Failed to update account.");
    }
    Console::printSuccess("Account updated.");
}

void PasswordManager::get(const SecureString& service, bool isHiddenPassword) const {
    const Account* account = findAccount(service);
    if(account == nullptr) {
        Console::printError("Account not found.");
        return;
    }

    SecureString msg("Account Details: ");
    msg += account->getService();
    Console::printHeader(msg.view());

    std::cout << "  " << Console::BOLD << "Service " << Console::RESET << " : " << account->getService().view() << "\n";
    std::cout << "  " << Console::BOLD << "Username" << Console::RESET << " : " << account->getUsername().view() << "\n";

    std::cout << "  " << Console::BOLD << "Password" << Console::RESET << " : ";
    if(isHiddenPassword) Console::printHiddenPassword(account->getPassword());
    else std::cout << account->getPassword().view();
    std::cout << "\n";
    
    std::cout << "  " << Console::BOLD << "Category" << Console::RESET << " : " << account->getCategory().view() << "\n";
    std::cout << "  " << Console::BOLD << "Note    " << Console::RESET << " : " << account->getNote().view() << "\n";

    std::cout << "\nCopy password to clipboard? (y/n): ";
    char choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 'y' || choice == 'Y') {
        if (Console::copyToClipboard(account->getPassword())) {
            Console::printSuccess("Password copied to clipboard!");
        } else {
            Console::printError("Failed to copy password.");
        }
    }
}

void PasswordManager::remove(const SecureString& service) {
    for (
        auto it = accounts.begin();
        it != accounts.end();
        ++it
    ) {
        if(it->getService() == service) {
            accounts.erase(it);
            try {
                save();
            } catch(const StorageException& e) {
                Console::printError("Failed to delete account.");
            }
            Console::printSuccess("Account deleted.");
            return;
        }
    }

    Console::printError("Account not found.");
}
