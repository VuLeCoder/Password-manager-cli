#pragma once

class AuthGuard{
public:
    static bool verify();
    static void unlock();
    static void lock();
};
