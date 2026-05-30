#pragma once
#include "./Command.h"

#include <string>
#include <vector>

class CommandParser {
public:
    static Command parse(int argc, char* argv[]);
    static Command parse(const std::string& input);
};
