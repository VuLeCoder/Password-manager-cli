#include "./../include/Vault.h"
#include "./../include/Constants.h"

#include "filesystem"
#include "fstream"

bool Vault::exists() {
    return std::filesystem::exists(
        Constants::VAULT_DIR
    );
}

bool Vault::initialize() {
    if(exists()) {
        return false;
    }

    std::filesystem::create_directory(
        Constants::VAULT_DIR
    );

    std::ofstream vault(
        Constants::VAULT_DB
    );
    vault.close();

    std::ofstream master(
        Constants::MASTER_FILE
    );
    master.close();

    std::ofstream config(
        Constants::CONFIG_FILE
    );

    config
        << "{\n"
        << "  \"version\": \"1.0.0\"\n"
        << "}\n";

    config.close();

    return true;
}

