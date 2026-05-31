#include "./../../include/cli/Console.h"

#include <conio.h>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

const std::string Console::RESET   = "\033[0m";
const std::string Console::BOLD    = "\033[1m";
const std::string Console::RED     = "\033[31m";
const std::string Console::GREEN   = "\033[32m";
const std::string Console::YELLOW  = "\033[33m";
const std::string Console::BLUE    = "\033[34m";
const std::string Console::MAGENTA = "\033[35m";
const std::string Console::CYAN    = "\033[36m";
const std::string Console::GREY    = "\033[90m";

void enableAnsiSupport() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
}

void Console::printSuccess(const std::string& message) {
    enableAnsiSupport();
    std::cout << GREEN << "lptv " << RESET << GREEN << "success " << RESET << message << std::endl;
}

void Console::printError(const std::string& message) {
    enableAnsiSupport();
    std::cerr << RED << "lptv " << RESET << BOLD << RED << "ERR! " << RESET << message << std::endl;
}

void Console::printWarning(const std::string& message) {
    enableAnsiSupport();
    std::cout << YELLOW << "lptv " << RESET << YELLOW << "warn " << RESET << message << std::endl;
}

void Console::printInfo(const std::string& message) {
    enableAnsiSupport();
    std::cout << CYAN << "lptv " << RESET << "info " << message << std::endl;
}

void Console::printHeader(const std::string& message) {
    enableAnsiSupport();
    std::cout << std::endl << BOLD << message << RESET << std::endl;
}

std::string Console::getHiddenInput() {
    enableAnsiSupport();
    std::string input;

    while(true) {
        char c = _getch();
        
        if(c == '\r') {
            std::cout << std::endl;
            break;
        }

        if(c == '\b') {
            if (!input.empty()) {
                input.pop_back();
            }
            continue;
        }

        input += c;
    }
    return input;
}

void Console::printTable(
    const std::vector<std::string>& headers,
    const std::vector<std::vector<std::string>>& rows
) {
    if (headers.empty()) return;
    enableAnsiSupport();

    size_t numCols = headers.size();
    std::vector<size_t> colWidths(numCols, 0);

    for (size_t i = 0; i < numCols; ++i) {
        colWidths[i] = headers[i].length();
    }

    for (const auto& row : rows) {
        for (size_t i = 0; i < numCols && i < row.size(); ++i) {
            colWidths[i] = std::max(colWidths[i], row[i].length());
        }
    }

    // Print Header
    std::cout << std::endl << BOLD << GREY;
    for (size_t i = 0; i < numCols; ++i) {
        std::cout << std::left << std::setw(colWidths[i] + 2) << headers[i];
    }
    std::cout << RESET << std::endl;

    // Print separator (minimalist)
    std::cout << GREY;
    for (size_t i = 0; i < numCols; ++i) {
        std::cout << std::string(colWidths[i], '-') << "  ";
    }
    std::cout << RESET << std::endl;

    // Print Rows
    for (const auto& row : rows) {
        for (size_t i = 0; i < numCols; ++i) {
            std::string val = (i < row.size()) ? row[i] : "";
            std::cout << std::left << std::setw(colWidths[i] + 2) << val;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool Console::copyToClipboard(const std::string& text) {
    FILE* pipe = _popen("clip", "w");
    if (!pipe) return false;

    fprintf(pipe, "%s", text.c_str());
    _pclose(pipe);
    return true;
}
