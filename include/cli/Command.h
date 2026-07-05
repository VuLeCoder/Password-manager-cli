#pragma once

#include <string/SecureString.h>
#include <string>
#include <vector>

struct Command{
    std::string name;
    std::vector<SecureString> args;
};
