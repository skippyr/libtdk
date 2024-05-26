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
        return key;                                                                                                    \
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

#ifdef _WIN32
static TDK::EventType GetWindowsEventType(INPUT_RECORD& record);
static TDK::KeyEvent ParseWindowsKeyEvent(INPUT_RECORD& record, HANDLE inputHandle);
#endif
static void PrepareStreamsAndCache();
static TDK::EventInfo ReadGenericEvent(int waitInMilliseconds, std::function<bool(TDK::EventInfo& eventInfo)> filter);
static int WriteANSISequence(const char* format, ...);

#ifdef _WIN32
static TDK::EventType GetWindowsEventType(INPUT_RECORD& record)
{
    if (record.EventType == WINDOW_BUFFER_SIZE_EVENT)
    {
        return TDK::EventType::WindowResize;
    }
    else if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown &&
             record.Event.KeyEvent.wVirtualKeyCode != VK_CONTROL && record.Event.KeyEvent.wVirtualKeyCode != VK_SHIFT &&
             record.Event.KeyEvent.wVirtualKeyCode != VK_MENU && record.Event.KeyEvent.wVirtualKeyCode != VK_CAPITAL &&
             record.Event.KeyEvent.wVirtualKeyCode != VK_NUMLOCK &&
             record.Event.KeyEvent.wVirtualKeyCode != VK_SCROLL &&
             (record.Event.KeyEvent.uChar.UnicodeChar ||
              !(record.Event.KeyEvent.dwControlKeyState &
                (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED | LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED | SHIFT_PRESSED))))
    {
        return TDK::EventType::Key;
    }
    return TDK::EventType::Invalid;
}

static TDK::KeyEvent ParseWindowsKeyEvent(INPUT_RECORD& record, HANDLE inputHandle)
{
    if (record.Event.KeyEvent.uChar.UnicodeChar)
    {
        TDK::KeyEvent keyEvent;
        DWORD totalEventsRead;
        int buffer;
        if ((buffer = record.Event.KeyEvent.uChar.UnicodeChar) <= 26 && buffer != TDK::Key::Tab &&
            buffer != TDK::Key::Enter)
        {
            keyEvent.m_key = buffer + 96;
        }
        else if (buffer >= HIGH_SURROGATE_START && buffer <= HIGH_SURROGATE_END)
        {
            ReadConsoleInputW(inputHandle, &record, 1, &totalEventsRead);
            ReadConsoleInputW(inputHandle, &record, 1, &totalEventsRead);
            *(reinterpret_cast<short*>(&buffer) + 1) = record.Event.KeyEvent.uChar.UnicodeChar;
            WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<wchar_t*>(&buffer), 2,
                                reinterpret_cast<char*>(&keyEvent.m_key), 4, NULL, NULL);
        }
        else
        {
            WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<wchar_t*>(&buffer), 1,
                                reinterpret_cast<char*>(&keyEvent.m_key), 4, NULL, NULL);
        }
        keyEvent.m_hasAlt = record.Event.KeyEvent.dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED);
        keyEvent.m_hasCtrl = record.Event.KeyEvent.dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED);
        return keyEvent;
    }
    PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_LEFT && record.Event.KeyEvent.wVirtualKeyCode <= VK_DOWN,
              record.Event.KeyEvent.wVirtualKeyCode - VK_LEFT + static_cast<int>(TDK::Key::LeftArrow));
    PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_PRIOR && record.Event.KeyEvent.wVirtualKeyCode <= VK_HOME,
              record.Event.KeyEvent.wVirtualKeyCode - VK_PRIOR + static_cast<int>(TDK::Key::PageUp));
    PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_INSERT && record.Event.KeyEvent.wVirtualKeyCode <= VK_DELETE,
              record.Event.KeyEvent.wVirtualKeyCode - VK_INSERT + static_cast<int>(TDK::Key::Insert));
    PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_F1 && record.Event.KeyEvent.wVirtualKeyCode <= VK_F12,
              record.Event.KeyEvent.wVirtualKeyCode - VK_F1 + static_cast<int>(TDK::Key::F1));
    return EOF;
}
#endif

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

