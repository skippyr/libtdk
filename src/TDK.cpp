#include "TDK.hpp"

/* @brief A cache that contains the TTY statuses of the standard terminal streams. */
static char g_cache = 0;

/*
 * @brief Looks into cache to check if a standard terminal stream is a TTY.
 * @param a_stream The stream to be checked. It must be a value from the TDK::Stream enum class.
 * @returns The check result.
 */
#define IS_TTY(stream) static_cast<bool>(g_cache & 1 << static_cast<int>(stream))
/* @brief Causes an early return if the standard terminal stream being targeted is not a TTY. */
#define CHECK_STREAM_TTY_STATUS()                                                                                      \
    PrepareTTYAndCache();                                                                                              \
    if ((stream.rdbuf() == std::cout.rdbuf() && !IS_TTY(TDK::Stream::Output)) ||                                       \
        (stream.rdbuf() == std::cerr.rdbuf() && !IS_TTY(TDK::Stream::Error)))                                          \
    {                                                                                                                  \
        return stream;                                                                                                 \
    }
/* @brief A bit that states the cache has already been filled in. */
#define HAS_CACHED_TTY_BIT (1 << 7)
/* @brief Sets the event key member based on a condition. */
#define PARSE_KEY(condition, key)                                                                                      \
    if (condition)                                                                                                     \
    {                                                                                                                  \
        event.m_key = key;                                                                                             \
        break;                                                                                                         \
    }
#ifdef _WIN32
/*
 * @brief Creates the TTY status cache of a standard terminal stream.
 * @param a_stream The stream being checked.
 */
#define TTY_CACHE(stream)                                                                                              \
    (!!(_isatty(_fileno(!static_cast<int>(stream)       ? stdin                                                        \
                        : static_cast<int>(stream) == 1 ? stdout                                                       \
                                                        : stderr)))                                                    \
     << static_cast<int>(stream))
#else
/*
 * @brief Creates the TTY status cache of a standard terminal stream.
 * @param a_stream The stream being checked.
 */
#define TTY_CACHE(stream)                                                                                              \
    (isatty(fileno(!static_cast<int>(stream)       ? stdin                                                             \
                   : static_cast<int>(stream) == 1 ? stdout                                                            \
                                                   : stderr))                                                          \
     << static_cast<int>(stream))
#endif

/*
 * @brief Creates a cache containing info about the TTY statuses of the standard terminal streams and, on
 * Windows, set the ENABLE_VIRTUAL_TERMINAL_PROCESSING bit in order to the terminal to start parsing ANSI escape
 * sequences.
 */
static void PrepareTTYAndCache();
/*
 * @brief Writes an ANSI escape sequence to either the standard output or error terminal streams based on the
 * fact if they are or not TTY and their character orientation.
 * @param format The format to be used to write the sequence. It uses the same format specifiers as the printf function
 * family.
 * @param ... The arguments to be formatted.
 */
static int WriteANSI(const char* format, ...);

