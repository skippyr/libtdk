#include <cstring>
#include <iostream>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#define BOOLEAN_STATE(a_value) (a_value ? "\x1b[32menabled\x1b[39m" : "\x1b[31mdisabled\x1b[39m")

static void writeHelpPage()
{
    std::cout << "\x1b[1mUsage:\x1b[22m \x1b[32mscan-codes \x1b[39m[\x1b[33mOPTION\x1b[39m]..."
              << std::endl
              << "Writes the scan codes related to key, focus and mouse events." << std::endl
              << "By default, it only handle key events." << std::endl
              << std::endl
              << "\x1b[1mOPTIONS\x1b[22m" << std::endl
              << "           \x1b[33m--help\x1b[39m  shows these help instructions." << std::endl
              << "    \x1b[33m--allow-mouse\x1b[39m  allows mouse events." << std::endl
              << "    \x1b[33m--allow-focus\x1b[39m  allows focus events." << std::endl;
}

int main(int totalArguments, const char** arguments)
{
    bool allowFocus = false;
    bool allowMouse = false;
    for (int offset = 0; offset < totalArguments; ++offset)
    {
        if (!std::strcmp(arguments[offset], "--help"))
        {
            writeHelpPage();
            return 0;
        }
    }
    for (int offset = 0; offset < totalArguments; ++offset)
    {
        if (!std::strcmp(arguments[offset], "--allow-focus"))
        {
            allowFocus = true;
        }
        else if (!std::strcmp(arguments[offset], "--allow-mouse"))
        {
            allowMouse = true;
        }
        else if (arguments[offset][0] == '-' && arguments[offset][1] == '-')
        {
            std::cerr << "\x1b[1;31m[ERROR]\x1b[22;39m Use of unrecognized flag \x1b[33m"
                      << arguments[offset] << "\x1b[39m." << std::endl
                      << "        For more info about options, use the \x1b[33m--help\x1b[39m flag."
                      << std::endl;
            return 1;
        }
    }
    if (allowFocus)
    {
        std::cout << "\x1b[?1004h";
    }
    if (allowMouse)
    {
        std::cout << "\x1b[?1003h\x1b[?1006h";
    }
    struct termios attributes;
    int flags = fcntl(STDIN_FILENO, F_GETFL);
    tcgetattr(STDIN_FILENO, &attributes);
    attributes.c_lflag &= ~(ICANON | ECHO | ISIG);
    attributes.c_iflag &= ~IXON;
    tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
    std::cout << "\x1b[?25lFor more info, use the \x1b[33m--help\x1b[39m option." << std::endl
              << "Press the [\x1b[32mEscape\x1b[39m] key to exit" << std::endl
              << std::endl;
    std::cout << "    ~> Focus events are " << BOOLEAN_STATE(allowFocus) << "." << std::endl
              << "    ~> Mouse events are " << BOOLEAN_STATE(allowMouse) << "." << std::endl
              << std::endl;
    std::cout << "\x1b[38;5;8mWaiting for a event to start...\x1b[39m";
    while (true)
    {
        int character;
        bool isEscaping = (character = std::getchar()) == 27;
        std::cout << "\x1b[2K\x1b[1G\x1b[1mScanned Codes:\x1b[22m " << character;
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
        int offset;
        for (offset = 0; (character = std::getchar()) != EOF; ++offset)
        {
            std::cout << " " << character;
        }
        fcntl(STDIN_FILENO, F_SETFL, flags);
        if (isEscaping && !offset)
        {
            break;
        }
    }
    std::cout << "\x1b[?25h" << std::endl;
    attributes.c_lflag |= ICANON | ECHO | ISIG;
    attributes.c_iflag |= IXON;
    tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
    if (allowFocus)
    {
        std::cout << "\x1b[?1004l";
    }
    if (allowMouse)
    {
        std::cout << "\x1b[?1003l\x1b[?1006l";
    }
    return 0;
}
