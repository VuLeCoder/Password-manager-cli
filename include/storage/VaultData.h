#pragma once
#include "./../core/Account.h"
#include <string>
#include <vector>

struct VaultData {
    std::vector<Account> accounts;
    std::vector<std::string> categories;
};
