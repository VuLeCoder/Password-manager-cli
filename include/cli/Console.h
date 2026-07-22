#pragma once
#include <string/SecureString.h>
#include <string_view>
#include <vector>
#include <iostream>
#include <chrono>

class Console{
public:
    static bool readSecureInput(SecureString&);
    static bool readSecureHiddenInput(SecureString&);

    static void printHiddenPassword();

    static void printTable(
        const std::vector<std::string>& headers,
        const std::vector<std::vector<SecureString>>& rows
    );

    static bool copyToClipboard(const SecureString& text, int delaySeconds = 30);

    static void clear();

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

    static std::chrono::steady_clock::time_point shellEndTime;
    static bool isShell;

    // Semantic Printing
    template<typename... Args>
    static void printSuccess(const Args&... args);

    template<typename... Args>
    static void printWarning(const Args&... args);

    template<typename... Args>
    static void printInfo(const Args&... args);

    static void printError(std::string_view message);
    static void printHeader(std::string_view message);


private:
    static int getch();
    static bool kbhit();

    static void enableAnsiSupport();
    static bool readLine(SecureString& out, bool echo);
};

template<typename... Args>
void Console::printSuccess(const Args&... args) {
    enableAnsiSupport();

    std::cout << GREEN << "lptv " << RESET << GREEN << "success " << RESET;
    (std::cout << ... << args);
    std::cout << std::endl;
}

template<typename... Args>
void Console::printWarning(const Args&... args) {
    enableAnsiSupport();

    std::cout << YELLOW << "lptv " << RESET << YELLOW << "warn " << RESET;
    (std::cout << ... << args);
    std::cout << std::endl;
}

template<typename... Args>
void Console::printInfo(const Args&... args) {
    enableAnsiSupport();

    std::cout << CYAN << "lptv " << RESET << "info ";
    (std::cout << ... << args);
    std::cout << std::endl;
}
