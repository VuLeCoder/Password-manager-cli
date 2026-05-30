#pragma once
#include <string>

namespace Constants {
    inline const std::string VAULT_DIR = ".lptv";
    inline const std::string VAULT_DB = ".lptv/vault.db";
    inline const std::string MASTER_FILE = ".lptv/lptv.dat";
    inline const std::string CONFIG_FILE = ".lptv/config.json";
    inline const std::string SESSION_FILE = ".lptv/session.dat";

    inline constexpr int SESSION_TIMEOUT = 1800;
}
