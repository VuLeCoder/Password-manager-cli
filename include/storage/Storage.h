#pragma once
#include "./../core/Account.h"

#include <vector>
#include <string>

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
