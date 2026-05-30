#include "./../../include/cli/CommandParser.h"
#include <sstream>

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

Command CommandParser::parse(const std::string& input) {
    Command cmd;
    std::stringstream ss(input);
    std::string part;

    if (ss >> part) {
        cmd.name = part;
    }

    while (ss >> part) {
        cmd.args.push_back(part);
    }

    return cmd;
}
