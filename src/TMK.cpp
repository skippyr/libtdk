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

#ifdef _WIN32
    Arguments::Arguments(int totalArguments, char** utf8Arguments, wchar_t** utf16Arguments)
        : m_totalArguments(totalArguments), m_utf8Arguments(utf8Arguments), m_utf16Arguments(utf16Arguments)
    {
    }

    std::wstring Arguments::GetUTF16ArgumentByOffset(std::size_t offset) const
    {
        if (offset >= m_totalArguments)
        {
            throw OutOfRangeException();
        }
        return m_utf16Arguments[offset];
    }
#else
    Arguments::Arguments(int totalArguments, char** utf8Arguments)
        : m_totalArguments(totalArguments), m_utf8Arguments(utf8Arguments)
    {
    }
#endif

    Arguments::~Arguments()
    {
#ifdef _WIN32
        LocalFree(m_utf16Arguments);
        for (int offset = 0; offset < m_totalArguments; ++offset)
        {
            delete[] m_utf8Arguments[offset];
        }
        delete[] m_utf8Arguments;
#endif
    }

    int Arguments::GetTotalArguments() const
    {
        return m_totalArguments;
    }

    std::string Arguments::GetUTF8ArgumentByOffset(std::size_t offset) const
    {
        if (offset >= m_totalArguments)
        {
            throw OutOfRangeException();
        }
        return m_utf8Arguments[offset];
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

    HexColor::HexColor(unsigned int code)
    {
        if (code > 0xffffff)
        {
            throw OutOfRangeException();
        }
        m_code = code;
    }

    HexColor::HexColor(RGBColor color) : m_code(color.GetRed() << 16 | color.GetGreen() << 8 | color.GetBlue())
    {
    }

    unsigned int HexColor::HexColor::GetCodeAsNumber() const
    {
        return m_code;
    }

    RGBColor::RGBColor(unsigned char red, unsigned char green, unsigned char blue)
        : m_red(red), m_green(green), m_blue(blue)
    {
    }

    RGBColor::RGBColor(HexColor color)
        : m_red(color.GetCodeAsNumber() >> 16 & 0xff), m_green(color.GetCodeAsNumber() >> 8 & 0xff),
          m_blue(color.GetCodeAsNumber() & 0xff)
    {
    }

    unsigned char RGBColor::GetRed() const
    {
        return m_red;
    }

    unsigned char RGBColor::GetGreen() const
    {
        return m_green;
    }

    unsigned char RGBColor::GetBlue() const
    {
        return m_blue;
    }

#ifdef _WIN32
    std::string Terminal::Encoding::ConvertUTF16ToUTF8(std::wstring utf16String)
    {
        int utf8Size = WideCharToMultiByte(CP_UTF8, 0, utf16String.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::unique_ptr<char[]> utf8Buffer = std::make_unique<char[]>(utf8Size);
        WideCharToMultiByte(CP_UTF8, 0, utf16String.c_str(), -1, utf8Buffer.get(), utf8Size, nullptr, nullptr);
        return utf8Buffer.get();
    }

    std::wstring Terminal::Encoding::ConvertUTF8ToUTF16(std::string utf8String)
    {
        int utf16Size = MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, nullptr, 0);
        std::unique_ptr<WCHAR[]> utf16Buffer = std::make_unique<WCHAR[]>(utf16Size);
        MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, utf16Buffer.get(), utf16Size);
        return utf16Buffer.get();
    }
