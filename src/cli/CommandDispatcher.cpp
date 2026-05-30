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
        std::cout << "\n === === === === === === === === === === ===\n"
            << "lptv init            - Initialize the vault\n"
            << "lptv unlock          - Unlock the vault\n"
            << "lptv lock            - Lock the vault\n"
            << "lptv status          - Check vault status\n"
            << "lptv shell           - Enter interactive mode\n"
            << "lptv list [cat]      - List services (optional category filter)\n"
            << "lptv search <query>  - Search accounts by service, username, or category\n"
            << "lptv add <svc>       - Add a new account\n"
            << "lptv update <svc>    - Update an account\n"
            << "lptv get <svc>       - Get account details\n"
            << "lptv delete <svc>    - Delete an account\n"
            << "=== === === === === === === === === === ===\n";

        return 0;
    }

    if(cmd.name == "shell") {
        std::cout << "--- Interactive Mode (type 'exit' or 'quit' to leave) ---\n";
        std::string input;
        while (true) {
            std::cout << "lptv> ";
            if (!std::getline(std::cin, input)) break;
            if (input == "exit" || input == "quit") break;
            if (input.empty()) continue;

            Command innerCmd = CommandParser::parse(input);
            if (innerCmd.name == "shell") {
                std::cout << "Already in interactive mode.\n";
                continue;
            }
            execute(innerCmd);
        }
        std::cout << "Exiting interactive mode.\n";
        return 0;
    }

    if(cmd.name == "init") {
        if(Vault::exists()) {
            std::cout << "Password vault already exists.\n";
            return 0;
        }

        std::cout << "Create lptv password: ";
        std::string password = Console::getHiddenInput();

        std::cout << "Confirm lptv password: ";
        std::string confirm = Console::getHiddenInput();

        if(password.empty()) {
            std::cout << "Password cannot be empty!\n";
            return 0;
        }

        if(password == confirm) {
            Vault::initialize();
            Vault::setLPTVPassword(password);
            std::cout << "Password vault initialized.\n";
            return 0;
        }

        std::cout << "Passwords do not match.\n";
        return 0;
    }

    if(!Vault::exists()) {
        std::cout
            << "Password vault not initialized.\n"
            << "Run: lptv init\n";

        return 0;
    }

    if(cmd.name == "status") {
        std::cout << "Vault Status\n";

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

        return 0;
    }

    if(cmd.name == "unlock") {
        if(AuthGuard::verify()) {
            std::cout << "Password vault already unlocked.\n";
            return 0;
        }

        std::cout << "Enter password: ";
        std::string password = Console::getHiddenInput();        

        if(Vault::verifyLPTV(password)) {
            AuthGuard::unlock();
            std::cout << "Password vault unlocked.\n";
            return 0;
        }

        std::cout << "Invalid password.\n";
        return 0;
    }

    if(cmd.name == "lock") {
        if(AuthGuard::verify()) {
            AuthGuard::lock();
            std::cout << "Password vault locked.\n";
            return 0;
        }

        std::cout << "Password vault already locked.\n";
        return 0;
    }

    if(cmd.name == "list") {
        if(!requireUnlock()) return 0;

        PasswordManager lptv;
        std::string category = cmd.args.empty() ? "" : cmd.args[0];
        lptv.list(category);
        return 0;
    }

    if(cmd.name == "search") {
        if(!requireUnlock()) return 0;
        if(cmd.args.empty()) {
            std::cout << "Missing search query.\n";
            return 0;
        }

        PasswordManager lptv;
        lptv.search(cmd.args[0]);
        return 0;
    }

    if(cmd.name == "add") {
        if(!requireUnlock()) return 0;
        if(!requireService(cmd)) return 0;

        PasswordManager lptv;
        lptv.add(cmd.args[0]);
        return 0;
    }

    if(cmd.name == "update") {
        if(!requireUnlock()) return 0;
        if(!requireService(cmd)) return 0;

        PasswordManager lptv;
        lptv.update(cmd.args[0]);
        return 0;
    }

    if(cmd.name == "get") {
        if(!requireUnlock()) return 0;
        if(!requireService(cmd)) return 0;
        
        PasswordManager lptv;
        lptv.get(cmd.args[0]);
        return 0;
    }

    if(cmd.name == "delete") {
        if(!requireUnlock()) return 0;
        if(!requireService(cmd)) return 0;

        PasswordManager lptv;
        lptv.remove(cmd.args[0]);
        return 0;
    }

    std::cout
        << "Unknown command.\n"
        << "Run: lptv help\n";
    return 0;
}
