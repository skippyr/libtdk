#include <cstdarg>
#include <iomanip>
#include <sstream>

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#else
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif

#include "TMK.hpp"

#define IS_TTY(stream) static_cast<bool>(g_cache & 1 << static_cast<int>(stream))
#define CHECK_STREAM_TTY_STATUS()                                                                                      \
    PrepareCacheAndStreams();                                                                                          \
    if ((stream.rdbuf() == std::cout.rdbuf() && !IS_TTY(TMK::Stream::Output)) ||                                       \
        (stream.rdbuf() == std::cerr.rdbuf() && !IS_TTY(TMK::Stream::Error)))                                          \
    {                                                                                                                  \
        return stream;                                                                                                 \
    }
#define HAS_CACHED_TTY_FLAG (1 << 7)
#ifdef _WIN32
#define TTY_CACHE(stream)                                                                                              \
    (!!(_isatty(_fileno(stream == TMK::Stream::Input    ? stdin                                                        \
                        : stream == TMK::Stream::Output ? stdout                                                       \
                                                        : stderr)))                                                    \
     << static_cast<int>(stream))
#else
#define TTY_CACHE(stream)                                                                                              \
    (isatty(fileno(stream == TMK::Stream::Input    ? stdin                                                             \
                   : stream == TMK::Stream::Output ? stdout                                                            \
                                                   : stderr))                                                          \
     << static_cast<int>(stream))
#endif

template <class T>
static T InvertColor(const T* color);
static void PrepareCacheAndStreams();
static TMK::EventInfo ReadGenericEvent(bool allowMouseCapture, int waitInMilliseconds,
                                       std::function<bool(TMK::EventInfo&)> filter);
static int WriteANSISequence(const char* format, ...);

static char g_cache = 0;

template <class T>
static T InvertColor(const T* color)
{
    T copy = *color;
    copy.SetLayer(copy.GetLayer() == TMK::Layer::Foreground ? TMK::Layer::Background : TMK::Layer::Foreground);
    return copy;
}

