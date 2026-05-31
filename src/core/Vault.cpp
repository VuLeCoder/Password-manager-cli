#include "./../../include/core/Vault.h"
#include "./../../include/utils/Constants.h"
#include "./../../include/utils/Security.h"

#include <filesystem>
#include <fstream>

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

bool Vault::hasLPTVPassword() {
    std::ifstream file(Constants::MASTER_FILE);
    return file.peek() != EOF;
}

void Vault::setLPTVPassword(const std::string& password) {
    auto salt = Security::generateSalt();
    auto key = Security::deriveKey(password, salt);
    auto verifier = Security::sha256(key.data(), key.size());

    std::ofstream file(Constants::MASTER_FILE, std::ios::binary);
    
    file.write(
        reinterpret_cast<const char*>(salt.data()),
        salt.size()
    );

    file.write(
        reinterpret_cast<const char*>(verifier.data()),
        verifier.size()
    );
}

std::pair<bool, std::array<uint8_t, 32>> Vault::verifyLPTV(const std::string& password) {
    std::ifstream file(Constants::MASTER_FILE, std::ios::binary);
    if(!file.is_open()) {
        return {false, {}};
    }

    std::vector<uint8_t> salt(16);
    std::array<uint8_t, 32> savedVerifier;

    file.read(
        reinterpret_cast<char*>(salt.data()),
        salt.size()
    );

    file.read(
        reinterpret_cast<char*>(savedVerifier.data()),
        savedVerifier.size()
    );

    auto key = Security::deriveKey(
        password,
        salt
    );
    auto verifier = Security::sha256(key.data(), key.size());

    if (verifier == savedVerifier) {
        return {true, key};
    }

    return {false, {}};
}