static void PrepareTTYAndCache()
{
    if (!(g_cache & HAS_CACHED_TTY_BIT))
    {
        g_cache = TTY_CACHE(TDK::Stream::Input) | TTY_CACHE(TDK::Stream::Output) | TTY_CACHE(TDK::Stream::Error) |
                  HAS_CACHED_TTY_BIT;
    }
#ifdef _WIN32
    HANDLE handle;
    DWORD mode;
    (GetConsoleMode((handle = GetStdHandle(STD_OUTPUT_HANDLE)), &mode) ||
     GetConsoleMode((handle = GetStdHandle(STD_ERROR_HANDLE)), &mode)) &&
        SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

static int WriteANSI(const char* format, ...)
{
    PrepareTTYAndCache();
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

TDK::KeyEvent::KeyEvent() : m_key(0), m_hasAlt(false), m_hasCtrl(false)
{
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
    CHECK_STREAM_TTY_STATUS();
    return stream << "\x1b[" << static_cast<int>(effect) << "m";
}

std::ostream& TDK::operator>>(std::ostream& stream, Effect effect)
{
    CHECK_STREAM_TTY_STATUS();
    return stream << "\x1b[" << static_cast<int>(effect) + 20 << "m";
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

int TDK::operator+(int code, Key key)
{
    return code + static_cast<int>(key);
}

bool TDK::operator==(int code, Key key)
{
    return code == static_cast<int>(key);
}

bool TDK::operator!=(int code, Key key)
{
    return code != static_cast<int>(key);
}

void TDK::ClearCursorLine()
{
    WriteANSI("\x1b[2K\x1b[1G");
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
    if (WriteANSI("\x1b[6n") || tcgetattr(STDIN_FILENO, &attributes))
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
    --coordinate._row;
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
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) && ioctl(STDIN_FILENO, TIOCGWINSZ, &windowSize) &&
        ioctl(STDERR_FILENO, TIOCGWINSZ, &size))
    {
        return -1;
    }
    dimensions.m_totalColumns = size.ws_col;
    dimensions.m_totalColumns = size.ws_row;
#endif
    return 0;
}

bool TDK::IsTTY(Stream stream)
{
    PrepareTTYAndCache();
    return IS_TTY(stream);
}

TDK::KeyEventStatus TDK::ReadKeyEvent(KeyEvent& event)
{
    PrepareTTYAndCache();
    if (!IS_TTY(TDK::Stream::Input) || std::fwide(stdin, 0) > 0 ||
        (!IS_TTY(TDK::Stream::Output) && !IS_TTY(TDK::Stream::Error)))
    {
        return KeyEventStatus::Failure;
    }
#ifdef _WIN32
    HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD record;
    DWORD mode;
    DWORD totalEventsRead;
    int buffer;
    GetConsoleMode(handle, &mode);
    SetConsoleMode(handle, mode & ~ENABLE_PROCESSED_INPUT);
    while (true)
    {
        ReadConsoleInputW(handle, &record, 1, &totalEventsRead);
        if (record.EventType == WINDOW_BUFFER_SIZE_EVENT)
        {
            SetConsoleMode(handle, mode);
            return KeyEventStatus::WindowResizeInterrupt;
        }
        else if (record.EventType != KEY_EVENT || !record.Event.KeyEvent.bKeyDown ||
                 record.Event.KeyEvent.wVirtualKeyCode == VK_CONTROL ||
                 record.Event.KeyEvent.wVirtualKeyCode == VK_SHIFT ||
                 record.Event.KeyEvent.wVirtualKeyCode == VK_MENU ||
                 record.Event.KeyEvent.wVirtualKeyCode == VK_CAPITAL ||
                 record.Event.KeyEvent.wVirtualKeyCode == VK_NUMLOCK ||
                 record.Event.KeyEvent.wVirtualKeyCode == VK_SCROLL)
        {
            continue;
        }
        if ((buffer = record.Event.KeyEvent.uChar.UnicodeChar))
        {
            if (buffer <= 26 && buffer != TDK::Key::Tab && buffer != TDK::Key::Enter)
            {
                event.m_key = buffer + 96;
            }
            else if (buffer >= HIGH_SURROGATE_START && buffer <= HIGH_SURROGATE_END)
            {
                ReadConsoleInputW(handle, &record, 1, &totalEventsRead);
                ReadConsoleInputW(handle, &record, 1, &totalEventsRead);
                *((short*)&buffer + 1) = record.Event.KeyEvent.uChar.UnicodeChar;
                WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)&buffer, 2, (char*)&event.m_key, 4, NULL, NULL);
            }
            else
            {
                event.m_key = 0;
                WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)&buffer, 1, (char*)&event.m_key, 4, NULL, NULL);
            }
            event.m_hasAlt = record.Event.KeyEvent.dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED);
            event.m_hasCtrl = record.Event.KeyEvent.dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED);
            break;
        }
        else if (record.Event.KeyEvent.dwControlKeyState &
                 (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED | LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED | SHIFT_PRESSED))
        {
            continue;
        }
        PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_LEFT && record.Event.KeyEvent.wVirtualKeyCode <= VK_DOWN,
                  record.Event.KeyEvent.wVirtualKeyCode - VK_LEFT + TDK::Key::LeftArrow);
        PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_PRIOR && record.Event.KeyEvent.wVirtualKeyCode <= VK_HOME,
                  record.Event.KeyEvent.wVirtualKeyCode - VK_PRIOR + TDK::Key::PageUp);
        PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_INSERT &&
                      record.Event.KeyEvent.wVirtualKeyCode <= VK_DELETE,
                  record.Event.KeyEvent.wVirtualKeyCode - VK_INSERT + TDK::Key::Insert);
        PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_F1 && record.Event.KeyEvent.wVirtualKeyCode <= VK_F12,
                  record.Event.KeyEvent.wVirtualKeyCode - VK_F1 + TDK::Key::F1);
    }
    SetConsoleMode(handle, mode);
#endif
    return KeyEventStatus::Success;
}

void TDK::RingBell()
{
    WriteANSI("\7");
}

void TDK::SetAlternateWindow(bool isToOpen)
{
    WriteANSI(isToOpen ? "\x1b[?1049h\x1b[2J\x1b[1;1H" : "\x1b[?1049l");
}

void TDK::SetCursorCoordinate(unsigned short column, unsigned short row)
{
    WriteANSI("\x1b[%hu;%huH", row + 1, column + 1);
}

void TDK::SetCursorCoordinate(Coordinate& coordinate)
{
    SetCursorCoordinate(coordinate.m_column, coordinate.m_row);
}

void TDK::SetCursorShape(CursorShape shape)
{
    WriteANSI("\x1b[%d q", static_cast<int>(shape));
}

void TDK::SetCursorVisibility(bool isToShow)
{
    WriteANSI("\x1b[?25%c", isToShow ? 'h' : 'l');
}
