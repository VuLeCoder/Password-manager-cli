#pragma once
#include "Command.h"

#include <string/SecureString.h>

class CommandParser {
public:
    static Command parse(int argc, char* argv[]);
    static Command parse(const SecureString& input);

private:
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
};
