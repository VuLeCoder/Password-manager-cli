#pragma once
#include <filesystem>

namespace fs = std::filesystem;

namespace Constants {
    inline const fs::path VAULT_DIR = ".lptv";

    inline const fs::path ACCOUNT = "accounts";
    inline const fs::path CATEGORY = "categories";
    inline const fs::path VAULT_DB = VAULT_DIR / "/vault.db";

    inline const fs::path MASTER_FILE = VAULT_DIR / "/lptv.dat";    
    
    inline const fs::path CONFIG_FILE = VAULT_DIR / "/config.json";
}
