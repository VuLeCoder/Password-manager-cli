#pragma once
#include "./VaultData.h"

#include <vector>
#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Storage {
private:
    static bool replaceVault(
        const std::string& tempPath,
        const std::string& finalPath
    );

public:
    static bool save(
        const VaultData& vaultData,
        const std::string& path
    );

    static bool saveDraft(
        const VaultData& vaultData,
        const std::string& tempPath
    );

    static VaultData load(
        const std::string& path
    );
};
