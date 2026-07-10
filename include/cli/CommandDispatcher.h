#pragma once
#include "Command.h"

class CommandDispatcher{
private:
    static void requireUnlock();

    static bool isHelp(const Command& cmd);

    static void handleHelp(std::string_view);
    static void handleInit(const Command& cmd);
    static void handleShell(const Command& cmd);
    static void handleStatus(const Command& cmd);
    static void handleList(const Command& cmd);
    static void handleSearch(const Command& cmd);
    static void handleAdd(const Command& cmd);
    static void handleUpdate(const Command& cmd);
    static void handleGet(const Command& cmd);
    static void handleDelete(const Command& cmd);
    static void handleGenerate(const Command& cmd);
    static void handleChangePassword(const Command& cmd);
    static void handleClear(const Command& cmd);

public:
    static void execute(const Command& cmd);
};
