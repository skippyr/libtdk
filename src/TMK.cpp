#include "TMK.hpp"

#include <cstdarg>
#include <cstdio>

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#else
#include <sys/ioctl.h>
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
    static int WriteANSISequence(const char* format, ...);

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

    static int WriteANSISequence(const char* format, ...)
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

    Dimensions::Dimensions() : m_totalColumns(0), m_totalRows(0)
    {
    }

    Dimensions::Dimensions(unsigned short totalColumns, unsigned short totalRows)
        : m_totalColumns(totalColumns), m_totalRows(totalRows)
    {
    }

    unsigned short Dimensions::GetTotalColumns() const
    {
        return m_totalColumns;
    }

    unsigned short Dimensions::GetTotalRows() const
    {
        return m_totalRows;
    }

    void Font::SetWeight(Weight weight)
    {
        WriteANSISequence(weight == Weight::Default ? "\x1b[22m" : "\x1b[22;%dm", static_cast<int>(weight));
    }

    void Font::SetEffect(Effect effect, bool isToEnable)
    {
        SetEffect(static_cast<int>(effect), isToEnable);
    }

    void Font::SetEffect(int effect, bool isToEnable)
    {
        for (int code = 3; code < 10; ++code)
        {
            if (effect & 1 << code)
            {
                WriteANSISequence("\x1b[%dm", isToEnable ? code : code + 20);
            }
        }
    }

    int Window::GetDimensions(Dimensions& dimensions)
    {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo) &&
            !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE), &bufferInfo))
        {
            return -1;
        }
        dimensions = Dimensions(bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1,
                                bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1);
#else
        struct winsize size;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) && ioctl(STDIN_FILENO, TIOCGWINSZ, &size) &&
            ioctl(STDERR_FILENO, TIOCGWINSZ, &size))
        {
            return -1;
        }
        dimensions = Dimensions(size.ws_col, size.ws_row);
#endif
        return 0;
    }

    void Window::SetTitle(const char* title)
    {
        WriteANSISequence("\x1b]0;%s\7", title);
    }

    int operator|(Effect effect0, Effect effect1)
    {
        return static_cast<int>(effect0) | static_cast<int>(effect1);
    }
}