static TDK::EventInfo ReadGenericEvent(int waitInMilliseconds, std::function<bool(TDK::EventInfo& eventInfo)> filter)
{
    PrepareStreamsAndCache();
    if (!IS_TTY(TDK::Stream::Input) || std::fwide(stdin, 0) > 0 ||
        (!IS_TTY(TDK::Stream::Output) && !IS_TTY(TDK::Stream::Error)))
    {
        return TDK::EventType::Invalid;
    }
#ifdef _WIN32
    HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    DWORD totalEventsRead;
    INPUT_RECORD record;
    TDK::EventInfo eventInfo;
    HANDLE timerHandle = nullptr;
    GetConsoleMode(inputHandle, &mode);
    SetConsoleMode(inputHandle, mode & ~ENABLE_PROCESSED_INPUT);
    while (true)
    {
        if (!waitInMilliseconds)
        {
            DWORD totalEventsAvailable;
            GetNumberOfConsoleInputEvents(inputHandle, &totalEventsAvailable);
            if (!totalEventsAvailable)
            {
                break;
            }
        }
        else if (waitInMilliseconds > 0)
        {
            if (!timerHandle)
            {
                timerHandle = CreateWaitableTimerW(nullptr, true, nullptr);
                LARGE_INTEGER duration;
                duration.QuadPart = -10000 * waitInMilliseconds;
                SetWaitableTimer(timerHandle, &duration, 0, nullptr, nullptr, false);
            }
            HANDLE handles[] = {timerHandle, inputHandle};
            int status = WaitForMultipleObjects(2, handles, false, INFINITE);
            if (status == WAIT_OBJECT_0)
            {
                eventInfo = TDK::EventType::TimeOut;
                CloseHandle(timerHandle);
                break;
            }
        }
        ReadConsoleInputW(inputHandle, &record, 1, &totalEventsRead);
        TDK::EventType type = GetWindowsEventType(record);
        eventInfo = type == TDK::EventType::WindowResize ? TDK::EventInfo(TDK::WindowResizeEvent())
                    : type == TDK::EventType::Key        ? TDK::EventInfo(ParseWindowsKeyEvent(record, inputHandle))
                                                         : TDK::EventType::None;
        if ((type == TDK::EventType::WindowResize || type == TDK::EventType::Key) && filter(eventInfo))
        {
            if (timerHandle)
            {
                CloseHandle(timerHandle);
            }
            break;
        }
    }
    SetConsoleMode(inputHandle, mode);
#endif
    return eventInfo;
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

TDK::KeyEvent::KeyEvent() : m_key(0), m_hasAlt(false), m_hasCtrl(false)
{
}

TDK::KeyEvent::KeyEvent(int key) : m_key(key), m_hasAlt(false), m_hasCtrl(false)
{
}

TDK::WindowResizeEvent::WindowResizeEvent()
{
    GetWindowDimensions(m_dimensions);
}

TDK::EventInfo::EventInfo() : m_type(EventType::None)
{
}

TDK::EventInfo::EventInfo(EventType type) : m_type(type)
{
}

TDK::EventInfo::EventInfo(KeyEvent keyEvent) : m_type(EventType::Key), m_keyEvent(keyEvent)
{
}

TDK::EventInfo::EventInfo(WindowResizeEvent windowResizeEvent)
    : m_type(EventType::WindowResize), m_windowResizeEvent(windowResizeEvent)
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

bool TDK::operator==(int code, Key key)
{
    return code == static_cast<int>(key);
}

bool TDK::operator!=(int code, Key key)
{
    return code != static_cast<int>(key);
}

bool TDK::operator>(int code, Key key)
{
    return code > static_cast<int>(key);
}

bool TDK::operator>=(int code, Key key)
{
    return code >= static_cast<int>(key);
}

bool TDK::operator<(int code, Key key)
{
    return code < static_cast<int>(key);
}

bool TDK::operator<=(int code, Key key)
{
    return code <= static_cast<int>(key);
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

TDK::EventInfo TDK::ReadEvent()
{
    return ReadGenericEvent(-1, [](EventInfo& eventInfo) { return true; });
}

TDK::EventInfo TDK::ReadEvent(unsigned int waitInMilliseconds)
{
    return ReadGenericEvent(waitInMilliseconds, [](EventInfo& eventInfo) { return true; });
}

TDK::EventInfo TDK::ReadEvent(unsigned int waitInMilliseconds, std::function<bool(EventInfo& eventInfo)> filter)
{
    return ReadGenericEvent(waitInMilliseconds, filter);
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
