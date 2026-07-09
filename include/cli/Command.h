#pragma once

#include <string/SecureString.h>
#include <string>
#include <vector>

namespace cmd {
    constexpr std::string_view INIT = "init";
    constexpr std::string_view STATUS = "status";
    constexpr std::string_view SHELL = "shell";

    constexpr std::string_view ADD = "add";
    constexpr std::string_view LIST = "list";
    constexpr std::string_view GET = "get";
    constexpr std::string_view DELETE = "delete";
    constexpr std::string_view UPDATE = "update";
    constexpr std::string_view SEARCH = "search";
    constexpr std::string_view GENERATE = "generate";
    constexpr std::string_view CHANGE_PASS = "change-password";
}

struct Command{
    std::string name;
    std::vector<SecureString> args;
};
