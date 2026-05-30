#include "../include/CommandParser.h"
#include "../include/CommandDispatcher.h"

int main(int argc, char* argv[]){
    auto command = CommandParser::parse(argc, argv);
    return CommandDispatcher::execute(command);
}
