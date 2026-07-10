#include "cli/Command.h"
#include "cli/CommandParser.h"
#include "cli/CommandDispatcher.h"
#include "cli/modules/HelpPrinter.h"
#include "cli/Console.h"

#include "core/AuthGuard.h"
#include "core/PasswordManager.h"
#include "core/Vault.h"

#include "exception/CommandException.h"
#include "utils/Security.h"

#include <unordered_map>
#include <chrono>
#include <thread>

// === === helper === ===
// === === === === === ===
struct HelpInfo {
    std::string title;
    std::string usage;
};

bool CommandCheck::isList(std::string_view sv) {
    return sv == cmd::LIST || sv == "ls";
}

bool CommandCheck::isGenerate(std::string_view sv) {
    return sv == cmd::GENERATE || sv == "gen";
}

bool CommandCheck::isChangePassword(std::string_view sv) {
    return sv == cmd::CHANGE_PASS || sv == "cpw";
}

bool CommandCheck::isHelp(std::string_view sv) {
    return sv == "--help" || sv == "-h";
}

bool CommandCheck::isCategory(std::string_view sv) {
    return sv == "--category" || sv == "-c";
}

bool CommandCheck::isNoUpper(std::string_view sv) {
    return sv == "--no-upper" || sv == "-u";
}

bool CommandCheck::isNoLower(std::string_view sv) {
    return sv == "--no-lower" || sv == "-l";
}

bool CommandCheck::isNoDigits(std::string_view sv) {
    return sv == "--no-digits" || sv == "-d";
}

bool CommandCheck::isNoSpecial(std::string_view sv) {
    return sv == "--no-special" || sv == "-s";
}
// === === === === === ===

// === === private === ===
void CommandDispatcher::requireUnlock() {
    if(AuthGuard::verify()) {
        return;
    }

    std::cout << "Vault is locked.\n";
    std::cout << Console::BOLD << "  Enter password" << Console::RESET << ": ";
    SecureString password;
    Console::readSecureHiddenInput(password);
    
    std::pair<bool, std::array<uint8_t, 32>> res = Vault::verifyLPTV(password);
    if(!res.first) {
        throw CommandException(CommandCode::Error, "Invalid password.");
    }

    AuthGuard::unlock(res.second);
}

bool CommandDispatcher::isHelp(const Command& cmd) {
    if(CommandCheck::isHelp(cmd.name)) {
        return true;
    }

    for(const auto& arg : cmd.args) {
        if(CommandCheck::isHelp(arg.view())) {
            return true;
        }
    }
    return false;
}

// === handler
void CommandDispatcher::handleHelp(std::string_view cmdName) {
    if(CommandCheck::isHelp(cmdName)) {
        HelpPrinter::printHelp("");
        return;
    }

    HelpPrinter::printHelp(cmdName);
}

void CommandDispatcher::handleInit(const Command& cmd) {
    if(Vault::exists()) {
        Console::printWarning("Password vault already exists.");
        return;
    }

    std::cout << Console::BOLD << "  Create lptv password" << Console::RESET << ": ";
    SecureString password;
    Console::readSecureHiddenInput(password);

    std::cout << Console::BOLD << "  Confirm lptv password" << Console::RESET << ": ";
    SecureString confirm;
    Console::readSecureHiddenInput(confirm);

    if(password.empty()) {
        throw CommandException(CommandCode::Error, "Password cannot be empty!");
    }

    if(!password.equals(confirm)) {
        throw CommandException(CommandCode::Error, "Passwords do not match.");
    }

    Vault::initialize();
    Vault::setLPTVPassword(password);
    Console::printSuccess("Password vault initialized.");
}

void CommandDispatcher::handleShell(const Command& cmd) {
    std::cout << Console::BOLD << "  Enter password" << Console::RESET << ": ";
    SecureString password;
    Console::readSecureHiddenInput(password);

    std::pair<bool, std::array<uint8_t, 32>> res = Vault::verifyLPTV(password);
    if(!res.first) {
        throw CommandException(CommandCode::Error, "Invalid password.");
    }
    
    AuthGuard::unlock(res.second);

    Console::isShell = true;
    Console::shellEndTime = std::chrono::steady_clock::now() + std::chrono::minutes(1);

    Console::printHeader("Entering Interactive Mode");
    std::cout << Console::GREY << "(type 'exit' or 'quit' to leave)" << Console::RESET << "\n\n";

    SecureString input;
    while(true) {
        if (std::chrono::steady_clock::now() >= Console::shellEndTime) {
            Console::printWarning("Shell session timeout (1 minutes elapsed). Exiting...");

            std::this_thread::sleep_for(std::chrono::seconds(3));
            Console::clear();
            break;
        }

        std::cout << Console::CYAN << "lptv> " << Console::RESET;
        if(!Console::readSecureInput(input)) break;
        if(input == "exit" || input == "quit") break;
        if(input.empty()) continue;

        Command innerCmd = CommandParser::parse(input);
        if(innerCmd.name == cmd::SHELL) {
            Console::printWarning("Already in interactive mode.");
            continue;
        }
        execute(innerCmd);
    }

    Console::isShell = false;
    Console::printInfo("Exiting interactive mode.");
}

void CommandDispatcher::handleStatus(const Command& cmd) {
    Console::printHeader("Vault Status");
    std::cout << "  Initialized : " << (Vault::exists() ? Console::GREEN + "Yes" : Console::RED + "No") << Console::RESET << "\n";
    std::cout << "  Unlocked    : " << (AuthGuard::verify() ? Console::GREEN + "Yes" : Console::RED + "No") << Console::RESET << "\n\n";
}

