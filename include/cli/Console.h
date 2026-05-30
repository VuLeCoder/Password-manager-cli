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
};