static void PrepareCacheAndStreams()
{
    if (!(g_cache & HAS_CACHED_TTY_FLAG))
    {
        g_cache = TTY_CACHE(TMK::Stream::Input) | TTY_CACHE(TMK::Stream::Output) | TTY_CACHE(TMK::Stream::Error) |
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

static TMK::EventInfo ReadGenericEvent(bool allowMouseCapture, int waitInMilliseconds,
                                       std::function<bool(TMK::EventInfo&)> filter)
{
    return TMK::EventType::None;
}

static int WriteANSISequence(const char* format, ...)
{
    PrepareCacheAndStreams();
    if (!IS_TTY(TMK::Stream::Output) && !IS_TTY(TMK::Stream::Error))
    {
        return -1;
    }
    std::va_list arguments;
    va_start(arguments, format);
    int totalBytesWritten = std::vfprintf(IS_TTY(TMK::Stream::Output) ? stdout : stderr, format, arguments);
    va_end(arguments);
    return -(totalBytesWritten < 0);
}

template <class T>
TMK::Layer TMK::Color<T>::GetLayer() const
{
    return m_layer;
}

template <class T>
void TMK::Color<T>::SetLayer(TMK::Layer layer)
{
    m_layer = FilterLayer(layer);
}

TMK::HexColor::HexColor(unsigned int code, Layer layer) : m_code(FilterCode(code))
{
    m_layer = FilterLayer(layer);
}

TMK::HexColor::HexColor(RGBColor color) : m_code(color.GetRed() << 16 | color.GetGreen() << 8 | color.GetBlue())
{
    m_layer = color.GetLayer();
}

unsigned int TMK::HexColor::FilterCode(unsigned int code)
{
    return (std::min)(static_cast<int>(code), 0xffffff);
}

TMK::HexColor TMK::HexColor::Invert() const
{
    return InvertColor(this);
}

std::string TMK::HexColor::ToString(bool isUpper, bool hasPrefix) const
{
    std::ostringstream stringStream;
    stringStream << std::hex << std::setw(6) << std::setfill('0') << m_code;
    std::string string = stringStream.str();
    if (isUpper)
    {
        for (int offset = 0; offset < string.length(); ++offset)
        {
            if (string[offset] >= 97 && string[offset] <= 122)
            {
                string[offset] = string[offset] - 32;
            }
        }
    }
    if (hasPrefix)
    {
        string = "0x" + string;
    }
    return string;
}

unsigned int TMK::HexColor::GetCode() const
{
    return m_code;
}

void TMK::HexColor::SetCode(unsigned int code)
{
    m_code = FilterCode(code);
}

TMK::RGBColor::RGBColor(unsigned char red, unsigned char green, unsigned char blue, Layer layer)
    : m_red(red), m_green(green), m_blue(blue)
{
    m_layer = FilterLayer(layer);
}

TMK::RGBColor::RGBColor(HexColor color)
    : m_red(color.GetCode() >> 16 & 0xff), m_green(color.GetCode() >> 8 & 0xff), m_blue(color.GetCode() & 0xff)
{
    m_layer = color.GetLayer();
}

TMK::RGBColor TMK::RGBColor::Invert() const
{
    return InvertColor(this);
}

unsigned char TMK::RGBColor::GetRed() const
{
    return m_red;
}

unsigned char TMK::RGBColor::GetGreen() const
{
    return m_green;
}

unsigned char TMK::RGBColor::GetBlue() const
{
    return m_blue;
}

void TMK::RGBColor::SetRed(unsigned char red)
{
    m_red = red;
}

void TMK::RGBColor::SetGreen(unsigned char green)
{
    m_green = green;
}

void TMK::RGBColor::SetBlue(unsigned char blue)
{
    m_blue = blue;
}

TMK::XColor::XColor(unsigned char code, Layer layer) : m_code(code)
{
    m_layer = FilterLayer(layer);
}

TMK::XColor::XColor(XColorCode code, Layer layer) : m_code(FilterCode(code))
{
    m_layer = FilterLayer(layer);
}

TMK::XColor TMK::XColor::Invert() const
{
    return InvertColor(this);
}

short TMK::XColor::FilterCode(XColorCode code)
{
    return (std::max)((std::min)(static_cast<int>(code), 255), static_cast<int>(TMK::XColorCode::Default));
}

short TMK::XColor::GetCode() const
{
    return m_code;
}

void TMK::XColor::SetCode(unsigned char code)
{
    m_code = code;
}

void TMK::XColor::SetCode(XColorCode code)
{
    m_code = FilterCode(code);
}

template <class T>
TMK::Layer TMK::Color<T>::FilterLayer(Layer layer)
{
    return layer == TMK::Layer::Foreground || layer == TMK::Layer::Background ? layer : TMK::Layer::Foreground;
}

TMK::FocusEvent::FocusEvent(bool hasFocus) : m_hasFocus(hasFocus)
{
}

bool TMK::FocusEvent::HasFocus() const
{
    return m_hasFocus;
}

TMK::EventInfo::EventInfo(EventType type) : m_type(type)
{
}

TMK::EventInfo::EventInfo(FocusEvent focusEvent) : m_type(EventType::Focus), m_focusEvent(focusEvent)
{
}

TMK::Coordinate::Coordinate() : m_column(0), m_row(0)
{
}

TMK::Coordinate::Coordinate(unsigned short column, unsigned short row) : m_column(column), m_row(row)
{
}

unsigned short TMK::Coordinate::GetColumn() const
{
    return m_column;
}

unsigned short TMK::Coordinate::GetRow() const
{
    return m_row;
}

void TMK::Coordinate::SetColumn(unsigned short column)
{
    m_column = column;
}

void TMK::Coordinate::SetRow(unsigned short row)
{
    m_row = row;
}

TMK::Dimensions::Dimensions() : m_totalColumns(0), m_totalRows(0)
{
}

TMK::Dimensions::Dimensions(unsigned short totalColumns, unsigned short totalRows)
    : m_totalColumns(totalColumns), m_totalRows(totalRows)
{
}

unsigned short TMK::Dimensions::GetTotalColumns() const
{
    return m_totalColumns;
}

unsigned short TMK::Dimensions::GetTotalRows() const
{
    return m_totalRows;
}

TMK::Effects::Effects(int code, bool isToEnable) : m_code(FilterCode(code)), m_isToEnable(isToEnable)
{
}

TMK::Effects::Effects(EffectCode code, bool isToEnable)
    : m_code(FilterCode(1 << static_cast<int>(code))), m_isToEnable(isToEnable)
{
}

int TMK::Effects::FilterCode(int code)
{
    int filteredCodes = 0;
    for (int ansiCode = 0; ansiCode < 10; ++ansiCode)
    {
        if (ansiCode != 6 && code & 1 << ansiCode)
        {
            filteredCodes |= 1 << ansiCode;
        }
    }
    return filteredCodes;
}

int TMK::Effects::GetCode() const
{
    return m_code;
}

bool TMK::Effects::IsToEnable() const
{
    return m_isToEnable;
}

std::ostream& TMK::operator<<(std::ostream& stream, Effects effects)
{
    for (int ansiCode = 0; ansiCode < 10; ++ansiCode)
    {
        if (effects.GetCode() & 1 << ansiCode)
        {
            std::cout << "\x1b[" << ansiCode + (!effects.IsToEnable() * 20) << "m";
        }
    }
    return stream;
}

std::ostream& TMK::operator<<(std::ostream& stream, HexColor color)
{
    return stream << RGBColor(color);
}

std::ostream& TMK::operator<<(std::ostream& stream, RGBColor color)
{
    CHECK_STREAM_TTY_STATUS();
    return stream << "\x1b[" << static_cast<int>(color.GetLayer()) << "8;2;" << static_cast<int>(color.GetRed()) << ";"
                  << static_cast<int>(color.GetGreen()) << ";" << static_cast<int>(color.GetBlue()) << "m";
}

std::ostream& TMK::operator<<(std::ostream& stream, XColor color)
{
    CHECK_STREAM_TTY_STATUS();
    return color.GetCode() == static_cast<int>(TMK::XColorCode::Default)
               ? stream << "\x1b[" << static_cast<int>(color.GetLayer()) << "9m"
               : stream << "\x1b[" << static_cast<int>(color.GetLayer()) << "8;5;" << color.GetCode() << "m";
}

std::ostream& TMK::operator<<(std::ostream& stream, Weight weight)
{
    CHECK_STREAM_TTY_STATUS();
    return weight == Weight::Default ? stream << "\x1b[22m" : stream << "\x1b[22;" << static_cast<int>(weight) << "m";
}

bool TMK::operator==(XColor color0, XColor color1)
{
    return color0.GetLayer() == color1.GetLayer() && color0.GetCode() == color1.GetCode();
}

bool TMK::operator==(HexColor color0, HexColor color1)
{
    return color0.GetLayer() == color1.GetLayer() && color0.GetCode() == color1.GetCode();
}

bool TMK::operator==(RGBColor color0, RGBColor color1)
{
    return color0.GetLayer() == color1.GetLayer() && color0.GetRed() == color1.GetRed() &&
           color0.GetGreen() == color1.GetGreen() && color0.GetBlue() == color1.GetBlue();
}

bool TMK::operator==(HexColor hexColor, RGBColor rgbColor)
{
    return hexColor == HexColor(rgbColor);
}

bool TMK::operator==(RGBColor rgbColor, HexColor hexColor)
{
    return HexColor(rgbColor) == hexColor;
}

bool TMK::operator==(Coordinate coordinate0, Coordinate coordinate1)
{
    return coordinate0.GetColumn() == coordinate1.GetColumn() && coordinate0.GetRow() == coordinate1.GetRow();
}

int TMK::operator|(EffectCode code0, EffectCode code1)
{
    return 1 << static_cast<int>(code0) | 1 << static_cast<int>(code1);
}

void TMK::ClearCursorLine()
{
    WriteANSISequence("\x1b[2K\x1b[1G");
}

void TMK::ClearInputBuffer()
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

void TMK::ClearWindow()
{
    WriteANSISequence("\x1b[2J\x1b[1;1H");
}

void TMK::CloseAlternateWindow()
{
    WriteANSISequence("\x1b[?1049l");
}

int TMK::GetCursorCoordinate(Coordinate& coordinate)
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo) &&
        !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE), &bufferInfo))
    {
        return -1;
    }
    coordinate = Coordinate(bufferInfo.dwCursorPosition.X - bufferInfo.srWindow.Left,
                            bufferInfo.dwCursorPosition.Y - bufferInfo.srWindow.Top);
