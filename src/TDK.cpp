#include "TDK.hpp"

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

static char g_cache = 0;

#define IS_TTY(stream) static_cast<bool>(g_cache & 1 << static_cast<int>(stream))
#define CHECK_STREAM_TTY_STATUS()                                                                                      \
    PrepareStreamsAndCache();                                                                                          \
    if ((stream.rdbuf() == std::cout.rdbuf() && !IS_TTY(TDK::Stream::Output)) ||                                       \
        (stream.rdbuf() == std::cerr.rdbuf() && !IS_TTY(TDK::Stream::Error)))                                          \
    {                                                                                                                  \
        return stream;                                                                                                 \
    }
#define HAS_CACHED_TTY_FLAG (1 << 7)
#define PARSE_KEY(condition, key)                                                                                      \
    if (condition)                                                                                                     \
    {                                                                                                                  \
        event.m_key = key;                                                                                             \
        break;                                                                                                         \
    }
#ifdef _WIN32
#define TTY_CACHE(stream)                                                                                              \
    (!!(_isatty(_fileno(!static_cast<int>(stream)       ? stdin                                                        \
                        : static_cast<int>(stream) == 1 ? stdout                                                       \
                                                        : stderr)))                                                    \
     << static_cast<int>(stream))
#else
#define TTY_CACHE(stream)                                                                                              \
    (isatty(fileno(!static_cast<int>(stream)       ? stdin                                                             \
                   : static_cast<int>(stream) == 1 ? stdout                                                            \
                                                   : stderr))                                                          \
     << static_cast<int>(stream))
#endif

static void PrepareStreamsAndCache();
static int WriteANSISequence(const char* format, ...);

