#pragma once
#include "core/Account.h"
#include <vector>

struct VaultData {
    std::vector<Account> accounts;
    std::vector<SecureString> categories;
};
