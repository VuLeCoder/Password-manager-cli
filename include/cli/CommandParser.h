#pragma once
#include "Command.h"

#include <string/SecureString.h>

namespace CommandCheck {
    bool isList(std::string_view);
    bool isGenerate(std::string_view);
    bool isChangePassword(std::string_view);

    bool isHelp(std::string_view);
    bool isCategory(std::string_view);
    bool isNoUpper(std::string_view);
    bool isNoLower(std::string_view);
    bool isNoDigits(std::string_view);
    bool isNoSpecial(std::string_view);
};

class CommandParser {
public:
    static Command parse(int argc, char* argv[]);
    static Command parse(const SecureString& input);

private:
    static std::string parse(const std::string&);
    static void validate(const Command&);

    static void validateInit(const Command&);
    static void validateStatus(const Command&);
    static void validateShell(const Command&);

    static void validateAdd(const Command&);
    static void validateList(const Command&);
    static void validateGet(const Command&);
    static void validateDelete(const Command&);
    static void validateUpdate(const Command&);
    static void validateSearch(const Command&);
    static void validateGenerate(const Command&);
    static void validateChangePassword(const Command&);
    static void validateClear(const Command&);
};
