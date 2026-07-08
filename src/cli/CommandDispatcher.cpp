#include "cli/CommandDispatcher.h"
#include "cli/Console.h"
#include "cli/CommandParser.h"
#include "cli/modules/HelpPrinter.h"

#include "core/AuthGuard.h"
#include "core/PasswordManager.h"
#include "core/Vault.h"

#include <string/SecureString.h>
#include <iostream>
#include <string_view>
#include <unordered_map>

// === === helper === ===
// === === === === === ===
struct HelpInfo {
    std::string title;
    std::string usage;
};
// === === === === === ===

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
        SecureString password;
        Console::readSecureHiddenInput(password);
        
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

bool CommandDispatcher::isHelp(const Command& cmd) {
    if(cmd.name == "--help" || cmd.name == "-h") {
        return true;
    }

    for(const auto& arg : cmd.args) {
        if(arg == "-h" || arg == "--help") {
            return true;
        }
    }
    return false;
}

int CommandDispatcher::handleHelp(std::string_view cmdName) {
    if(cmdName == "--help" || cmdName == "-h") {
        HelpPrinter::printHelp("");
        return 0;
    }

    HelpPrinter::printHelp(cmdName);
    return 0;
}

int CommandDispatcher::handleInit(const Command& cmd) {
    if(Vault::exists()) {
        Console::printWarning("Password vault already exists.");
        return 0;
    }

    std::cout << "Create lptv password: ";
    SecureString password;
    Console::readSecureHiddenInput(password);

    std::cout << "Confirm lptv password: ";
    SecureString confirm;
    Console::readSecureHiddenInput(confirm);

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

int CommandDispatcher::handleShell(const Command& cmd) {
    std::cout << "Enter password: ";
    SecureString password;
    Console::readSecureHiddenInput(password);        

    std::pair<bool, std::array<uint8_t, 32>> res = Vault::verifyLPTV(password);
    if(!res.first) {
        Console::printError("Invalid password.");
        return 0;
    }
    
    AuthGuard::unlock(res.second);

    m_isShellMode = true;
    Console::printHeader("Entering Interactive Mode");
    std::cout << Console::GREY << "(type 'exit' or 'quit' to leave)" << Console::RESET << "\n\n";

    SecureString input;
    while (true) {
        std::cout << Console::CYAN << "lptv> " << Console::RESET;
        if (!Console::readSecureInput(input)) break;
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

int CommandDispatcher::handleStatus(const Command& cmd) {
    Console::printHeader("Vault Status");
    std::cout << "  Initialized : " << (Vault::exists() ? Console::GREEN + "Yes" : Console::RED + "No") << Console::RESET << "\n";
    std::cout << "  Unlocked    : " << (AuthGuard::verify() ? Console::GREEN + "Yes" : Console::RED + "No") << Console::RESET << "\n\n";
    return 0;
}

int CommandDispatcher::handleList(const Command& cmd) {
    if(!requireUnlock()) return 0;

    PasswordManager lptv;
    if (!cmd.args.empty() && cmd.args[0] == "category") {
        lptv.listCategories();
    } else {
        SecureString category = cmd.args.empty() ? "" : cmd.args[0];
        lptv.list(category);
    }
    return 0;
}

int CommandDispatcher::handleSearch(const Command& cmd) {
    if(!requireUnlock()) return 0;

    if(cmd.args.empty()) {
        Console::printError("Missing search query.");
        return 0;
    }

    PasswordManager lptv;
    lptv.search(cmd.args[0]);
    return 0;
}

int CommandDispatcher::handleAdd(const Command& cmd) {
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

int CommandDispatcher::handleUpdate(const Command& cmd) {
    if(!requireUnlock()) return 0;
    if(!requireService(cmd)) return 0;

    PasswordManager lptv;
    lptv.update(cmd.args[0]);
    return 0;
}

int CommandDispatcher::handleGet(const Command& cmd) {
    if(!requireUnlock()) return 0;
    if(!requireService(cmd)) return 0;

    bool isHiddenPassword = true;
    if(cmd.args.size() > 1) {
        isHiddenPassword = !(cmd.args[1] == "--show");
    }
    
    PasswordManager lptv;
    lptv.get(cmd.args[0], isHiddenPassword);
    return 0;
}

int CommandDispatcher::handleDelete(const Command& cmd) {
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

// === === public === ===
int CommandDispatcher::execute(const Command& cmd) {
    for(const auto& ss : cmd.args) {
        std::cout << (ss.view() == "-h");
    }
    std::cout << "\n\n\n";

    if(isHelp(cmd)) {
        return handleHelp(cmd.name);
    }

    if(cmd.name == "init") {
        return handleInit(cmd);
    }

    if(!Vault::exists()) {
        Console::printError("Password vault not initialized.");
        std::cout << "Run: " << Console::BOLD << "lptv init" << Console::RESET << "\n\n";
        return 0;
    }

    using HandlerFunc = int(*)(const Command&);
    static const std::unordered_map<std::string, HandlerFunc> handlers = {
        {"shell", handleShell},
        {"status", handleStatus},
        {"list", handleList},
        {"search", handleSearch},
        {"add", handleAdd},
        {"update", handleUpdate},
        {"get", handleGet},
        {"delete", handleDelete}
    };

    auto it = handlers.find(cmd.name);
    if (it != handlers.end()) {
        return it->second(cmd);
    }

    Console::printError("Unknown command: " + cmd.name);
    std::cout << "Run " << Console::BOLD << "lptv help" << Console::RESET << " for usage.\n\n";
    return 0;
}
