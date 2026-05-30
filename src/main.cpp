#include "../include/PasswordManager.h"

#include <iostream>

int main(
    int argc,
    char* argv[]
)
{
    PasswordManager manager;

    if (argc < 2)
    {
        std::cout
            << "Usage:\n"
            << "lptv add <service>\n"
            << "lptv list\n"
            << "lptv get <service>\n"
            << "lptv delete <service>\n";

        return 0;
    }

    std::string command =
        argv[1];

    if (command == "add")
    {
        if (argc < 3)
        {
            std::cout
                << "Missing service name.\n";
            return 0;
        }

        manager.add(argv[2]);
    }
    else if (
        command == "list"
    )
    {
        manager.list();
    }
    else if (
        command == "get"
    )
    {
        if (argc < 3)
        {
            std::cout
                << "Missing service name.\n";
            return 0;
        }

        manager.get(argv[2]);
    }
    else if (
        command == "delete"
    )
    {
        if (argc < 3)
        {
            std::cout
                << "Missing service name.\n";
            return 0;
        }

        manager.remove(argv[2]);
    }
    else
    {
        std::cout
            << "Unknown command.\n";
    }

    return 0;
}
