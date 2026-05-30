#include "./../../include/storage/Storage.h"
#include "./../../include/storage/Encryptor.h"

#include <fstream>
#include <sstream>
#include <filesystem>

const std::string KEY = "lptvu.vuzle";

static std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> result;

    std::stringstream ss(str);
    std::string item;

    while(std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }

    return result;
}

bool Storage::save(
    const std::vector<Account>& accounts,
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

    std::string data = "";
    for(const auto& account : accounts) {
        data += account.getService() + "|"
            + account.getUsername() + "|"
            + account.getPassword() + "|"
            + account.getNote() + "\n";
    }
    file << Encryptor::encrypt(data, KEY);

    file.close();
    return true;
}

std::vector<Account> Storage::load(
    const std::string& path
) {
    std::vector<Account> accounts;
    
    std::ifstream file(path);
    if(!file.is_open()) {
        return accounts;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    std::string encryted = buffer.str();
    std::string plaintext = Encryptor::decrypt(encryted, KEY);

    std::stringstream ss(plaintext);
    std::string line;
    while(std::getline(ss, line)) {
        if(line.empty()) {
            continue;
        }

        auto parts = split(line, '|');

        if(parts.size() == 3) {
            parts.emplace_back("");
        }

        if(parts.size() != 4) {
            continue;
        }

        accounts.emplace_back(
            parts[0],
            parts[1],
            parts[2],
            parts[3]
        );
    }
    
    return accounts;
}