void CommandDispatcher::handleList(const Command& cmd) {
    requireUnlock();
    PasswordManager lptv;

    if(cmd.args.empty()) {
        SecureString category = cmd.args.empty() ? "" : cmd.args[0];
        lptv.list(category);
        return;
    }

    if(CommandCheck::isCategory(cmd.args.front().view())) {
        lptv.listCategories();
        return;
    }
}

void CommandDispatcher::handleSearch(const Command& cmd) {
    requireUnlock();
    PasswordManager lptv;
    lptv.search(cmd.args[0]);
}

void CommandDispatcher::handleAdd(const Command& cmd) {
    requireUnlock();
    PasswordManager lptv;

    if(CommandCheck::isCategory(cmd.args.front().view())) {
        lptv.addCategory(cmd.args[1]);
        return;
    }
    lptv.add(cmd.args[0]);
}

void CommandDispatcher::handleUpdate(const Command& cmd) {
    requireUnlock();
    PasswordManager lptv;

    if(CommandCheck::isCategory(cmd.args.front().view())) {
        lptv.updateCategory(cmd.args[1]);
        return;
    }
    lptv.update(cmd.args[0]);
}

void CommandDispatcher::handleGet(const Command& cmd) {
    requireUnlock();

    bool isHiddenPassword = !(cmd.args.back() == "--show");
    PasswordManager lptv;
    lptv.get(cmd.args[0], isHiddenPassword);
}

void CommandDispatcher::handleDelete(const Command& cmd) {
    requireUnlock();
    PasswordManager lptv;

    if(CommandCheck::isCategory(cmd.args.front().view())) {
        lptv.removeCategory(cmd.args[1]);
        return;
    }
    lptv.remove(cmd.args[0]);
}

void CommandDispatcher::handleGenerate(const Command& cmd) {
    size_t length = 16;
    bool useUpper = true;
    bool useLower = true;
    bool useDigits = true;
    bool useSpecial = true;

    for(const auto& arg : cmd.args) {
        if(CommandCheck::isNoUpper(arg.view())) {
            useUpper = false;
        } else if(CommandCheck::isNoLower(arg.view())) {
            useLower = false;
        } else if(CommandCheck::isNoDigits(arg.view())) {
            useDigits = false;
        } else if(CommandCheck::isNoSpecial(arg.view())) {
            useSpecial = false;
        } else {
            try {
                int len = std::stoi(std::string(arg.view()));
                if (len > 0) {
                    length = static_cast<size_t>(len);
                } else {
                    throw CommandException(CommandCode::Error, "Password length must be greater than 0.");
                }
            } catch (...) {
                throw CommandException(CommandCode::Error, "Invalid argument: " + std::string(arg.view()));
            }
        }
    }

    try {
        SecureString password = Security::generatePassword(length, useUpper, useLower, useDigits, useSpecial);
        std::cout << "\n  Generated Password : " << Console::GREEN << password.view() << Console::RESET << "\n\n";
        if (Console::copyToClipboard(password)) {
            Console::printSuccess("Password copied to clipboard! (will be cleared automatically after 30 seconds)");
        } else {
            Console::printWarning("Failed to copy password to clipboard.");
        }
    } catch (const std::exception& e) {
        throw CommandException(CommandCode::Error, e.what());
    }
}

void CommandDispatcher::handleChangePassword(const Command& cmd) {
    requireUnlock();
    PasswordManager lptv;

    std::cout << Console::BOLD << "  Enter new master password" << Console::RESET << ": ";
    SecureString password;
    Console::readSecureHiddenInput(password);

    std::cout << Console::BOLD << "  Confirm new master password" << Console::RESET << ": ";
    SecureString confirm;
    Console::readSecureHiddenInput(confirm);

    if (password.empty()) {
        throw CommandException(CommandCode::Error, "Password cannot be empty!");
    }

    if (!password.equals(confirm)) {
        throw CommandException(CommandCode::Error, "Passwords do not match.");
    }

    lptv.changeMasterPassword(password);
    Console::printSuccess("Master password changed successfully.");
}

void CommandDispatcher::handleClear(const Command& cmd) {
    Console::clear();
    if(Console::isShell) {
        std::cout << "\n";
        Console::printHeader("Entering Interactive Mode");
        std::cout << Console::GREY << "(type 'exit' or 'quit' to leave)" << Console::RESET << "\n\n";
    }
}
// ===
// === === === === === ===

// === === public === ===
void CommandDispatcher::execute(const Command& cmd) {
    if (cmd.name.empty()) {
        handleHelp("");
        return;
    }

    if(isHelp(cmd)) {
        handleHelp(cmd.name);
        return;
    }

    if(cmd.name == cmd::INIT) {
        handleInit(cmd);
        return;
    }

    if(!Vault::exists()) {
        throw PasswordManagerException("Password vault not initialized.");
    }

    using HandlerFunc = void(*)(const Command&);
    static const std::unordered_map<std::string_view, HandlerFunc> handlers = {
        { cmd::STATUS,      handleStatus        },
        { cmd::SHELL,       handleShell         },
        { cmd::LIST,        handleList          },
        { cmd::SEARCH,      handleSearch        },
        { cmd::ADD,         handleAdd           },
        { cmd::UPDATE,      handleUpdate        },
        { cmd::GET,         handleGet           },
        { cmd::DELETE,      handleDelete        },
        { cmd::GENERATE,    handleGenerate      },
        { cmd::CHANGE_PASS, handleChangePassword },
        { cmd::CLEAR, handleClear }
    };

    auto it = handlers.find(cmd.name);
    if (it != handlers.end()) {
        it->second(cmd);
    } else {
        throw CommandException(
            CommandCode::Unknown,
            "Unknown command: " + cmd.name
        );
    }
}