static void PrepareStreamsAndCache()
{
    if (!(g_cache & HAS_CACHED_TTY_FLAG))
    {
        g_cache = TTY_CACHE(TDK::Stream::Input) | TTY_CACHE(TDK::Stream::Output) | TTY_CACHE(TDK::Stream::Error) |
                  HAS_CACHED_TTY_FLAG;
    }
#ifdef _WIN32
    HANDLE handle;
    DWORD mode;
    (GetConsoleMode((handle = GetStdHandle(STD_OUTPUT_HANDLE)), &mode) ||
     GetConsoleMode((handle = GetStdHandle(STD_ERROR_HANDLE)), &mode)) &&
        SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

static int WriteANSISequence(const char* format, ...)
{
    PrepareStreamsAndCache();
    if (!IS_TTY(TDK::Stream::Output) && !IS_TTY(TDK::Stream::Error))
    {
        return -1;
    }
    std::va_list arguments;
    va_start(arguments, format);
    int totalBytesWritten = std::vfprintf(IS_TTY(TDK::Stream::Output) ? stdout : stderr, format, arguments);
    va_end(arguments);
    return -(totalBytesWritten < 0);
}

TDK::Coordinate::Coordinate() : m_column(0), m_row(0)
{
}

TDK::Coordinate::Coordinate(unsigned short column, unsigned short row) : m_column(column), m_row(row)
{
}

TDK::Dimensions::Dimensions() : m_totalColumns(0), m_totalRows(0)
{
}

TDK::Dimensions::Dimensions(unsigned short totalColumns, unsigned short totalRows)
    : m_totalColumns(totalColumns), m_totalRows(totalRows)
{
}

TDK::Effect::Effect(int code, bool isToEnable) : m_code(code), m_isToEnable(isToEnable)
{
}

TDK::Effect::Effect(TDK::EffectCode code, bool isToEnable)
    : m_code(1 << static_cast<int>(code)), m_isToEnable(isToEnable)
{
}

TDK::Event::Event(EventType type) : m_type(type)
{
}

TDK::HexColor::HexColor(unsigned int code, Layer layer)
    : m_code((std::min)(static_cast<int>(code), 0xffffff)), m_layer(layer)
{
}

TDK::HexColor TDK::HexColor::Invert()
{
    HexColor color = *this;
    color.m_layer = color.m_layer == TDK::Layer::Foreground ? TDK::Layer::Background : TDK::Layer::Foreground;
    return color;
}

TDK::RGBColor::RGBColor(unsigned char red, unsigned char green, unsigned char blue, Layer layer)
    : m_red(red), m_green(green), m_blue(blue), m_layer(layer)
{
}

TDK::RGBColor::RGBColor(HexColor color)
    : m_red(color.m_code >> 16 & 0xff), m_green(color.m_code >> 8 & 0xff), m_blue(color.m_code & 0xff),
      m_layer(color.m_layer)
{
}

TDK::RGBColor TDK::RGBColor::Invert()
{
    RGBColor color = *this;
    color.m_layer = color.m_layer == TDK::Layer::Foreground ? TDK::Layer::Background : TDK::Layer::Foreground;
    return color;
}

TDK::XColor::XColor(unsigned char code, Layer layer) : m_code(code), m_layer(layer)
{
}

TDK::XColor::XColor(XColorCode code, Layer layer) : m_code(static_cast<int>(code)), m_layer(layer)
{
}

TDK::XColor TDK::XColor::Invert()
{
    XColor color = *this;
    color.m_layer = color.m_layer == TDK::Layer::Foreground ? TDK::Layer::Background : TDK::Layer::Foreground;
    return color;
}

std::ostream& TDK::operator<<(std::ostream& stream, Effect effect)
{
    for (int code = 0; code < 10; ++code)
    {
        if (effect.m_code & 1 << code)
        {
            std::cout << "\x1b[" << code + (!effect.m_isToEnable * 20) << "m";
        }
    }
    return stream;
}

std::ostream& TDK::operator<<(std::ostream& stream, HexColor color)
{
    return stream << RGBColor(color);
}

std::ostream& TDK::operator<<(std::ostream& stream, RGBColor color)
{
    CHECK_STREAM_TTY_STATUS();
    return stream << "\x1b[" << static_cast<int>(color.m_layer) << "8;2;" << static_cast<int>(color.m_red) << ";"
                  << static_cast<int>(color.m_green) << ";" << static_cast<int>(color.m_blue) << "m";
}

std::ostream& TDK::operator<<(std::ostream& stream, XColor color)
{
    CHECK_STREAM_TTY_STATUS();
    return color.m_code == static_cast<int>(TDK::XColorCode::Default)
               ? stream << "\x1b[" << static_cast<int>(color.m_layer) << "9m"
               : stream << "\x1b[" << static_cast<int>(color.m_layer) << "8;5;" << color.m_code << "m";
}

std::ostream& TDK::operator<<(std::ostream& stream, Weight weight)
{
    CHECK_STREAM_TTY_STATUS();
    return weight == Weight::Default ? stream << "\x1b[22m" : stream << "\x1b[22;" << static_cast<int>(weight) << "m";
}

int TDK::operator|(EffectCode code0, EffectCode code1)
{
    return 1 << static_cast<int>(code0) | 1 << static_cast<int>(code1);
}

bool TDK::operator==(Event event0, Event event1)
{
    return event0.m_type == event1.m_type;
}

void TDK::ClearCursorLine()
{
    WriteANSISequence("\x1b[2K\x1b[1G");
}

void TDK::ClearInputBuffer()
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

void TDK::ClearWindow()
{
    WriteANSISequence("\x1b[2J\x1b[1;1H");
}

void TDK::CloseAlternateWindow()
{
    WriteANSISequence("\x1b[?1049l");
}

int TDK::GetCursorCoordinate(Coordinate& coordinate)
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo) &&
        !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE), &bufferInfo))
    {
        return -1;
    }
    coordinate.m_column = bufferInfo.dwCursorPosition.X - bufferInfo.srWindow.Left;
    coordinate.m_row = bufferInfo.dwCursorPosition.Y - bufferInfo.srWindow.Top;
#else
    struct termios attributes;
    ClearInputBuffer();
    if (WriteANSISequence("\x1b[6n") || tcgetattr(STDIN_FILENO, &attributes))
    {
        return -1;
    }
    attributes.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
    int totalMatchesRead = std::scanf("\x1b[%hu;%huR", &coordinate.m_row, &coordinate.m_column);
    attributes.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
    if (totalMatchesRead != 2)
    {
        return -1;
    }
    --coordinate.m_column;
    --coordinate.m_row;
#endif
    return 0;
}

int TDK::GetWindowDimensions(Dimensions& dimensions)
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo) &&
        !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE), &bufferInfo))
    {
        return -1;
    }
    dimensions.m_totalColumns = bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1;
    dimensions.m_totalRows = bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1;
#else
    struct winsize size;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) && ioctl(STDIN_FILENO, TIOCGWINSZ, &size) &&
        ioctl(STDERR_FILENO, TIOCGWINSZ, &size))
    {
        return -1;
    }
    dimensions.m_totalColumns = size.ws_col;
    dimensions.m_totalRows = size.ws_row;
#endif
    return 0;
}

bool TDK::IsTTY(Stream stream)
{
    PrepareStreamsAndCache();
    return IS_TTY(stream);
}

void TDK::OpenAlternateWindow()
{
    WriteANSISequence("\x1b[?1049h\x1b[2J\x1b[1;1H");
}

