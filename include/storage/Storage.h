#pragma once
#include "./VaultData.h"

#include <vector>
#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Storage {
public:
    static bool save(
        const VaultData& vaultData,
        const std::string& path
    );

    static VaultData load(
        const std::string& path
    );
};
