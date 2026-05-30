#pragma once

class SessionManager{
public:
    static bool exists();
    static bool isValid();
    static void create();
    static void destroy();
};
