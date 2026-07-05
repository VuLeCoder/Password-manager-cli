#pragma once
#include <string/SecureString.h>
#include <string_view>
#include <vector>

class Console{
public:
    static bool readSecureInput(SecureString&);
    static bool readSecureHiddenInput(SecureString&);

    static void printHiddenPassword(const SecureString& password);

    static void printTable(
        const std::vector<std::string>& headers,
        const std::vector<std::vector<SecureString>>& rows
    );
    static bool copyToClipboard(const SecureString& text);

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
    static void printSuccess(std::string_view message);
    static void printError(std::string_view message);
    static void printWarning(std::string_view message);
    static void printInfo(std::string_view message);
    static void printHeader(std::string_view message);


private:
    static bool readLine(SecureString& out, bool echo);
};