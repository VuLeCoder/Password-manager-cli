#include "./../../include/cli/CommandParser.h"
#include "string/SecureString.h"
#include <cstddef>

Command CommandParser::parse(int argc, char* argv[]) {
    Command cmd;
    if(argc > 1) {
        cmd.name = argv[1];
    }

    for(int i=2; i<argc; ++i) {
        cmd.args.push_back(argv[i]);
    }
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

    return cmd;
}
