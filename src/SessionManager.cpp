#include "./../include/Constants.h"
#include "./../include/SessionManager.h"

#include <time.h>
#include <filesystem>
#include <fstream>

bool SessionManager::exists() {
    return std::filesystem::exists(Constants::SESSION_FILE);
}

bool SessionManager::isValid() {
    if(!exists()) return false;

    std::time_t expire;
    std::ifstream file(Constants::SESSION_FILE);
    if(!file.is_open()) return false;

    file >> expire;
    file.close();

    if(time(nullptr) >= expire) {
        destroy();
        return false;
    }
    return true;
}

void SessionManager::create() {
    auto expire = time(nullptr) + 1800;
    std::ofstream file(Constants::SESSION_FILE);
    file << expire;
    file.close();
}

void SessionManager::destroy() {
    std::filesystem::remove(Constants::SESSION_FILE);
}
