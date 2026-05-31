#include "./../../include/core/SessionContext.h"
#include <algorithm>
#include <cstring>

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
    std::fill(key.begin(), key.end(), 0);
    active = false;
}

bool SessionContext::isActive() const {
    return active;
}
