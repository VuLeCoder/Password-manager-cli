#pragma once
#include <string>
#include <vector>

class Console{
public:
    static std::string getHiddenInput();
    static void printTable(
        const std::vector<std::string>& headers,
        const std::vector<std::vector<std::string>>& rows
    );
    static bool copyToClipboard(const std::string& text);

    // ANSI Colors
    static const std::string RESET;
    static const std::string BOLD;
    static const std::string RED;
    static const std::string GREEN;
    static const std::string YELLOW;
    static const std::string BLUE;
    static const std::string MAGENTA;
    static const std::string CYAN;
    static const std::string GREY;

    // Semantic Printing
    static void printSuccess(const std::string& message);
    static void printError(const std::string& message);
    static void printWarning(const std::string& message);
    static void printInfo(const std::string& message);
    static void printHeader(const std::string& message);
};