#else
    struct termios attributes;
    ClearInputBuffer();
    if (WriteANSISequence("\x1b[6n") || tcgetattr(STDIN_FILENO, &attributes))
    {
        return -1;
    }
    attributes.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
    unsigned short column;
    unsigned short row;
    int totalMatchesRead = std::scanf("\x1b[%hu;%huR", &row, &column);
    attributes.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
    if (totalMatchesRead != 2)
    {
        return -1;
    }
    coordinate = Coordinate(--column, --row);
#endif
    return 0;
}

int TMK::GetWindowDimensions(Dimensions& dimensions)
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

bool TMK::IsTTY(Stream stream)
{
    PrepareCacheAndStreams();
    return static_cast<int>(stream) >= static_cast<int>(TMK::Stream::Input) &&
                   static_cast<int>(stream) <= static_cast<int>(TMK::Stream::Error)
               ? IS_TTY(stream)
               : false;
}

void TMK::OpenAlternateWindow()
{
    WriteANSISequence("\x1b[?1049h\x1b[2J\x1b[1;1H");
}

void TMK::RingBell()
{
    WriteANSISequence("\7");
}

TMK::EventInfo TMK::ReadEvent(bool allowMouseCapture)
{
    return ReadGenericEvent(allowMouseCapture, -1, [](EventInfo&) { return true; });
}

