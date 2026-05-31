#pragma once
#include <string>

namespace Constants {
    inline const std::string VAULT_DIR = ".lptv";

    inline const std::string VAULT_DB = VAULT_DIR + "/vault.db";
    inline const std::string CATEGORY_DB = VAULT_DIR + "/categories.db";

    inline const std::string MASTER_FILE = VAULT_DIR + "/lptv.dat";    
    inline const std::string SESSION_FILE = VAULT_DIR + "/session.dat";
    
    inline const std::string CONFIG_FILE = VAULT_DIR + "/config.json";

    inline constexpr int SESSION_TIMEOUT = 1800;
}
