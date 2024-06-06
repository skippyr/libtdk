#include "TMK.hpp"

#include <cstdarg>

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

#ifdef _WIN32
#define TTY_CACHE(a_stream) (!!_isatty(a_stream::GetFileNumber()) << a_stream::GetFileNumber())
#else
#define TTY_CACHE(a_stream) (isatty(a_stream::GetFileNumber()) << a_stream::GetFileNumber())
#endif
#define IS_TTY(a_stream) (g_cache & 1 << a_stream::GetFileNumber())
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
#ifdef _WIN32
                HANDLE handle;
                DWORD mode;
                SetConsoleOutputCP(CP_UTF8);
                (GetConsoleMode((handle = GetStdHandle(STD_OUTPUT_HANDLE)), &mode) ||
                 GetConsoleMode((handle = GetStdHandle(STD_ERROR_HANDLE)), &mode)) &&
                    SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
                g_cache |= TTY_CACHE(Terminal::Input) | TTY_CACHE(Terminal::Output) | TTY_CACHE(Terminal::Error) |
                           CACHE_HAS_BEEN_FILLED_FLAG;
            }
        }

        static int WriteEscapeSequence(std::string format, ...)
        {
            Setup::InitEnvironment();
            if (!IS_TTY(Terminal::Output) && !IS_TTY(Terminal::Error))
            {
                return -1;
            }
            std::va_list arguments;
            va_start(arguments, format);
            int totalBytesWritten =
                std::vfprintf(IS_TTY(Terminal::Output) ? Terminal::Output::GetFile() : Terminal::Error::GetFile(),
                              format.c_str(), arguments);
            va_end(arguments);
            return -(totalBytesWritten < 0);
        }

    private:
        Setup() = delete;
    };

    std::FILE* Terminal::Input::GetFile()
    {
        return stdin;
    }

    int Terminal::Input::GetFileNumber()
    {
        return 0;
    }

    void Terminal::Output::Flush()
    {
        std::fflush(GetFile());
    }

    int Terminal::Output::WriteLine(std::string format, ...)
    {
        Setup::InitEnvironment();
        std::va_list arguments;
        va_start(arguments, format);
        int totalBytesWritten = std::vprintf(format.c_str(), arguments);
        std::putchar('\n');
        va_end(arguments);
        return -(totalBytesWritten < 0);
    }

    int Terminal::Output::WriteLine()
    {
        Setup::InitEnvironment();
        return -(std::putchar('\n') == EOF);
    }

    int Terminal::Output::Write(std::string format, ...)
    {
        Setup::InitEnvironment();
        std::va_list arguments;
        va_start(arguments, format);
        int totalBytesWritten = std::vprintf(format.c_str(), arguments);
        va_end(arguments);
        return -(totalBytesWritten < 0);
    }

    std::FILE* Terminal::Output::GetFile()
    {
        return stdout;
    }

    int Terminal::Output::GetFileNumber()
    {
        return 1;
    }

    int Terminal::Error::WriteLine(std::string format, ...)
    {
        Setup::InitEnvironment();
        Terminal::Output::Flush();
        std::va_list arguments;
        va_start(arguments, format);
        int totalBytesWritten = std::vfprintf(GetFile(), format.c_str(), arguments);
        std::fputc('\n', GetFile());
        va_end(arguments);
        return -(totalBytesWritten < 0);
    }

    int Terminal::Error::WriteLine()
    {
        Setup::InitEnvironment();
        Terminal::Output::Flush();
        return -(std::fputc('\n', GetFile()) == EOF);
    }

    int Terminal::Error::Write(std::string format, ...)
    {
        Setup::InitEnvironment();
        Terminal::Output::Flush();
        std::va_list arguments;
        va_start(arguments, format);
        int totalBytesWritten = std::vfprintf(GetFile(), format.c_str(), arguments);
        va_end(arguments);
        return -(totalBytesWritten < 0);
    }

    Terminal::Process::Arguments::Arguments(int totalArguments, char** arguments)
        : m_totalArguments(totalArguments), m_arguments(arguments)
    {
    }

    Terminal::Process::Arguments::~Arguments()
    {
#ifdef _WIN32
        for (int offset = 0; offset < m_totalArguments; ++offset)
        {
            delete[] m_arguments[offset];
        }
        delete[] m_arguments;
#endif
    }

    int Terminal::Process::Arguments::GetTotalArguments() const
    {
        return m_totalArguments;
    }

    std::string Terminal::Process::Arguments::operator[](std::size_t offset)
    {
        return offset < m_totalArguments ? m_arguments[offset] : "";
    }

    std::FILE* Terminal::Error::GetFile()
    {
        return stderr;
    }

    int Terminal::Error::GetFileNumber()
    {
        return 2;
    }

    Terminal::Dimensions::Dimensions() : m_width(0), m_height(0)
    {
    }

    Terminal::Dimensions::Dimensions(unsigned short width, unsigned short height) : m_width(width), m_height(height)
    {
    }

    unsigned short Terminal::Dimensions::GetWidth() const
    {
        return m_width;
    }

    unsigned short Terminal::Dimensions::GetHeight() const
    {
        return m_height;
    }

    int Terminal::Window::GetDimensions(Dimensions& dimensions)
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

    void Terminal::Bell::Ring()
    {
        Setup::WriteEscapeSequence("\7");
    }
}
