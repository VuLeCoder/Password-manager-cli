#include "./../../include/core/SessionContext.h"
#include "./../../include/storage/Storage.h"
#include "./../../include/utils/Encryptor.h"
#include "./../../include/utils/Constants.h"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <openssl/rand.h>

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
    
    std::ofstream file(path, std::ios::binary);
    if(!file.is_open()) {
        return false;
    }

    json j;
    j[Constants::ACCOUNT] = vaultData.accounts;
    j[Constants::CATEGORY] = vaultData.categories;

    std::string data = j.dump();
    std::vector<uint8_t> plaintext(data.begin(), data.end());

    auto& key = SessionContext::get().getKey();

    std::array<uint8_t, 12> nonce;
    std::array<uint8_t, 16> tag;
    RAND_bytes(nonce.data(), nonce.size());

    std::vector<uint8_t> ciphertext;
    bool ok = Encryptor::encrypt(
        plaintext,
        key.data(),
        nonce.data(),
        ciphertext,
        tag
    );
    if(!ok) return false;
    
    file.write(reinterpret_cast<char*>(nonce.data()), nonce.size());
    file.write(reinterpret_cast<char*>(tag.data()), tag.size());
    file.write(reinterpret_cast<char*>(ciphertext.data()), ciphertext.size());

    file.close();
    return true;
}

VaultData Storage::load(
    const std::string& path
) {    
    std::ifstream file(path, std::ios::binary);
    if(!file.is_open()) {
        return {};
    }

    std::vector<uint8_t> data(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );
    file.close();

    if(data.size() < 28) {
        return {};
    }

    std::array<uint8_t, 12> nonce;
    std::array<uint8_t, 16> tag;

    std::copy(data.begin(), data.begin() + 12, nonce.begin());
    std::copy(data.begin() + 12, data.begin() + 28, tag.begin());

        std::vector<uint8_t> ciphertext(
        data.begin() + 28,
        data.end()
    );

    std::vector<uint8_t> plaintext;

    auto& key = SessionContext::get().getKey();

    bool ok = Encryptor::decrypt(
        ciphertext,
        key.data(),
        nonce.data(),
        tag.data(),
        plaintext
    );
    if(!ok) return {};

    std::string jsonStr(
        plaintext.begin(),
        plaintext.end()
    );

    json j = json::parse(jsonStr);

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

