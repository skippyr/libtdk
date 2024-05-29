#include <cstring>
#include <iostream>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

static void writeHelpPage()
{
    std::cout << "Usage: scan-codes [OPTION]..." << std::endl
              << "Writes the scan codes related to key, focus and mouse events." << std::endl
              << "By default, it only handle key events." << std::endl
              << std::endl
              << "OPTIONS" << std::endl
              << "    --allow-mouse  allow mouse events." << std::endl
              << "    --allow-focus  allow focus events." << std::endl;
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
        else if (!std::strcmp(arguments[offset], "--allow-focus"))
        {
            allowFocus = true;
        }
        else if (!std::strcmp(arguments[offset], "--allow-mouse"))
        {
            allowMouse = true;
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
    std::cout << "Waiting for events to dump." << std::endl
              << "Press the Escape key to exit" << std::endl;
    while (true)
    {
        int character;
        bool isEscaping = (character = std::getchar()) == 27;
        std::cout << character;
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
        int offset;
        for (offset = 0; (character = std::getchar()) != EOF; ++offset)
        {
            std::cout << " " << character;
        }
        fcntl(STDIN_FILENO, F_SETFL, flags);
        std::cout << std::endl;
        if (isEscaping && !offset)
        {
            break;
        }
    }
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
