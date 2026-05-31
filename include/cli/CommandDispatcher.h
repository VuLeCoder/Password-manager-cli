#pragma once
#include "Command.h"

class CommandDispatcher{
private:
    static bool m_isShellMode;
    static bool requireUnlock();
    static bool requireService(const Command& cmd);

public:
    static int execute(const Command& cmd);
};
