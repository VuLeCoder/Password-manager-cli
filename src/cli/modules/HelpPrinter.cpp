#include "cli/modules/HelpPrinter.h"
#include "cli/Console.h"
#include "exception/CommandException.h"
#include "cli/Command.h"

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
        {cmd::ADD, {
            "Add an account or category",
            "Help: add",
            "  Usage: lptv add <service>            Add a new account\n"
                   "         lptv add --category <name>    Add a new category\n\n"
        }},

        {cmd::LIST, {
            "List accounts or categories",
            "Help: list",
            "  Usage: lptv list                 List all accounts\n"
                   "         lptv list <category>      List accounts in category\n"
                   "         lptv list --category      List all categories\n\n"
        }},

        {cmd::GET, {
            "Get account details",
            "Help: get",
            "  Usage: lptv get <service>          Show account details\n"
                   "         lptv get <service> --show   Show account details and password\n\n"
        }},

        {cmd::DELETE, {
            "Delete an account or category",
            "Help: delete",
            "  Usage: lptv delete <service>          Remove an account\n"
                   "         lptv delete --category <name>  Remove a category\n\n"
        }},

        {cmd::UPDATE, {
            "Update an account",
            "Help: update",
            "  Usage: lptv update <service>       Update account details\n\n"
        }},

        {cmd::SEARCH, {
            "Search accounts by keyword",
            "Help: search",
            "  Usage: lptv search <query>   Search accounts by service or username\n\n"
        }},

        {cmd::INIT, {
            "Initialize the vault",
            "Help: init",
            "  Usage: lptv init     Initialize a new vault and set master password\n\n"
        }},

        {cmd::STATUS, {
            "Check vault status",
            "Help: status",
            "  Usage: lptv status   Check if vault is initialized and unlocked\n\n"
        }},

        {cmd::SHELL, {
            "Enter interactive mode",
            "Help: shell",
            "  Usage: lptv shell    Enter interactive shell mode\n\n"
        }},

        {cmd::GENERATE, {
            "Generate a secure random password",
            "Help: generate",
            "  Usage: lptv generate [length] [options]  Generate a random password\n"
                   "  Options:\n"
                   "    -u, --no-upper      Exclude uppercase letters\n"
                   "    -l, --no-lower      Exclude lowercase letters\n"
                   "    -d, --no-digits     Exclude digits\n"
                   "    -s, --no-special    Exclude special characters\n\n"
        }},

        {cmd::CHANGE_PASS, {
            "Change master password",
            "Help: change-password",
            "  Usage: lptv change-password      Change your vault's master password\n\n"
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
        << "  lptv <command> [options] [arguments]\n\n"
        << "Commands:\n";
    
    for(const auto& [command, info] : HELP) {
        if (!info.showInGeneral) {
            continue;
        }
        std::cout
            << "  "
            << Console::BOLD
            << std::left << std::setw(16)
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
}

void HelpPrinter::printHelp(std::string_view command) {
    if(command.empty()) {
        printGeneral();
        return;
    }

    printCommand(command);
}
