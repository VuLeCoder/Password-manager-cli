#pragma once
#include <string_view>

class Command;

class HelpPrinter {
public:
    static void printHelp(std::string_view command);

private:
    static void printCommand(std::string_view command);
    static void printGeneral();
};
