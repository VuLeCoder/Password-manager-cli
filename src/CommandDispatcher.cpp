#include "../include/CommandDispatcher.h"
#include "../include/Vault.h"
#include "../include/PasswordManager.h"

#include <iostream>

// === === private === ===
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
            << "lptv list\n"
            << "lptv add <service>\n"
            << "lptv update <service>\n"
            << "lptv get <service>\n"
            << "lptv delete <service>\n";

        return 0;
    }

    if(cmd.name == "init") {
        if(Vault::initialize()) {
            std::cout << "Password vault initialized.\n";
        } else {
            std::cout << "Password vault already exists.\n";
        }
        return 0;
    }

    if(!Vault::exists()) {
        std::cout
            << "Password vault not initialized.\n"
            << "Run: lptv init\n";

        return 0;
    }

    if(cmd.name == "list") {
        PasswordManager lptv;
        lptv.list();
        return 0;
    }

    if(cmd.name == "add") {
        if(!requireService(cmd)) return 0;

        PasswordManager lptv;
        lptv.add(cmd.args[0]);
        return 0;
    }

    if(cmd.name == "update") {
        if(!requireService(cmd)) return 0;

        PasswordManager lptv;
        lptv.update(cmd.args[0]);
        return 0;
    }

    if(cmd.name == "get") {
        if(!requireService(cmd)) return 0;
        
        PasswordManager lptv;
        lptv.get(cmd.args[0]);
        return 0;
    }

    if(cmd.name == "delete") {
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
