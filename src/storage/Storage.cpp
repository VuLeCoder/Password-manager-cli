#include "./../../include/storage/Storage.h"
#include "./../../include/storage/Encryptor.h"
#include "./../../include/utils/Constants.h"

#include <fstream>
#include <sstream>
#include <filesystem>

const std::string KEY = "lptvu.vuzle";

// === === private === ===


// === === public === ===
bool Storage::save(
    const VaultData& vaultData,
    const std::string& path
) {
    std::filesystem::path p(path);
    if(p.has_parent_path()) {
        std::filesystem::create_directories(p.parent_path());
    }
    
    std::ofstream file(path);
    if(!file.is_open()) {
        return false;
    }

    json j;
    j[Constants::ACCOUNT] = vaultData.accounts;
    j[Constants::CATEGORY] = vaultData.categories;

    std::string data = j.dump(4);
    file << data;
    // file << Encryptor::encrypt(data, KEY);

    file.close();
    return true;
}

VaultData Storage::load(
    const std::string& path
) {    
    std::ifstream file(path);
    if(!file.is_open()) {
        return {};
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    std::string encryted = buffer.str();
    // std::string plaintext = Encryptor::decrypt(encryted, KEY);

    if(encryted.empty()) {
        return {};
    }

    // json j = json::parse(plaintext);
    json j = json::parse(encryted);

    VaultData vaultData;

    if(j.contains(Constants::ACCOUNT)) {
        vaultData.accounts =
            j[Constants::ACCOUNT]
            .get<std::vector<Account>>();
    }

    if(j.contains(Constants::CATEGORY)) {
        vaultData.categories =
            j[Constants::CATEGORY]
            .get<std::vector<std::string>>();
    }

    return vaultData;
}