#endif

    std::FILE* Terminal::Input::GetFile()
    {
        return stdin;
    }

    int Terminal::Input::GetFileNumber()
    {
        return 0;
    }

    bool Terminal::Input::IsTTY()
    {
        return IS_TTY(Input);
    }

    char Terminal::Input::ReadByte()
    {
        return std::getchar();
    }

    void Terminal::Output::Flush()
    {
        std::fflush(GetFile());
    }

    void Terminal::Output::WriteLine(std::string format, ...)
    {
        Setup::InitEnvironment();
        std::va_list arguments;
        va_start(arguments, format);
        if (std::vprintf(format.c_str(), arguments) < 0)
        {
            throw WideCharacterOrientationException();
        }
        std::putchar('\n');
        va_end(arguments);
    }

    void Terminal::Output::WriteLine()
    {
        Setup::InitEnvironment();
        if (std::putchar('\n') == EOF)
        {
            throw WideCharacterOrientationException();
        }
    }

    void Terminal::Output::Write(std::string format, ...)
    {
        Setup::InitEnvironment();
        std::va_list arguments;
        va_start(arguments, format);
        if (std::vprintf(format.c_str(), arguments) < 0)
        {
            throw WideCharacterOrientationException();
        }
        va_end(arguments);
    }

    std::FILE* Terminal::Output::GetFile()
    {
        return stdout;
    }

    int Terminal::Output::GetFileNumber()
    {
        return 1;
    }

    bool Terminal::Output::IsTTY()
    {
        Setup::InitEnvironment();
        return IS_TTY(Output);
    }

    void Terminal::Error::WriteLine(std::string format, ...)
    {
        Setup::InitEnvironment();
        Terminal::Output::Flush();
        std::va_list arguments;
        va_start(arguments, format);
        if (std::vfprintf(GetFile(), format.c_str(), arguments) < 0)
        {
            throw WideCharacterOrientationException();
        }
        std::fputc('\n', GetFile());
        va_end(arguments);
    }

    void Terminal::Error::WriteLine()
    {
        Setup::InitEnvironment();
        Terminal::Output::Flush();
        if (std::fputc('\n', GetFile()) == EOF)
        {
            throw WideCharacterOrientationException();
        }
    }

    void Terminal::Error::Write(std::string format, ...)
    {
        Setup::InitEnvironment();
        Terminal::Output::Flush();
        std::va_list arguments;
        va_start(arguments, format);
        if (std::vfprintf(GetFile(), format.c_str(), arguments) < 0)
        {
            throw WideCharacterOrientationException();
        }
        va_end(arguments);
    }

    std::FILE* Terminal::Error::GetFile()
    {
        return stderr;
    }

    int Terminal::Error::GetFileNumber()
    {
        return 2;
    }

    bool Terminal::Error::IsTTY()
    {
        return IS_TTY(Error);
    }

    Arguments Terminal::Process::GetArguments(int rawTotalArguments, char** rawArguments)
    {
#ifdef _WIN32
        LPWSTR* utf16Arguments = CommandLineToArgvW(GetCommandLineW(), &rawTotalArguments);
        char** utf8Arguments = new char*[rawTotalArguments];
        for (int offset = 0; offset < rawTotalArguments; ++offset)
        {
            int size = WideCharToMultiByte(CP_UTF8, 0, utf16Arguments[offset], -1, nullptr, 0, nullptr, nullptr);
            utf8Arguments[offset] = new char[size];
            WideCharToMultiByte(CP_UTF8, 0, utf16Arguments[offset], -1, utf8Arguments[offset], size, nullptr, nullptr);
        }
        return Arguments(rawTotalArguments, utf8Arguments, utf16Arguments);
#else
        return Arguments(rawTotalArguments, rawArguments);
#endif
    }

    void Terminal::Process::Exit(int exitCode)
    {
        std::exit(exitCode);
    }

    Dimensions Terminal::Window::GetDimensions()
    {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo) &&
            !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE), &bufferInfo))
        {
            throw NoValidTTYException();
        }
        return Dimensions(bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1,
                          bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1);
#else
        struct winsize size;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) && ioctl(STDIN_FILENO, TIOCGWINSZ, &size) &&
            ioctl(STDERR_FILENO, TIOCGWINSZ, &size))
        {
            throw NoValidTTYException();
        }
        return Dimensions(size.ws_col, size.ws_row);
#endif
    }

    void Terminal::Window::OpenAlternateWindow()
    {
        Setup::WriteEscapeSequence("\x1b[?1049h\x1b[2J\x1b[1;1H");
    }

    void Terminal::Window::CloseAlternateWindow()
    {
        Setup::WriteEscapeSequence("\x1b[?1049l");
    }

    void Terminal::Bell::Ring()
    {
        Setup::WriteEscapeSequence("\7");
    }

    void Terminal::Font::SetWeight(FontWeight weight)
    {
        Setup::WriteEscapeSequence("\x1b[22;%dm", static_cast<int>(weight));
    }

    void Terminal::Font::SetXColor(unsigned char color, FontLayer layer)
    {
        Setup::WriteEscapeSequence("\x1b[%d8;5;%hum", static_cast<int>(layer), color);
    }

    void Terminal::Font::SetXColor(XColor color, FontLayer layer)
    {
        Terminal::Font::SetXColor(static_cast<unsigned char>(color), layer);
    }

    void Terminal::Font::SetRGBColor(unsigned char red, unsigned char green, unsigned char blue, FontLayer layer)
    {
        Setup::WriteEscapeSequence("\x1b[%d8;2;%hu;%hu;%hum", static_cast<int>(layer), red, green, blue);
    }

    void Terminal::Font::SetRGBColor(RGBColor color, FontLayer layer)
    {
        SetRGBColor(color.GetRed(), color.GetGreen(), color.GetBlue(), layer);
    }

    void Terminal::Font::SetHexColor(unsigned int hex, FontLayer layer)
    {
        SetRGBColor(RGBColor(HexColor(hex)), layer);
    }

    void Terminal::Font::SetHexColor(HexColor color, FontLayer layer)
    {
        SetRGBColor(RGBColor(color), layer);
    }

    void Terminal::Font::ResetColors()
    {
        Setup::WriteEscapeSequence("\x1b[39;49m");
    }

    void Terminal::Font::ResetWeight()
    {
        Setup::WriteEscapeSequence("\x1b[22m");
    }

    void Terminal::Cursor::SetShape(CursorShape shape, bool isBlinking)
    {
        Setup::WriteEscapeSequence("\x1b[%d q", static_cast<int>(shape) - isBlinking);
    }

    void Terminal::Cursor::SetVisibility(bool isVisible)
    {
        Setup::WriteEscapeSequence("\x1b[?25%c", isVisible ? 'h' : 'l');
    }

    void Terminal::Cursor::ResetShape()
    {
        Setup::WriteEscapeSequence("\x1b[0 q");
    }
}
