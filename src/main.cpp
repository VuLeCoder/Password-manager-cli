#include "cli/CommandDispatcher.h"
#include "cli/CommandParser.h"

int main(int argc, char* argv[]){
    auto command = CommandParser::parse(argc, argv);
    CommandDispatcher::execute(command);
    
    return 0;
}
