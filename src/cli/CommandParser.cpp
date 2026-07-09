#include "cli/Command.h"
#include "cli/CommandParser.h"
#include "exception/CommandException.h"

#include <string/SecureString.h>
#include <unordered_map>
#include <cstddef>

// === === ===
constexpr static bool isHelpCmd(std::string_view sv) {
    return sv == "-h" || sv == "--help";
}
// === === ===

// === === ===
Command CommandParser::parse(int argc, char* argv[]) {
    Command cmd;
    if(argc > 1) {
        cmd.name = argv[1];
    }

    for(int i=2; i<argc; ++i) {
        cmd.args.push_back(argv[i]);
    }

    validate(cmd);
    return cmd;
}

Command CommandParser::parse(const SecureString& input) {
    Command cmd;

    size_t i = 0;
    size_t n = input.size();

    while(i < n && std::isspace(static_cast<unsigned char>(input[i]))) ++i;
    while(i < n && !std::isspace(static_cast<unsigned char>(input[i]))) {
        cmd.name.push_back(input[i++]);
    }

    while(i < n) {
        while(i < n && std::isspace(static_cast<unsigned char>(input[i]))) ++i;

        SecureString part;
        while(i < n && !std::isspace(static_cast<unsigned char>(input[i]))) {
            part.push_back(input[i++]);
        }
        if(!part.empty()) cmd.args.push_back(std::move(part));
    }

    validate(cmd);
    return cmd;
}
// === === ===

void CommandParser::validate(const Command& cmd) {
    if(isHelpCmd(cmd.name) && cmd.args.empty()) {
        return;
    }

    using ValidateFunc = void(*)(const Command&);
    static const std::unordered_map<std::string, ValidateFunc> validates = {
        { "init",    validateInit    },
        { "status",  validateStatus  },
        { "shell",   validateShell   },

        { "add",     validateAdd     },
        { "list",    validateList    },
        { "get",     validateGet     },
        { "delete",  validateDelete  },
        { "update",  validateUpdate  },
        { "search",  validateSearch  },
    };

    auto it = validates.find(cmd.name);
    if(it != validates.end()) {
        it->second(cmd);
        return;
    }

    throw CommandException(
        CommandCode::Unknown, 
        "Unknown command: " + cmd.name
    );
}

void CommandParser::validateInit(const Command& cmd) {
    if(cmd.args.empty()) {
        return;
    }

    if(cmd.args.size() == 1 && isHelpCmd(cmd.args.front().view())) {
        return;
    }

    throw CommandException(
        CommandCode::Error,
        "Invalid usage.\nRun 'lptv " + cmd.name + " -h' for help."
    );
}

void CommandParser::validateStatus(const Command& cmd) {
    if(cmd.args.empty()) {
        return;
    }

    if(cmd.args.size() == 1 && isHelpCmd(cmd.args.front().view())) {
        return;
    }

    throw CommandException(
        CommandCode::Error,
        "Invalid usage.\nRun 'lptv " + cmd.name + " -h' for help."
    );
}

void CommandParser::validateShell(const Command& cmd) {
    if(cmd.args.empty()) {
        return;
    }

    if(cmd.args.size() == 1 && isHelpCmd(cmd.args.front().view())) {
        return;
    }

    throw CommandException(
        CommandCode::Error,
        "Invalid usage.\nRun 'lptv " + cmd.name + " -h' for help."
    );
}


void CommandParser::validateAdd(const Command& cmd) {
    const auto& args = cmd.args;
    
    if(args.size() == 1 && args.front() != "category") {
        return;
    }

    if(args.size() == 2 && args.front() == "category") {
        return;
    }

    throw CommandException(
        CommandCode::Error,
        "Invalid usage.\nRun 'lptv " + cmd.name + " -h' for help."
    );
}

void CommandParser::validateList(const Command& cmd) {
    if(cmd.args.empty()) {
        return;
    }

    if(cmd.args.size() == 1 && isHelpCmd(cmd.args.front().view())) {
        return;
    }

    throw CommandException(
        CommandCode::Error,
        "Invalid usage.\nRun 'lptv " + cmd.name + " -h' for help."
    );
}

void CommandParser::validateGet(const Command& cmd) {
    if(cmd.args.size() == 1) {
        return;
    }

    throw CommandException(
        CommandCode::Error,
        "Invalid usage.\nRun 'lptv " + cmd.name + " -h' for help."
    );
}

void CommandParser::validateDelete(const Command& cmd) {
    const auto& args = cmd.args;

    if(args.size() == 1 && args.front() != "category") {
        return;
    }

    if(args.size() == 2 && args.front() == "category") {
        return;
    }

    throw CommandException(
        CommandCode::Error,
        "Invalid usage.\nRun 'lptv " + cmd.name + " -h' for help."
    );
}

void CommandParser::validateUpdate(const Command& cmd) {
    if(cmd.args.size() == 1) {
        return;
    }

    throw CommandException(
        CommandCode::Error,
        "Invalid usage.\nRun 'lptv " + cmd.name + " -h' for help."
    );
}

void CommandParser::validateSearch(const Command& cmd) {
    if(cmd.args.size() == 1) {
        return;
    }

    throw CommandException(
        CommandCode::Error,
        "Invalid usage.\nRun 'lptv " + cmd.name + " -h' for help."
    );
}
