#pragma once

#include <vector>
#include <string>
#include "Account.h"

class Storage {
public:
    static bool save(
        const std::vector<Account>& accounts,
        const std::string& path
    );

    static std::vector<Account> load(
        const std::string& path
    );
};
