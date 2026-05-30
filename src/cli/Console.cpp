#include "./../../include/cli/Console.h"

#include <conio.h>
#include <iostream>
#include <algorithm>
#include <iomanip>

std::string Console::getHiddenInput() {
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

    size_t numCols = headers.size();
    std::vector<size_t> colWidths(numCols, 0);

    // Calculate column widths
    for (size_t i = 0; i < numCols; ++i) {
        colWidths[i] = headers[i].length();
    }

    for (const auto& row : rows) {
        for (size_t i = 0; i < numCols && i < row.size(); ++i) {
            colWidths[i] = std::max(colWidths[i], row[i].length());
        }
    }

    // Helper to print separator line
    auto printSeparator = [&]() {
        for (size_t width : colWidths) {
            std::cout << "+" << std::string(width + 2, '-');
        }
        std::cout << "+" << std::endl;
    };

    // Print Header
    printSeparator();
    for (size_t i = 0; i < numCols; ++i) {
        std::cout << "| " << std::left << std::setw(colWidths[i]) << headers[i] << " ";
    }
    std::cout << "|" << std::endl;
    printSeparator();

    // Print Rows
    for (const auto& row : rows) {
        for (size_t i = 0; i < numCols; ++i) {
            std::string val = (i < row.size()) ? row[i] : "";
            std::cout << "| " << std::left << std::setw(colWidths[i]) << val << " ";
        }
        std::cout << "|" << std::endl;
    }
    printSeparator();
}
