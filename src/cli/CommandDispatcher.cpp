#include "./../../include/cli/CommandDispatcher.h"
#include "./../../include/cli/Console.h"
#include "./../../include/cli/CommandParser.h"

#include "./../../include/core/AuthGuard.h"
#include "./../../include/core/PasswordManager.h"
#include "./../../include/core/Vault.h"

#include <iostream>

bool CommandDispatcher::m_isShellMode = false;

// === === private === ===
bool CommandDispatcher::requireUnlock() {
    if(AuthGuard::verify()) {
        return true;
    }

    if (m_isShellMode) {
        Console::printError("Vault is locked.");
        std::cout << "Run: " << Console::BOLD << "lptv unlock" << Console::RESET << "\n\n";
        return false;
    } else {
        std::cout << "Vault is locked. Enter password: ";
        std::string password = Console::getHiddenInput();
        
        std::pair<bool, std::array<uint8_t, 32>> res = Vault::verifyLPTV(password);
        if(res.first) {
            AuthGuard::unlock(res.second);
            return true;
        }

        Console::printError("Invalid password.");
        return false;
    }
}

bool CommandDispatcher::requireService(const Command& cmd) {
    if(cmd.args.empty()) {
        Console::printError("Missing service name.");
        return false;
    }
    return true;
}

