#include "./../../include/cli/Console.h"

#include <conio.h>
#include <iostream>

std::string Console::getHiddenInput() {
    std::string input;

    while(true) {
        char c = _getch();
        
        if(c == '\r') {
            std::cout << std::endl;
            break;
        }

        if(c == '\b') {
            input.pop_back();
            continue;
        }

        input += c;
    }
    return input;
}
