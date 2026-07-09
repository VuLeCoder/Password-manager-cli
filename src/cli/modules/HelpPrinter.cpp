#include "cli/modules/HelpPrinter.h"
#include "cli/Console.h"
#include "exception/CommandException.h"

#include <iomanip>
#include <unordered_map>

namespace {
    using namespace std::string_view_literals;

    struct HelpInfo {
        std::string_view description;
        std::string_view title;
        std::string_view usage;
        bool showInGeneral = true;
    };

    const std::unordered_map<std::string_view, HelpInfo> HELP = {
        {"add", {
            "Add an account or category",
            "Help: add",
            "  Usage: lptv add <service>         Add a new account\n"
                   "         lptv add category <name>   Add a new category\n\n"
        }},

        {"list", {
            "List accounts or categories",
            "Help: list",
            "  Usage: lptv list                  List all accounts\n"
                   "         lptv list <category>       List accounts in category\n"
                   "         lptv list category         List all categories\n\n"
        }},

        {"get", {
            "Get account details",
            "Help: get",
            "  Usage: lptv get <service>          Show account details\n"
                   "         lptv get <service> --show   Show account details and password\n\n"
        }},

        {"delete", {
            "Delete an account or category",
            "Help: delete",
            "  Usage: lptv delete <service>       Remove an account\n"
                   "         lptv delete category <name> Remove a category\n\n"
        }},

        {"update", {
            "Update an account",
            "Help: update",
            "  Usage: lptv update <service>       Update account details\n\n"
        }},

        {"search", {
            "Search accounts reference word",
            "Help: search",
            "  Usage: lptv search <query>         Search accounts by service or username\n\n"
        }},

        {"init", {
            "Initialize the vault",
            "Help: init",
            "  Usage: lptv init                   Initialize a new vault and set master password\n\n"
        }},

        {"status", {
            "Check vault status",
            "Help: status",
            "  Usage: lptv status                 Check if vault is initialized and unlocked\n\n"
        }},

        {"shell", {
            "Enter interactive mode",
            "Help: shell",
            "  Usage: lptv shell                  Enter interactive shell mode\n\n"
        }}
    };
};

void HelpPrinter::printCommand(std::string_view command) {
    auto it = HELP.find(command);

    if(it == HELP.end()) {
        throw CommandException(
            CommandCode::Error,
            "Unknown command: " + std::string(command)
        );
    }

    Console::printHeader(it->second.title);
    std::cout << it->second.usage;
}

void HelpPrinter::printGeneral() {
    Console::printHeader("LPTV Password Manager");

    std::cout
        << "Usage:\n"
        << "  lptv <command> [arguments]\n\n"
        << "Commands:\n";
    
    for(const auto& [command, info] : HELP) {
        std::cout
            << "  "
            << Console::BOLD
            << std::left << std::setw(10)
            << command
            << Console::RESET
            << info.description
            << '\n';
    }

    std::cout
        << "\nHelp:\n"
        << "  lptv <command> --help\n"
        << "  lptv <command> -h\n\n"
        << Console::GREY
        << "Standalone commands automatically prompt for the master password\n"
        << "when the vault is locked."
        << Console::RESET
        << "\n\n";

    // std::cout << "\nUsage: lptv <command>\n\n"
    //     << "where <command> is one of:\n"
    //     << "    " << Console::BOLD << "init" << Console::RESET << "       Initialize the vault\n"
    //     << "    " << Console::BOLD << "status" << Console::RESET << "     Check vault status\n"
    //     << "    " << Console::BOLD << "shell" << Console::RESET << "      Enter interactive mode\n"
    //     << "    " << Console::BOLD << "list" << Console::RESET << "       List accounts or categories\n"
    //     << "    " << Console::BOLD << "search" << Console::RESET << "     Search accounts\n"
    //     << "    " << Console::BOLD << "add" << Console::RESET << "        Add an account or category\n"
    //     << "    " << Console::BOLD << "update" << Console::RESET << "     Update an account\n"
    //     << "    " << Console::BOLD << "get" << Console::RESET << "        Get account details\n"
    //     << "    " << Console::BOLD << "delete" << Console::RESET << "     Delete an account or category\n\n"
    //     << "lptv help <command>  search for help on <command>\n"
    //     << "lptv <command> -h    quick help on <command>\n\n"
    //     << Console::GREY << "Note: Standalone commands will auto-prompt for password if the vault is locked." << Console::RESET << "\n\n";
}

void HelpPrinter::printHelp(std::string_view command) {
    if(command.empty()) {
        printGeneral();
        return;
    }

    printCommand(command);
}
