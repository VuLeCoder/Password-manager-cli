#pragma once

#include "VaultData.h"
#include <filesystem>

namespace fs = std::filesystem;

class BinaryStorage{
private:
    static void replaceVault(
        const fs::path&, const fs::path&
    );

    static void saveDraft(
        const VaultData&,
        const fs::path&
    );

public:
    static void save(
        const VaultData&,
        const fs::path&
    );

    static VaultData load(const fs::path&);
};
