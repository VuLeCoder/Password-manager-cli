#include "./../../include/core/SessionContext.h"
#include <algorithm>
#include <cstring>
#include <openssl/crypto.h>

SessionContext& SessionContext::get() {
    static SessionContext instance;
    return instance;
}

void SessionContext::setKey(const std::array<uint8_t, 32>& k) {
    key = k;
    active = true;
}

const std::array<uint8_t, 32>& SessionContext::getKey() const {
    return key;
}

void SessionContext::clear() {
    OPENSSL_cleanse(key.data(), key.size());
    active = false;
}

bool SessionContext::isActive() const {
    return active;
}
