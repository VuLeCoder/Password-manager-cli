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
    return Storage::save({accounts, categories}, Constants::VAULT_DB);
}

Account PasswordManager::inputAccount(const std::string& service) {
    std::string username;
    std::string password;
    std::string category = "";
    std::string note;

    std::cout << Console::BOLD << "  Username " << Console::RESET << ": ";
    std::getline(std::cin, username);

    std::cout << Console::BOLD << "  Password " << Console::RESET << ": ";
    password = Console::getHiddenInput();

    if (categories.empty()) {
        std::cout << Console::BOLD << "  Category " << Console::RESET << "(Enter new): ";
        std::getline(std::cin, category);
        if (!category.empty()) {
            addCategory(category);
        }
    } else {
        std::cout << Console::BOLD << "  Select Category" << Console::RESET << ":\n";
        for (size_t i = 0; i < categories.size(); ++i) {
            std::cout << "    " << Console::CYAN << i + 1 << ". " << Console::RESET << categories[i] << "\n";
        }
        std::cout << "    " << Console::CYAN << "0. " << Console::RESET << "[Enter New Category]\n";
        std::cout << "  " << Console::BOLD << "Choice" << Console::RESET << ": ";
        
        std::string choiceStr;
        std::getline(std::cin, choiceStr);
        try {
            int choice = std::stoi(choiceStr);
            if (choice > 0 && choice <= (int)categories.size()) {
                category = categories[choice - 1];
            } else {
                std::cout << "  " << Console::BOLD << "Enter new category" << Console::RESET << ": ";
                std::getline(std::cin, category);
                if (!category.empty()) {
                    addCategory(category);
                }
            }
        } catch (...) {
            std::cout << "  " << Console::BOLD << "Enter new category" << Console::RESET << ": ";
            std::getline(std::cin, category);
            if (!category.empty()) {
                addCategory(category);
            }
        }
    }

    std::cout << Console::BOLD << "  Note     " << Console::RESET << ": ";
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
    VaultData data = Storage::load(Constants::VAULT_DB);
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
        std::cout << "  " << Console::CYAN << i + 1 << ". " << Console::RESET << categories[i] << "\n";
    }
    std::cout << std::endl;
}

void PasswordManager::addCategory(const std::string& category) {
    for (const auto& cat : categories) {
        if (toLower(cat) == toLower(category)) {
            Console::printWarning("Category '" + category + "' already exists.");
            return;
        }
    }

    categories.push_back(category);
    if (save()) {
        Console::printSuccess("Category '" + category + "' added.");
    } else {
        Console::printError("Failed to save category.");
    }
}

void PasswordManager::removeCategory(const std::string& category) {
    auto it = std::find_if(categories.begin(), categories.end(), [&](const std::string& cat) {
        return toLower(cat) == toLower(category);
    });

    if (it != categories.end()) {
        categories.erase(it);
        if (save()) {
            Console::printSuccess("Category '" + category + "' removed.");
        } else {
            Console::printError("Failed to save categories.");
        }
    } else {
        Console::printError("Category '" + category + "' not found.");
    }
}

void PasswordManager::search(const std::string& query) const {
    if (accounts.empty()) {
        Console::printInfo("No accounts found.");
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
        Console::printInfo("No matches found for '" + query + "'.");
        return;
    }

    Console::printTable(headers, rows);
}

void PasswordManager::list(const std::string& categoryFilter) const
{
    if(accounts.empty()) {
        Console::printInfo("No accounts found.");
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
            Console::printInfo("No accounts in category '" + categoryFilter + "'.");
        } else {
            Console::printInfo("No accounts found.");
        }
        return;
    }

    Console::printTable(headers, rows);
}

void PasswordManager::add(const std::string& service) {
    Account* existsAcc = findAccount(service);
    if(existsAcc != nullptr) {
        Console::printWarning("Service '" + service + "' already exists.");
        
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

    if(save()) {
        Console::printSuccess("Account saved.");
        return;
    }
    Console::printError("Failed to save account.");
}

void PasswordManager::update(const std::string& service) {
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

    if(save()) {
        Console::printSuccess("Account updated.");
        return;
    }
    Console::printError("Failed to update account.");
}

void PasswordManager::get(const std::string& service) const {
    const Account* account = findAccount(service);
    if(account == nullptr) {
        Console::printError("Account not found.");
        return;
    }

    Console::printHeader("Account Details: " + account->getService());
    std::cout << "  " << Console::BOLD << "Service " << Console::RESET << " : " << account->getService() << "\n";
    std::cout << "  " << Console::BOLD << "Username" << Console::RESET << " : " << account->getUsername() << "\n";

    std::cout << "  " << Console::BOLD << "Password" << Console::RESET << " : ";
    Console::printHiddenPassword(account->getPassword());
    std::cout << "\n";
    
    std::cout << "  " << Console::BOLD << "Category" << Console::RESET << " : " << account->getCategory() << "\n";
    std::cout << "  " << Console::BOLD << "Note    " << Console::RESET << " : " << account->getNote() << "\n";

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

void PasswordManager::remove(const std::string& service) {
    for (
        auto it = accounts.begin();
        it != accounts.end();
        ++it
    ) {
        if(it->getService() == service) {
            accounts.erase(it);

            if(save()) {
                Console::printSuccess("Account deleted.");
                return;
            }
            Console::printError("Failed to delete account.");
            return;
        }
    }

    Console::printError("Account not found.");
}
