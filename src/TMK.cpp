#include "TMK.hpp"

#include <cstdarg>
#include <cstdio>

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#ifdef _WIN32
#define TTY_CACHE(stream) (!!_isatty(stream::GetFileNo()) << stream::GetFileNo())
#else
#define TTY_CACHE(stream) (isatty(stream::GetFileNo()) << stream::GetFileNo())
#endif
#define IS_TTY(stream) (g_cache & 1 << stream::GetFileNo())

namespace TMK
{
    static void CacheTTYStatus();
    static int WriteANSI(const char* format, ...);

    static char g_cache = 0;

    static void CacheTTYStatus()
    {
        if (!(g_cache & 1 << 7))
        {
            g_cache |= TTY_CACHE(Input) | TTY_CACHE(Output) | TTY_CACHE(Error) | 1 << 7;
#ifdef _WIN32
            HANDLE handle;
            DWORD mode;
            SetConsoleOutputCP(CP_UTF8);
            (GetConsoleMode((handle = GetStdHandle(STD_OUTPUT_HANDLE)), &mode) ||
             GetConsoleMode((handle = GetStdHandle(STD_ERROR_HANDLE)), &mode)) &&
                SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
        }
    }

    static int WriteANSI(const char* format, ...)
    {
        CacheTTYStatus();
        if (!IS_TTY(Output) && !IS_TTY(Error))
        {
            return -1;
        }
        std::va_list arguments;
        va_start(arguments, format);
        int totalBytesWritten = std::vfprintf(IS_TTY(Output) ? stdout : stderr, format, arguments);
        va_end(arguments);
        return -(totalBytesWritten < 0);
    }

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
        CacheTTYStatus();
        std::va_list arguments;
        va_start(arguments, format);
        int totalBytesWritten = std::vprintf(format, arguments);
        std::putchar('\n');
        va_end(arguments);
        return -(totalBytesWritten < 0);
    }

    int Output::WriteLine()
    {
        CacheTTYStatus();
        return -(std::putchar('\n') == EOF);
    }

    int Output::Write(const char* format, ...)
    {
        CacheTTYStatus();
        std::va_list arguments;
        va_start(arguments, format);
        int totalBytesWritten = std::vprintf(format, arguments);
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

    int Error::WriteLine(const char* format, ...)
    {
        CacheTTYStatus();
        std::va_list arguments;
        va_start(arguments, format);
        int totalBytesWritten = std::vfprintf(stderr, format, arguments);
        std::fputc('\n', stderr);
        va_end(arguments);
        return -(totalBytesWritten < 0);
    }

    int Error::WriteLine()
    {
        CacheTTYStatus();
        return -(std::fputc('\n', stderr) == EOF);
    }

    int Error::Write(const char* format, ...)
    {
        CacheTTYStatus();
        std::va_list arguments;
        va_start(arguments, format);
        int totalBytesWritten = std::vfprintf(stderr, format, arguments);
        va_end(arguments);
        return -(totalBytesWritten < 0);
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

    void Font::SetWeight(Weight weight)
    {
        WriteANSI(weight == Weight::Default ? "\x1b[22m" : "\x1b[22;%dm", static_cast<int>(weight));
    }
}
