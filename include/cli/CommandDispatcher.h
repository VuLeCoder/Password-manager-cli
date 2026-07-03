#pragma once
#include "Command.h"

class CommandDispatcher{
private:
    static bool m_isShellMode;
    static bool requireUnlock();
    static bool requireService(const Command& cmd);

    static bool isHelp(const Command& cmd);
    static int handleHelp(const Command& cmd);
    static int handleInit(const Command& cmd);
    static int handleShell(const Command& cmd);
    static int handleStatus(const Command& cmd);
    static int handleList(const Command& cmd);
    static int handleSearch(const Command& cmd);
    static int handleAdd(const Command& cmd);
    static int handleUpdate(const Command& cmd);
    static int handleGet(const Command& cmd);
    static int handleDelete(const Command& cmd);

public:
    static int execute(const Command& cmd);
};
