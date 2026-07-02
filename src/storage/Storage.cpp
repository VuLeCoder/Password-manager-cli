#include "./../../include/core/SessionContext.h"
#include "./../../include/storage/Storage.h"
#include "./../../include/utils/Encryptor.h"
#include "./../../include/utils/Constants.h"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <openssl/rand.h>

#ifdef _WIN32
#include <windows.h>
#endif

// === === private === ===
bool Storage::replaceVault(
    const std::string& tempPath,
    const std::string& finalPath
) {
#ifdef _WIN32
    std::wstring tempW = std::filesystem::path(tempPath).wstring();
    std::wstring finalW = std::filesystem::path(finalPath).wstring();
    
    BOOL ok = MoveFileExW(
        tempW.c_str(),
        finalW.c_str(),
        MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH
    );

    return ok != 0;
#else
    std::error_code ec;
    std::filesystem::rename(tempPath, finalPath, ec);
    return !ec;
#endif
}

// === === public === ===
bool Storage::save(
    const VaultData& vaultData,
    const std::string& path
) {
    std::filesystem::path finalPath(path);
    std::filesystem::path tempPath = finalPath;
    tempPath += ".tmp";

    if(finalPath.has_parent_path()) {
        std::filesystem::create_directories(finalPath.parent_path());
    }

    if(!saveDraft(vaultData, tempPath.string())) {
        return false;
    }

    if(!replaceVault(tempPath.string(), finalPath.string())) {
        std::filesystem::remove(tempPath);
        return false;
    }

    return true;
}

bool Storage::saveDraft(
    const VaultData& vaultData,
    const std::string& tempPath
) {
    json j;
    j[Constants::ACCOUNT] = vaultData.accounts;
    j[Constants::CATEGORY] = vaultData.categories;

    std::string data = j.dump();
    std::vector<uint8_t> plaintext(data.begin(), data.end());

    auto& key = SessionContext::get().getKey();

    std::array<uint8_t, 12> nonce;
    std::array<uint8_t, 16> tag;
    
    if(RAND_bytes(nonce.data(), nonce.size()) != 1) {
        return false;
    }

    std::vector<uint8_t> ciphertext;
    bool ok = Encryptor::encrypt(
        plaintext,
        key.data(),
        nonce.data(),
        ciphertext,
        tag
    );
    if(!ok) return false;

    std::ofstream file(tempPath, std::ios::binary);
    if(!file.is_open()) {
        return false;
    }
    
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

