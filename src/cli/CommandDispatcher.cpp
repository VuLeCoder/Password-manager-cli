#include "./../../include/cli/CommandDispatcher.h"
#include "./../../include/cli/Console.h"

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
        std::cout
            << "lptv init\n"
            << "lptv unlock\n"
            << "lptv lock\n"
            << "lptv status\n"
            << "lptv list\n"
            << "lptv add <service>\n"
            << "lptv update <service>\n"
            << "lptv get <service>\n"
            << "lptv delete <service>\n";

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
        lptv.list();
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
