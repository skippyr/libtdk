#include "TMK.hpp"

#include <cstdarg>
#include <cstdio>

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#endif

#ifdef _WIN32
#define TTY_CACHE(stream) (!!_isatty(stream::GetFileNo()) << stream::GetFileNo())
#else
#define TTY_CACHE(stream) isatty(fileno(stream::GetFile()))
#endif
#define IS_TTY(stream) (g_cache & 1 << stream::GetFileNo())

namespace TMK
{
    static void CacheTTYStatus();

    static char g_cache = 0;

    static void CacheTTYStatus()
    {
        if (!(g_cache & 1 << 7))
        {
            g_cache |= TTY_CACHE(Stream::Input) | TTY_CACHE(Stream::Output) | TTY_CACHE(Stream::Error) | 1 << 7;
#ifdef _WIN32
            HANDLE handle;
            DWORD mode;
            (GetConsoleMode((handle = GetStdHandle(STD_OUTPUT_HANDLE)), &mode) ||
             GetConsoleMode((handle = GetStdHandle(STD_ERROR_HANDLE)), &mode)) &&
                SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
        }
    }

    namespace Stream
    {
        bool Input::IsTTY()
        {
            CacheTTYStatus();
            return IS_TTY(Input);
        }

        std::FILE* Input::GetFile()
        {
            return stdin;
        }

        int Input::GetFileNo()
        {
            return 0;
        }

        int Output::WriteLine(const char* format, ...)
        {
            std::va_list arguments;
            va_start(arguments, format);
            int totalBytesWritten = std::vprintf(format, arguments);
            std::putchar('\n');
            va_end(arguments);
            return -(totalBytesWritten < 0);
        }

        bool Output::IsTTY()
        {
            CacheTTYStatus();
            return IS_TTY(Output);
        }

        std::FILE* Output::GetFile()
        {
            return stdout;
        }

        int Output::GetFileNo()
        {
            return 1;
        }

        bool Error::IsTTY()
        {
            CacheTTYStatus();
            return IS_TTY(Error);
        }

        std::FILE* Error::GetFile()
        {
            return stderr;
        }

        int Error::GetFileNo()
        {
            return 2;
        }
    }
}
