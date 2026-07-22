#include "cli/Console.h"
#include "string/SecureString.h"

#include <iomanip>
#include <thread>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
    #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    #endif

    #define POPEN  _popen
    #define PCLOSE _pclose

    void Console::enableAnsiSupport() {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE) return;

        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode)) return;

        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }

    int Console::getch() {
        return _getch();
    }

    bool Console::kbhit() {
        return _kbhit();
    }

#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/select.h>

    #define POPEN  popen
    #define PCLOSE pclose

    namespace {
        termios original_tty;
        bool tty_changed = false;

        void setRawMode(bool enable) {
            if (enable) {
                if (tcgetattr(STDIN_FILENO, &original_tty) == 0) {
                    termios newt = original_tty;
                    newt.c_lflag &= ~(ICANON | ECHO);
                    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
                    tty_changed = true;
                }
            } else {
                if (tty_changed) {
                    tcsetattr(STDIN_FILENO, TCSANOW, &original_tty);
                    tty_changed = false;
                }
            }
        }
    }

    void Console::enableAnsiSupport() {}

    int Console::getch() {
        unsigned char c = 0;
        ssize_t n = read(STDIN_FILENO, &c, 1);
        if (n == 1) {
            return c;
        }
        return EOF;
    }

    bool Console::kbhit() {
        fd_set rfds;
        struct timeval tv;
        FD_ZERO(&rfds);
        FD_SET(STDIN_FILENO, &rfds);
        tv.tv_sec = 0;
        tv.tv_usec = 0;

        int retval = select(STDIN_FILENO + 1, &rfds, NULL, NULL, &tv);
        return (retval > 0);
    }

#endif

const std::string Console::RESET   = "\033[0m";
const std::string Console::BOLD    = "\033[1m";
const std::string Console::RED     = "\033[31m";
const std::string Console::GREEN   = "\033[32m";
const std::string Console::YELLOW  = "\033[33m";
const std::string Console::BLUE    = "\033[34m";
const std::string Console::MAGENTA = "\033[35m";
const std::string Console::CYAN    = "\033[36m";
const std::string Console::GREY    = "\033[90m";

std::chrono::steady_clock::time_point Console::shellEndTime;
bool Console::isShell = false;

void Console::printHeader(std::string_view message) {
    enableAnsiSupport();
    std::cout << std::endl << BOLD << message << RESET << std::endl;
}

void Console::printError(std::string_view sv) {
    enableAnsiSupport();

    std::cerr << RED << "lptv " << RESET << BOLD << RED << "ERR! " << RESET;
    std::cout << sv;
    std::cout << std::endl;
}

void Console::printHiddenPassword() {
    std::cout << "***********";
}

void Console::printTable(
    const std::vector<std::string>& headers,
    const std::vector<std::vector<SecureString>>& rows
) {
    if (headers.empty()) return;
    enableAnsiSupport();

    size_t numCols = headers.size();
    std::vector<size_t> colWidths(numCols, 0);

    for (size_t i = 0; i < numCols; ++i) {
        colWidths[i] = headers[i].length();
    }

    for (const auto& row : rows) {
        for (size_t i = 0; i < numCols && i < row.size(); ++i) {
            colWidths[i] = std::max(colWidths[i], row[i].size());
        }
    }

    // Print Header
    std::cout << std::endl << BOLD << GREY;
    for (size_t i = 0; i < numCols; ++i) {
        std::cout << std::left << std::setw(colWidths[i] + 2) << headers[i];
    }
    std::cout << RESET << std::endl;

    // Print separator (minimalist)
    std::cout << GREY;
    for (size_t i = 0; i < numCols; ++i) {
        std::cout << std::string(colWidths[i], '-') << "  ";
    }
    std::cout << RESET << std::endl;

    // Print Rows
    for (const auto& row : rows) {
        for (size_t i = 0; i < numCols; ++i) {
            std::string_view val = (i < row.size()) ? row[i].view() : "";
            std::cout << std::left << std::setw(colWidths[i] + 2) << val;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// === === === === ===
//
// === === === === ===
bool Console::readLine(SecureString& out, bool echo) {
    enableAnsiSupport();
    std::cout.flush();
    out.clear();

#ifndef _WIN32
    setRawMode(true);
#endif

    while(true) {
        int ch = 0;
        while (true) {
            if(isShell && std::chrono::steady_clock::now() >= shellEndTime) {
#ifndef _WIN32
                setRawMode(false);
#endif
                Console::printWarning("Shell session timeout (1 minutes elapsed). Exiting...");

                std::this_thread::sleep_for(std::chrono::seconds(3));
                Console::clear();
                std::exit(0);
            }
            if(kbhit()) {
                ch = getch();
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        if(ch == 26) {
#ifndef _WIN32
            setRawMode(false);
#endif
            return false;
        }

        if(ch == 0 || ch == 0xE0) {
            getch();
            continue;
        }

        switch(ch) {
        case '\r':
        case '\n':
            std::cout << '\n';
#ifndef _WIN32
            setRawMode(false);
#endif
            return true;

        case 127:
        case '\b':
            if(!out.empty()) {
                out.pop_back();

                if(echo) {
                    std::cout << "\b \b";
                    std::cout.flush();
                }
            }
            break;

        default:
            if(ch >= 32 && ch <= 126) {
                out.push_back(static_cast<char>(ch));

                if (echo) {
                    std::cout.put(static_cast<char>(ch));
                    std::cout.flush();
                }
            }
            break;
        }
    }
}

bool Console::readSecureInput(SecureString& out) {
    return readLine(out, true);
}

bool Console::readSecureHiddenInput(SecureString& out) {
    return readLine(out, false);
}

bool Console::copyToClipboard(const SecureString& text, int delaySeconds) {
    FILE* pipe = POPEN("clip", "w");
    if (!pipe) return false;

    fprintf(pipe, "%s", text.c_str());
    PCLOSE(pipe);

    if (delaySeconds > 0) {
        std::string cmd = "start /b powershell -WindowStyle Hidden -Command \"Start-Sleep -Seconds " 
                          + std::to_string(delaySeconds) 
                          + "; cmd.exe /c 'type nul | clip'\"";
        FILE* clearPipe = POPEN(cmd.c_str(), "w");
        if(clearPipe) {
            PCLOSE(clearPipe);
        }
    }
    return true;
}

void Console::clear() {
    enableAnsiSupport();
    std::cout << "\x1b[2J\x1b[H";
    std::cout.flush();
}
