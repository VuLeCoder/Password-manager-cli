#pragma once
#include "Command.h"

#include <string/SecureString.h>

class CommandParser {
public:
    static Command parse(int argc, char* argv[]);
    static Command parse(const SecureString& input);
};
