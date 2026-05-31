#include "./../../include/core/AuthGuard.h"
#include "./../../include/core/SessionContext.h"

bool AuthGuard::verify() {
    return SessionContext::get().isActive();
}

void AuthGuard::unlock(const std::array<uint8_t, 32>& k) {
    if(AuthGuard::verify()) {
        return;
    }

    SessionContext::get().setKey(k);
}

void AuthGuard::lock() {
    SessionContext::get().clear();
}
