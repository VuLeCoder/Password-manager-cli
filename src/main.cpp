#include "./../include/cli/CommandDispatcher.h"
#include "./../include/cli/CommandParser.h"

int main(int argc, char* argv[]){
    auto command = CommandParser::parse(argc, argv);
    return CommandDispatcher::execute(command);
}
