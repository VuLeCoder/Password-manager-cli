#pragma once
#include <string>

namespace Constants {
    inline const std::string VAULT_DIR = ".lptv";

    inline const std::string ACCOUNT = "accounts";
    inline const std::string CATEGORY = "categories";
    inline const std::string VAULT_DB = VAULT_DIR + "/vault.db";

    inline const std::string MASTER_FILE = VAULT_DIR + "/lptv.dat";    
    
    inline const std::string CONFIG_FILE = VAULT_DIR + "/config.json";
}
