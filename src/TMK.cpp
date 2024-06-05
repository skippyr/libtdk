#include "TMK.hpp"

#include <cstdarg>

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#else
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
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

    void Input::ClearBuffer()
    {
#ifdef _WIN32
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
#else
        struct termios attributes;
        int flags = fcntl(STDIN_FILENO, F_GETFL);
        if (tcgetattr(STDIN_FILENO, &attributes))
        {
            return;
        }
        attributes.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
        while (std::getchar() != EOF)
        {
        }
        attributes.c_lflag |= ICANON | ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
        fcntl(STDIN_FILENO, F_SETFL, flags);
#endif
    }

    std::FILE* Input::GetFile()
    {
        return stdin;
    }

    int Input::GetFileNo()
    {
        return 0;
    }

    int Output::WriteLine(std::string format, ...)
    {
        CacheTTYStatus();
        std::va_list arguments;
        va_start(arguments, format);
        int totalBytesWritten = std::vprintf(format.c_str(), arguments);
        std::putchar('\n');
        va_end(arguments);
        return -(totalBytesWritten < 0);
    }

    int Output::WriteLine()
    {
        CacheTTYStatus();
        return -(std::putchar('\n') == EOF);
    }

    int Output::Write(std::string format, ...)
    {
        CacheTTYStatus();
        std::va_list arguments;
        va_start(arguments, format);
        int totalBytesWritten = std::vprintf(format.c_str(), arguments);
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

    int Error::WriteLine(std::string format, ...)
    {
        CacheTTYStatus();
        std::fflush(stdout);
        std::va_list arguments;
        va_start(arguments, format);
        int totalBytesWritten = std::vfprintf(stderr, format.c_str(), arguments);
        std::fputc('\n', stderr);
        va_end(arguments);
        return -(totalBytesWritten < 0);
    }

    int Error::WriteLine()
    {
        CacheTTYStatus();
        std::fflush(stdout);
        return -(std::fputc('\n', stderr) == EOF);
    }

    int Error::Write(std::string format, ...)
    {
        CacheTTYStatus();
        std::fflush(stdout);
        std::va_list arguments;
        va_start(arguments, format);
        int totalBytesWritten = std::vfprintf(stderr, format.c_str(), arguments);
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

    Arguments::Arguments(int totalArguments, char** arguments)
        : m_totalArguments(totalArguments), m_arguments(arguments)
    {
    }

    Arguments::~Arguments()
    {
#ifdef _WIN32
        for (int offset = 0; offset < m_totalArguments; ++offset)
        {
            delete[] m_arguments[offset];
        }
        delete[] m_arguments;
#endif
    }

    int Arguments::GetTotalArguments() const
    {
        return m_totalArguments;
    }

    const char* Arguments::GetArgumentByOffset(std::size_t offset) const
    {
        return offset < m_totalArguments ? m_arguments[offset] : nullptr;
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

    Arguments Environment::GetArguments(int rawTotalArguments, char** rawArguments)
    {
#ifdef _WIN32
        LPWSTR* argumentsUTF16 = CommandLineToArgvW(GetCommandLineW(), &rawTotalArguments);
        char** items = new char*[rawTotalArguments];
        for (int offset = 0; offset < rawTotalArguments; ++offset)
        {
            int size = WideCharToMultiByte(CP_UTF8, 0, argumentsUTF16[offset], -1, nullptr, 0, nullptr, nullptr);
            items[offset] = new char[size];
            WideCharToMultiByte(CP_UTF8, 0, argumentsUTF16[offset], -1, items[offset], size, nullptr, nullptr);
        }
        LocalFree(argumentsUTF16);
        return Arguments(rawTotalArguments, items);
#else
        return Arguments(rawTotalArguments, rawArguments);
#endif
    }

    void Font::SetWeight(Weight weight)
    {
        WriteANSISequence(weight == Weight::Default ? "\x1b[22m" : "\x1b[22;%dm", static_cast<int>(weight));
    }

    void Font::SetEffect(Effect effect)
    {
        SetEffect(static_cast<int>(effect));
    }

    void Font::SetEffect(int effect)
    {
        for (int code = 3; code < 10; ++code)
        {
            if (effect & 1 << code)
            {
                WriteANSISequence("\x1b[%dm", code);
            }
        }
    }

    void Font::ResetEffects()
    {
        for (int code = 23; code < 30; ++code)
        {
            if (code != 26)
            {
                WriteANSISequence("\x1b[%dm", code);
            }
        }
    }

    void Font::SetXColor(XColor color, Layer layer)
    {
        SetXColor(static_cast<int>(color), layer);
    }

    void Font::SetXColor(int color, Layer layer)
    {
        WriteANSISequence("\x1b[%d8;5;%dm", static_cast<int>(layer), color);
    }

    void Font::SetHexColor(int hex, Layer layer)
    {
        SetRGBColor(hex >> 16 & 0xff, hex >> 8 & 0xff, hex & 0xff, layer);
    }

    void Font::SetRGBColor(int red, int green, int blue, Layer layer)
    {
        WriteANSISequence("\x1b[%d8;2;%d;%d;%dm", static_cast<int>(layer), red, green, blue);
    }

    void Font::ResetColors()
    {
        WriteANSISequence("\x1b[39;49m");
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

    void Window::SetTitle(std::string title)
    {
        WriteANSISequence("\x1b]0;%s\7", title.c_str());
    }

    void Window::OpenAlternateBuffer()
    {
        WriteANSISequence("\x1b[?1049h\x1b[2J\x1b[1;1H");
    }

    void Window::CloseAlternateBuffer()
    {
        WriteANSISequence("\x1b[?1049l");
    }

    void Window::Clear()
    {
        WriteANSISequence("\x1b[2J\x1b[1;1H");
    }

    void Bell::Ring()
    {
        WriteANSISequence("\7");
    }

    int operator|(Effect effect0, Effect effect1)
    {
        return static_cast<int>(effect0) | static_cast<int>(effect1);
    }
}