TMK::EventInfo TMK::ReadEvent(bool allowMouseCapture, unsigned short waitInMilliseconds)
{
    return ReadGenericEvent(allowMouseCapture, waitInMilliseconds, [](EventInfo&) { return true; });
}

TMK::EventInfo TMK::ReadEvent(bool allowMouseCapture, unsigned short waitInMilliseconds,
                              std::function<bool(EventInfo&)> filter)
{
    return ReadGenericEvent(allowMouseCapture, waitInMilliseconds, filter);
}

void TMK::SetCursorCoordinate(unsigned short column, unsigned short row)
{
    WriteANSISequence("\x1b[%hu;%huH", row + 1, column + 1);
}

void TMK::SetCursorCoordinate(Coordinate coordinate)
{
    SetCursorCoordinate(coordinate.GetColumn(), coordinate.GetRow());
}

void TMK::SetCursorShape(CursorShape shape)
{
    WriteANSISequence("\x1b[%d q",
                      static_cast<int>(static_cast<int>(shape) >= static_cast<int>(TMK::CursorShape::Default) &&
                                               static_cast<int>(shape) <= static_cast<int>(TMK::CursorShape::Bar)
                                           ? shape
                                           : TMK::CursorShape::Default));
}

void TMK::SetWindowTitle(std::string title)
{
    WriteANSISequence("\x1b]0;%s\7", title.c_str());
}

void TMK::SetCursorVisibility(bool isVisible)
{
    WriteANSISequence("\x1b[?25%c", isVisible ? 'h' : 'l');
}