#ifdef _WIN32
TDK::EventType GetWindowsEventType(INPUT_RECORD& record)
{
    if (record.EventType == WINDOW_BUFFER_SIZE_EVENT)
    {
        return TDK::EventType::WindowResize;
    }
    else if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown &&
             record.Event.KeyEvent.wVirtualKeyCode != VK_CONTROL && record.Event.KeyEvent.wVirtualKeyCode != VK_SHIFT &&
             record.Event.KeyEvent.wVirtualKeyCode != VK_MENU && record.Event.KeyEvent.wVirtualKeyCode != VK_CAPITAL &&
             record.Event.KeyEvent.wVirtualKeyCode != VK_NUMLOCK && record.Event.KeyEvent.wVirtualKeyCode != VK_SCROLL)
    {
        return TDK::EventType::Key;
    }
    return TDK::EventType::Invalid;
}
#endif

TDK::Event TDK::ReadEvent()
{
    return TDK::ReadEvent(-1);
}

TDK::Event TDK::ReadEvent(int waitInMilliseconds)
{
    PrepareStreamsAndCache();
    if (!IS_TTY(TDK::Stream::Input) || std::fwide(stdin, 0) > 0 ||
        (!IS_TTY(TDK::Stream::Output) && !IS_TTY(TDK::Stream::Error)))
    {
        return EventType::Invalid;
    }
#ifdef _WIN32
    HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    DWORD totalEventsRead;
    INPUT_RECORD record;
    GetConsoleMode(inputHandle, &mode);
    SetConsoleMode(inputHandle, mode & ~ENABLE_PROCESSED_INPUT);
    if (waitInMilliseconds < 0)
    {
        while (true)
        {
            ReadConsoleInputW(inputHandle, &record, 1, &totalEventsRead);
            EventType type = GetWindowsEventType(record);
            if (type == EventType::WindowResize)
            {
                SetConsoleMode(inputHandle, mode);
                return EventType::WindowResize;
            }
            else if (type == EventType::Key)
            {
                break;
            }
        }
    }
    else if (!waitInMilliseconds)
    {
        while (true)
        {
            DWORD totalEventsAvailable;
            GetNumberOfConsoleInputEvents(inputHandle, &totalEventsAvailable);
            if (!totalEventsAvailable)
            {
                SetConsoleMode(inputHandle, mode);
                return EventType::None;
            }
            ReadConsoleInputW(inputHandle, &record, 1, &totalEventsRead);
            EventType type = GetWindowsEventType(record);
            if (type == EventType::WindowResize)
            {
                SetConsoleMode(inputHandle, mode);
                return EventType::WindowResize;
            }
            else if (type == EventType::Key)
            {
                break;
            }
        }
    }
    else
    {
        HANDLE timerHandle = CreateWaitableTimerW(nullptr, true, nullptr);
        LARGE_INTEGER duration;
        duration.QuadPart = -10000 * waitInMilliseconds;
        SetWaitableTimer(timerHandle, &duration, 0, nullptr, nullptr, false);
        HANDLE handles[] = {timerHandle, inputHandle};
        while (true)
        {
            int status = WaitForMultipleObjects(2, handles, false, INFINITE);
            if (status == WAIT_OBJECT_0)
            {
                SetConsoleMode(inputHandle, mode);
                CloseHandle(timerHandle);
                return EventType::TimeOut;
            }
            else if (status == WAIT_OBJECT_0 + 1)
            {
                ReadConsoleInputW(inputHandle, &record, 1, &totalEventsRead);
                EventType type = GetWindowsEventType(record);
                if (type == EventType::WindowResize)
                {
                    SetConsoleMode(inputHandle, mode);
                    CloseHandle(timerHandle);
                    return EventType::WindowResize;
                }
                else if (type == EventType::Key)
                {
                    CloseHandle(timerHandle);
                    break;
                }
            }
        }
    }
    SetConsoleMode(inputHandle, mode);
#endif
    return EventType::Key;
}

void TDK::RingBell()
{
    WriteANSISequence("\7");
}

void TDK::SetCursorCoordinate(unsigned short column, unsigned short row)
{
    WriteANSISequence("\x1b[%hu;%huH", row + 1, column + 1);
}

void TDK::SetCursorCoordinate(Coordinate& coordinate)
{
    SetCursorCoordinate(coordinate.m_column, coordinate.m_row);
}

void TDK::SetCursorShape(CursorShape shape)
{
    WriteANSISequence("\x1b[%d q", static_cast<int>(shape));
}

void TDK::SetWindowTitle(std::string title)
{
    WriteANSISequence("\x1b]0;%s\7", title.c_str());
}

void TDK::SetCursorVisibility(bool isToShow)
{
    WriteANSISequence("\x1b[?25%c", isToShow ? 'h' : 'l');
}
