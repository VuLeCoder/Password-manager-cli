#include "cli/CommandDispatcher.h"
#include "cli/CommandParser.h"
#include "cli/Console.h"

#include "exception/PasswordManagerException.h"

int main(int argc, char* argv[]) {
    try {
        auto command = CommandParser::parse(argc, argv);
        CommandDispatcher::execute(command);
    } catch (const PasswordManagerException& e) {
        Console::printError(e.what());
        return 1;
    } catch (const std::exception& e) {
        Console::printError(e.what());
        return 1;
    }
    
    return 0;
}
