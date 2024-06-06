#include "TMK.hpp"

#include <cstdarg>

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#ifdef _WIN32
#define TTY_CACHE(a_stream) (!!_isatty(a_stream::GetFileNumber()) << a_stream::GetFileNumber())
#else
#define TTY_CACHE(a_stream) (isatty(a_stream::GetFileNumber()) << a_stream::GetFileNumber())
#endif
#define CACHE_HAS_BEEN_FILLED_FLAG (1 << 7)

namespace TMK
{
    char g_cache = 0;

    class Setup
    {
    public:
        static void InitEnvironment()
        {
            if (!(g_cache & CACHE_HAS_BEEN_FILLED_FLAG))
            {
                g_cache |= TTY_CACHE(Terminal::Input) | TTY_CACHE(Terminal::Output) | TTY_CACHE(Terminal::Error) |
                           CACHE_HAS_BEEN_FILLED_FLAG;
            }
#ifdef _WIN32
            HANDLE handle;
            DWORD mode;
            SetConsoleOutputCP(CP_UTF8);
            (GetConsoleMode((handle = GetStdHandle(STD_OUTPUT_HANDLE)), &mode) ||
             GetConsoleMode((handle = GetStdHandle(STD_ERROR_HANDLE)), &mode)) &&
                SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
        }

    private:
        Setup() = delete;
    };

    int Terminal::Input::GetFileNumber()
    {
        return 0;
    }

    int Terminal::Output::WriteLine(std::string format, ...)
    {
        Setup::InitEnvironment();
        std::va_list arguments;
        va_start(arguments, format);
        int totalBytesWritten = std::vprintf(format.c_str(), arguments);
        va_end(arguments);
        return -(totalBytesWritten < 0);
    }

    int Terminal::Output::GetFileNumber()
    {
        return 1;
    }

    int Terminal::Error::GetFileNumber()
    {
        return 2;
    }
}
