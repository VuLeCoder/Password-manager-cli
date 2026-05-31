#include "./../../include/core/AuthGuard.h"
#include "./../../include/core/SessionContext.h"
#include "./../../include/core/SessionManager.h"

bool AuthGuard::verify() {
    return SessionManager::isValid() &&
        SessionContext::get().isActive();
}

void AuthGuard::unlock(const std::array<uint8_t, 32>& k) {
    if(AuthGuard::verify()) {
        return;
    }

    SessionManager::create();
    SessionContext::get().setKey(k);
}

void AuthGuard::lock() {
    SessionManager::destroy();
    SessionContext::get().clear();
}
