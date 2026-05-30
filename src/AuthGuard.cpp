#include "./../include/AuthGuard.h"
#include "./../include/SessionManager.h"

bool AuthGuard::verify() {
    return SessionManager::isValid();
}

void AuthGuard::unlock() {
    SessionManager::create();
}

void AuthGuard::lock() {
    SessionManager::destroy();
}
