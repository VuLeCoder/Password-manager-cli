#pragma once
#include <string>
#include <vector>

class CommandParser {
public:
    static std::vector<std::string> parse(int argc, char* argv[]);
};
