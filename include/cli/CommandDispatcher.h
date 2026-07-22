#pragma once
#include "Command.h"

class CommandDispatcher{
private:
    static void requireUnlock();

    static bool isHelp(const Command&);

    static void handleHelp(std::string_view);
    static void handleInit(const Command&);
    static void handleShell(const Command&);
    static void handleStatus(const Command&);
    static void handleList(const Command&);
    static void handleSearch(const Command&);
    static void handleAdd(const Command&);
    static void handleUpdate(const Command&);
    static void handleGet(const Command&);
    static void handleDelete(const Command&);
    static void handleGenerate(const Command&);
    static void handleChangePassword(const Command&);
    static void handleClear(const Command&);

public:
    static void execute(const Command&);
};
