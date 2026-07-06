#include "cli/Console.h"
#include "string/SecureString.h"

#include <conio.h>
#include <iostream>
#include <iomanip>
#include <string_view>

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

void Console::printSuccess(std::string_view message) {
    enableAnsiSupport();
    std::cout << GREEN << "lptv " << RESET << GREEN << "success " << RESET << message << std::endl;
}

void Console::printError(std::string_view message) {
    enableAnsiSupport();
    std::cerr << RED << "lptv " << RESET << BOLD << RED << "ERR! " << RESET << message << std::endl;
}

void Console::printWarning(std::string_view message) {
    enableAnsiSupport();
    std::cout << YELLOW << "lptv " << RESET << YELLOW << "warn " << RESET << message << std::endl;
}

void Console::printInfo(std::string_view message) {
    enableAnsiSupport();
    std::cout << CYAN << "lptv " << RESET << "info " << message << std::endl;
}

void Console::printHeader(std::string_view message) {
    enableAnsiSupport();
    std::cout << std::endl << BOLD << message << RESET << std::endl;
}

void Console::printHiddenPassword(const SecureString& password) {
    std::cout << "***********";
}

void Console::printTable(
    const std::vector<std::string>& headers,
    const std::vector<std::vector<SecureString>>& rows
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
            colWidths[i] = std::max(colWidths[i], row[i].size());
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
            std::string_view val = (i < row.size()) ? row[i].view() : "";
            std::cout << std::left << std::setw(colWidths[i] + 2) << val;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// === === === === ===
//
// === === === === ===
bool Console::readLine(SecureString& out, bool echo) {
    enableAnsiSupport();
    out.clear();

    while(true) {
        int ch = _getch();

        if(ch == 26) {
            return false;
        }

        if(ch == 0 || ch == 0xE0) {
            _getch();
            continue;
        }

        switch(ch) {
        case '\r':
            std::cout << '\n';
            return true;

        case '\b':
            if(!out.empty()) {
                out.pop_back();

                if(echo) {
                    std::cout << "\b \b";
                    std::cout.flush();
                }
            }
            break;

        default:
            if(ch >= 32 && ch <= 126) {
                out.push_back(static_cast<char>(ch));

                if (echo) {
                    std::cout.put(static_cast<char>(ch));
                    std::cout.flush();
                }
            }
            break;
        }
    }
}

bool Console::readSecureInput(SecureString& out) {
    return readLine(out, true);
}

bool Console::readSecureHiddenInput(SecureString& out) {
    return readLine(out, false);
}

bool Console::copyToClipboard(const SecureString& text) {
    FILE* pipe = _popen("clip", "w");
    if (!pipe) return false;

    fprintf(pipe, "%s", text.c_str());
    _pclose(pipe);
    return true;
}