// === === public === ===
int CommandDispatcher::execute(const Command& cmd) {
    bool isHelpRequested = cmd.name == "help" || cmd.name.empty() || cmd.name == "-h" || cmd.name == "--help";
    
    if (!isHelpRequested) {
        for (const auto& arg : cmd.args) {
            if (arg == "-h" || arg == "--help") {
                isHelpRequested = true;
                break;
            }
        }
    }

    if (isHelpRequested) {
        std::string targetCmd = "";
        if (cmd.name == "help" && !cmd.args.empty()) {
            targetCmd = cmd.args[0];
        } else if (cmd.name != "help" && cmd.name != "-h" && cmd.name != "--help") {
            targetCmd = cmd.name;
        }

        if (targetCmd == "add") {
            Console::printHeader("Help: add");
            std::cout << "  Usage: lptv add <service>         Add a new account\n"
                      << "         lptv add category <name>   Add a new category\n\n";
        } else if (targetCmd == "list") {
            Console::printHeader("Help: list");
            std::cout << "  Usage: lptv list                  List all accounts\n"
                      << "         lptv list <category>       List accounts in category\n"
                      << "         lptv list category         List all categories\n\n";
        } else if (targetCmd == "get") {
            Console::printHeader("Help: get");
            std::cout << "  Usage: lptv get <service>          Show account details and copy password\n\n";
        } else if (targetCmd == "delete") {
            Console::printHeader("Help: delete");
            std::cout << "  Usage: lptv delete <service>       Remove an account\n"
                      << "         lptv delete category <name> Remove a category\n\n";
        } else if (targetCmd == "update") {
            Console::printHeader("Help: update");
            std::cout << "  Usage: lptv update <service>       Update account details\n\n";
        } else if (targetCmd == "search") {
            Console::printHeader("Help: search");
            std::cout << "  Usage: lptv search <query>         Search accounts by service or username\n\n";
        } else if (targetCmd == "init") {
            Console::printHeader("Help: init");
            std::cout << "  Usage: lptv init                   Initialize a new vault and set master password\n\n";
        } else if (targetCmd == "unlock") {
            Console::printHeader("Help: unlock");
            std::cout << "  Usage: lptv unlock                 Unlock the vault to access accounts\n\n";
        } else if (targetCmd == "lock") {
            Console::printHeader("Help: lock");
            std::cout << "  Usage: lptv lock                   Lock the vault and clear session\n\n";
        } else if (targetCmd == "status") {
            Console::printHeader("Help: status");
            std::cout << "  Usage: lptv status                 Check if vault is initialized and unlocked\n\n";
        } else if (targetCmd == "shell") {
            Console::printHeader("Help: shell");
            std::cout << "  Usage: lptv shell                  Enter interactive shell mode\n\n";
        } else {
            std::cout << "\nUsage: lptv <command>\n\n"
                << "where <command> is one of:\n"
                << "    " << Console::BOLD << "init" << Console::RESET << "       Initialize the vault\n"
                << "    " << Console::BOLD << "unlock" << Console::RESET << "     Unlock the vault\n"
                << "    " << Console::BOLD << "lock" << Console::RESET << "       Lock the vault\n"
                << "    " << Console::BOLD << "status" << Console::RESET << "     Check vault status\n"
                << "    " << Console::BOLD << "shell" << Console::RESET << "      Enter interactive mode\n"
                << "    " << Console::BOLD << "list" << Console::RESET << "       List accounts or categories\n"
                << "    " << Console::BOLD << "search" << Console::RESET << "     Search accounts\n"
                << "    " << Console::BOLD << "add" << Console::RESET << "        Add an account or category\n"
                << "    " << Console::BOLD << "update" << Console::RESET << "     Update an account\n"
                << "    " << Console::BOLD << "get" << Console::RESET << "        Get account details\n"
                << "    " << Console::BOLD << "delete" << Console::RESET << "     Delete an account or category\n\n"
                << "lptv help <command>  search for help on <command>\n"
                << "lptv <command> -h    quick help on <command>\n\n";
        }

        return 0;
    }

    if(cmd.name == "shell") {
        m_isShellMode = true;
        Console::printHeader("Entering Interactive Mode");
        std::cout << Console::GREY << "(type 'exit' or 'quit' to leave)" << Console::RESET << "\n\n";
        std::string input;
        while (true) {
            std::cout << Console::CYAN << "lptv> " << Console::RESET;
            if (!std::getline(std::cin, input)) break;
            if (input == "exit" || input == "quit") break;
            if (input.empty()) continue;

            Command innerCmd = CommandParser::parse(input);
            if (innerCmd.name == "shell") {
                Console::printWarning("Already in interactive mode.");
                continue;
            }
            execute(innerCmd);
        }
        m_isShellMode = false;
        Console::printInfo("Exiting interactive mode.");
        return 0;
    }

    if(cmd.name == "init") {
        if(Vault::exists()) {
            Console::printWarning("Password vault already exists.");
            return 0;
        }

        std::cout << "Create lptv password: ";
        std::string password = Console::getHiddenInput();

        std::cout << "Confirm lptv password: ";
        std::string confirm = Console::getHiddenInput();

        if(password.empty()) {
            Console::printError("Password cannot be empty!");
            return 0;
        }

        if(password == confirm) {
            Vault::initialize();
            Vault::setLPTVPassword(password);
            Console::printSuccess("Password vault initialized.");
            return 0;
        }

        Console::printError("Passwords do not match.");
        return 0;
    }

    if(!Vault::exists()) {
        Console::printError("Password vault not initialized.");
        std::cout << "Run: " << Console::BOLD << "lptv init" << Console::RESET << "\n\n";
        return 0;
    }

    if(cmd.name == "status") {
        Console::printHeader("Vault Status");
        std::cout << "  Initialized : " << (Vault::exists() ? Console::GREEN + "Yes" : Console::RED + "No") << Console::RESET << "\n";
        std::cout << "  Unlocked    : " << (AuthGuard::verify() ? Console::GREEN + "Yes" : Console::RED + "No") << Console::RESET << "\n\n";
        return 0;
    }

    if(cmd.name == "unlock") {
        if(AuthGuard::verify()) {
            Console::printInfo("Password vault already unlocked.");
            return 0;
        }

        std::cout << "Enter password: ";
        std::string password = Console::getHiddenInput();        

        std::pair<bool, std::array<uint8_t, 32>> res = Vault::verifyLPTV(password);
        if(res.first) {
            AuthGuard::unlock(res.second);
            Console::printSuccess("Password vault unlocked.");
            return 0;
        }

        Console::printError("Invalid password.");
        return 0;
    }

    if(cmd.name == "lock") {
        if(AuthGuard::verify()) {
            AuthGuard::lock();
            Console::printSuccess("Password vault locked.");
            return 0;
        }

        Console::printInfo("Password vault already locked.");
        return 0;
    }

    if(cmd.name == "list") {
        if(!requireUnlock()) return 0;

        PasswordManager lptv;
        if (!cmd.args.empty() && cmd.args[0] == "category") {
            lptv.listCategories();
        } else {
            std::string category = cmd.args.empty() ? "" : cmd.args[0];
            lptv.list(category);
        }
        return 0;
    }

    if(cmd.name == "search") {
        if(!requireUnlock()) return 0;

        if(cmd.args.empty()) {
            Console::printError("Missing search query.");
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
        if (cmd.args[0] == "category") {
            if (cmd.args.size() < 2) {
                Console::printError("Missing category name.");
            } else {
                lptv.addCategory(cmd.args[1]);
            }
        } else {
            lptv.add(cmd.args[0]);
        }
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
        if (cmd.args[0] == "category") {
            if (cmd.args.size() < 2) {
                Console::printError("Missing category name.");
            } else {
                lptv.removeCategory(cmd.args[1]);
            }
        } else {
            lptv.remove(cmd.args[0]);
        }
        return 0;
    }

    Console::printError("Unknown command: " + cmd.name);
    std::cout << "Run " << Console::BOLD << "lptv help" << Console::RESET << " for usage.\n\n";
    return 0;
}
