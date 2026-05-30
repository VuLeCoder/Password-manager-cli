#include "./../../include/cli/CommandDispatcher.h"
#include "./../../include/cli/Console.h"
#include "./../../include/cli/CommandParser.h"

#include "./../../include/core/AuthGuard.h"
#include "./../../include/core/PasswordManager.h"
#include "./../../include/core/Vault.h"

#include <iostream>

// === === private === ===
bool CommandDispatcher::requireUnlock() {
    if(!AuthGuard::verify()) {
        std::cout
            << "Vault is locked.\n"
            << "Run: lptv unlock\n";
        return false;
    }
    return true;
}

bool CommandDispatcher::requireService(const Command& cmd) {
    if(cmd.args.empty()) {
        std::cout << "Missing service name.\n";
        return false;
    }
    return true;
}

// === === public === ===
int CommandDispatcher::execute(const Command& cmd) {
    if(cmd.name == "help") {
        std::cout << "\n\n === === === === === === === === === === ===\n"
            << ">  " << "lptv init                - Initialize the vault\n"
            << ">  " << "lptv unlock              - Unlock the vault\n"
            << ">  " << "lptv lock                - Lock the vault\n"
            << ">  " << "lptv status              - Check vault status\n"
            << ">  " << "lptv shell               - Enter interactive mode\n"
            << ">  " << "lptv list [cat]          - List accounts (optional category filter)\n"
            << ">  " << "lptv list category       - List all categories\n"
            << ">  " << "lptv search <query>      - Search accounts\n"
            << ">  " << "lptv add <svc>           - Add a new account\n"
            << ">  " << "lptv add category <cat>  - Add a new category\n"
            << ">  " << "lptv update <svc>        - Update an account\n"
            << ">  " << "lptv get <svc>           - Get account details\n"
            << ">  " << "lptv delete <svc>        - Delete an account\n"
            << ">  " << "lptv delete category <c> - Delete a category\n"
            << "=== === === === === === === === === === ===\n\n\n";

        return 0;
    }

    if(cmd.name == "shell") {
        std::cout << "\n\n--- Interactive Mode (type 'exit' or 'quit' to leave) ---\n";
        std::string input;
        while (true) {
            std::cout << ">  " << "lptv> ";
            if (!std::getline(std::cin, input)) break;
            if (input == "exit" || input == "quit") break;
            if (input.empty()) continue;

            Command innerCmd = CommandParser::parse(input);
            if (innerCmd.name == "shell") {
                std::cout << "Already in interactive mode.\n\n";
                continue;
            }
            execute(innerCmd);
            std::cout << "\n";
        }
        std::cout << "Exiting interactive mode.\n\n\n";
        return 0;
    }

    if(cmd.name == "init") {
        if(Vault::exists()) {
            std::cout << "\nPassword vault already exists.\n\n\n";
            return 0;
        }

        std::cout << "\nCreate lptv password: ";
        std::string password = Console::getHiddenInput();

        std::cout << "Confirm lptv password: ";
        std::string confirm = Console::getHiddenInput();

        if(password.empty()) {
            std::cout << "Password cannot be empty!\n\n\n";
            return 0;
        }

        if(password == confirm) {
            Vault::initialize();
            Vault::setLPTVPassword(password);
            std::cout << "Password vault initialized.\n\n\n";
            return 0;
        }

        std::cout << "Passwords do not match.\n\n\n";
        return 0;
    }

    if(!Vault::exists()) {
        std::cout
            << "\nPassword vault not initialized.\n"
            << "Run: lptv init\n\n\n";

        return 0;
    }

    if(cmd.name == "status") {
        std::cout << "\n === === === === === === === === === === ===\n";
        std::cout << "=== === Vault Status\n";

        std::cout
            << "Initialized : "
            << (Vault::exists() ? "Yes" : "No")
            << "\n";

        std::cout
            << "Unlocked    : "
            << (AuthGuard::verify()
                    ? "Yes"
                    : "No")
            << "\n";

        std::cout << "\n\n";
        return 0;
    }

    if(cmd.name == "unlock") {
        if(AuthGuard::verify()) {
            std::cout << "\n\nPassword vault already unlocked.\n";
            std::cout << "\n\n";
            return 0;
        }

        std::cout << "\n\nEnter password: ";
        std::string password = Console::getHiddenInput();        

        if(Vault::verifyLPTV(password)) {
            AuthGuard::unlock();
            std::cout << "Password vault unlocked.\n";
            std::cout << "\n\n";
            return 0;
        }

        std::cout << "Invalid password.\n";
        std::cout << "\n\n";
        return 0;
    }

    if(cmd.name == "lock") {
        if(AuthGuard::verify()) {
            AuthGuard::lock();
            std::cout << "\n\nPassword vault locked.\n";
            std::cout << "\n\n";
            return 0;
        }

        std::cout << "\n\nPassword vault already locked.\n";
        std::cout << "\n\n";
        return 0;
    }

    if(cmd.name == "list") {
        std::cout << "\n\n";
        if(!requireUnlock()) {
            return 0;
        }

        PasswordManager lptv;
        if (!cmd.args.empty() && cmd.args[0] == "category") {
            lptv.listCategories();
        } else {
            std::string category = cmd.args.empty() ? "" : cmd.args[0];
            lptv.list(category);
        }
        std::cout << "\n\n";
        return 0;
    }

    if(cmd.name == "search") {
        if(!requireUnlock()) {
            std::cout << "\n\n";
            return 0;
        }

        if(cmd.args.empty()) {
            std::cout << "\n\n";
            std::cout << "Missing search query.\n";
            std::cout << "\n\n";
            return 0;
        }

        std::cout << "\n\n";
        PasswordManager lptv;
        lptv.search(cmd.args[0]);
        std::cout << "\n\n";
        return 0;
    }

    if(cmd.name == "add") {
        std::cout << "\n\n";

        if(!requireUnlock()) return 0;
        if(!requireService(cmd)) return 0;

        PasswordManager lptv;
        if (cmd.args[0] == "category") {
            if (cmd.args.size() < 2) {
                std::cout << "Missing category name.\n";
            } else {
                lptv.addCategory(cmd.args[1]);
            }
        } else {
            lptv.add(cmd.args[0]);
        }
        std::cout << "\n\n";
        return 0;
    }

    if(cmd.name == "update") {
        if(!requireUnlock()) {
            std::cout << "\n\n";
            return 0;
        }

        if(!requireService(cmd)) {
            std::cout << "\n\n";
            return 0;
        }

        PasswordManager lptv;
        lptv.update(cmd.args[0]);
        std::cout << "\n\n";
        return 0;
    }

    if(cmd.name == "get") {
        if(!requireUnlock()) {
            std::cout << "\n\n";
            return 0;
        }

        if(!requireService(cmd)) {
            std::cout << "\n\n";
            return 0;
        }
        
        PasswordManager lptv;
        lptv.get(cmd.args[0]);
        std::cout << "\n\n";
        return 0;
    }

    if(cmd.name == "delete") {
        if(!requireUnlock()) {
            std::cout << "\n\n";
            return 0;
        }

        if(!requireService(cmd)) {
            std::cout << "\n\n";
            return 0;
        }

        PasswordManager lptv;
        if (cmd.args[0] == "category") {
            if (cmd.args.size() < 2) {
                std::cout << "Missing category name.\n";
            } else {
                lptv.removeCategory(cmd.args[1]);
            }
        } else {
            lptv.remove(cmd.args[0]);
        }
        std::cout << "\n\n";
        return 0;
    }

    std::cout
        << "Unknown command.\n"
        << "Run: lptv help\n";
    std::cout << "\n\n";
    return 0;
}
