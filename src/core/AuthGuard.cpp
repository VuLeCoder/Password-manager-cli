#include "./../../include/core/AuthGuard.h"
#include "./../../include/core/SessionManager.h"

bool AuthGuard::verify() {
    return SessionManager::isValid();
}

void AuthGuard::unlock() {
    SessionManager::create();
}

void AuthGuard::lock() {
    SessionManager::destroy();
}
