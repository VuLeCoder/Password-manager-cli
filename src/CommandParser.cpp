#include "./../include/CommandParser.h"

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
