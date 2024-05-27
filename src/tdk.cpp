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

#include "TDK.hpp"

#define IS_TTY(stream) static_cast<bool>(g_cache & 1 << static_cast<int>(stream))
#define CHECK_STREAM_TTY_STATUS()                                                                                      \
    PrepareStreamsAndCache();                                                                                          \
    if ((stream.rdbuf() == std::cout.rdbuf() && !IS_TTY(TDK::Stream::Output)) ||                                       \
        (stream.rdbuf() == std::cerr.rdbuf() && !IS_TTY(TDK::Stream::Error)))                                          \
    {                                                                                                                  \
        return stream;                                                                                                 \
    }
#define HAS_CACHED_TTY_FLAG (1 << 7)
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

template <class T>
static T InvertColor(const T* color);
static void PrepareStreamsAndCache();
static int WriteANSISequence(const char* format, ...);

static char g_cache = 0;

template <class T>
static T InvertColor(const T* color)
{
    T copy = *color;
    copy.m_setLayer(copy.m_getLayer() == TDK::Layer::Foreground ? TDK::Layer::Background : TDK::Layer::Foreground);
    return copy;
}

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

template <class T>
TDK::Color<T>::Color()
{
}

template <class T>
TDK::Layer TDK::Color<T>::m_getLayer() const
{
    return m_layer;
}

template <class T>
void TDK::Color<T>::m_setLayer(TDK::Layer layer)
{
    m_layer = s_filterLayer(layer);
}

template <class T>
TDK::Layer TDK::Color<T>::s_filterLayer(Layer layer)
{
    return layer == TDK::Layer::Foreground || layer == TDK::Layer::Background ? layer : TDK::Layer::Foreground;
}

TDK::Coordinate::Coordinate() : m_column(0), m_row(0)
{
}

TDK::Coordinate::Coordinate(unsigned short column, unsigned short row) : m_column(column), m_row(row)
{
}

unsigned short TDK::Coordinate::m_getColumn() const
{
    return m_column;
}

unsigned short TDK::Coordinate::m_getRow() const
{
    return m_row;
}

void TDK::Coordinate::m_setColumn(unsigned short column)
{
    m_column = column;
}

void TDK::Coordinate::m_setRow(unsigned short row)
{
    m_row = row;
}

TDK::Effects::Effects(int code, bool isToEnable) : m_code(s_filterCode(code)), m_isToEnable(isToEnable)
{
}

TDK::Effects::Effects(TDK::EffectCode code, bool isToEnable)
    : m_code(s_filterCode(1 << static_cast<int>(code))), m_isToEnable(isToEnable)
{
}

int TDK::Effects::m_getCode() const
{
    return m_code;
}

bool TDK::Effects::m_getIsToEnable() const
{
    return m_isToEnable;
}

int TDK::Effects::s_filterCode(int code)
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

TDK::HexColor::HexColor(unsigned int code, Layer layer) : m_code(s_filterCode(code))
{
    m_layer = s_filterLayer(layer);
}

TDK::HexColor::HexColor(RGBColor color) : m_code(color.m_getRed() << 16 | color.m_getGreen() << 8 | color.m_getBlue())
{
    m_layer = color.m_getLayer();
}

unsigned int TDK::HexColor::s_filterCode(unsigned int code)
{
    return (std::min)(static_cast<int>(code), 0xffffff);
}

TDK::HexColor TDK::HexColor::m_invert() const
{
    return InvertColor(this);
}

unsigned int TDK::HexColor::m_getCode() const
{
    return m_code;
}

void TDK::HexColor::m_setCode(unsigned int code)
{
    m_code = s_filterCode(code);
}

TDK::Region::Region()
    : m_totalColumns(0), m_totalRows(0), m_area(0), m_topLeftCoordinate(), m_topRightCoordinate(),
      m_bottomLeftCoordinate(), m_bottomRightCoordinate()
{
}

TDK::Region::Region(unsigned short totalColumns, unsigned short totalRows)
    : m_totalColumns(totalColumns), m_totalRows(totalRows), m_area(totalColumns * totalRows), m_topLeftCoordinate(0, 0),
      m_topRightCoordinate(totalColumns - 1, 0), m_bottomLeftCoordinate(0, totalRows - 1),
      m_bottomRightCoordinate(totalColumns - 1, totalRows - 1)
{
}

TDK::Region::Region(Coordinate cornerCoordinate0, Coordinate cornerCoordinate1)
{
    unsigned short maxColumn = (std::max)(cornerCoordinate0.m_getColumn(), cornerCoordinate1.m_getColumn());
    unsigned short minColumn = (std::min)(cornerCoordinate0.m_getColumn(), cornerCoordinate1.m_getColumn());
    unsigned short maxRow = (std::max)(cornerCoordinate0.m_getRow(), cornerCoordinate1.m_getRow());
    unsigned short minRow = (std::min)(cornerCoordinate0.m_getRow(), cornerCoordinate1.m_getRow());
    m_totalColumns = maxColumn - minColumn;
    m_totalRows = maxRow - minRow;
    m_area = m_totalColumns * m_totalRows;
    m_topLeftCoordinate = Coordinate(minColumn, minRow);
    m_topRightCoordinate = Coordinate(maxColumn, minRow);
    m_bottomLeftCoordinate = Coordinate(minColumn, maxRow);
    m_bottomRightCoordinate = Coordinate(maxColumn, maxRow);
}

bool TDK::Region::m_contains(unsigned short column, unsigned short row) const
{
    return column >= m_topLeftCoordinate.m_getColumn() && column <= m_topRightCoordinate.m_getColumn() &&
           row >= m_topLeftCoordinate.m_getRow() && row <= m_bottomLeftCoordinate.m_getRow();
}

bool TDK::Region::m_contains(Coordinate coordinate) const
{
    return m_contains(coordinate.m_getColumn(), coordinate.m_getRow());
}

unsigned short TDK::Region::m_getTotalColumns() const
{
    return m_totalColumns;
}

unsigned short TDK::Region::m_getTotalRows() const
{
    return m_totalRows;
}

unsigned int TDK::Region::m_getArea() const
{
    return m_area;
}

TDK::Coordinate TDK::Region::m_getTopLeftCoordinate() const
{
    return m_topLeftCoordinate;
}

TDK::Coordinate TDK::Region::m_getTopRightCoordinate() const
{
    return m_topRightCoordinate;
}

TDK::Coordinate TDK::Region::m_getBottomLeftCoordinate() const
{
    return m_bottomLeftCoordinate;
}

TDK::Coordinate TDK::Region::m_getBottomRightCoordinate() const
{
    return m_bottomRightCoordinate;
}

TDK::RGBColor::RGBColor(unsigned char red, unsigned char green, unsigned char blue, Layer layer)
    : m_red(red), m_green(green), m_blue(blue)
{
    m_layer = s_filterLayer(layer);
}

TDK::RGBColor::RGBColor(HexColor color)
    : m_red(color.m_getCode() >> 16 & 0xff), m_green(color.m_getCode() >> 8 & 0xff), m_blue(color.m_getCode() & 0xff)

{
    m_layer = color.m_getLayer();
}

TDK::RGBColor TDK::RGBColor::m_invert() const
{
    return InvertColor(this);
}

unsigned char TDK::RGBColor::m_getRed() const
{
    return m_red;
}

unsigned char TDK::RGBColor::m_getGreen() const
{
    return m_green;
}

unsigned char TDK::RGBColor::m_getBlue() const
{
    return m_blue;
}

void TDK::RGBColor::m_setRed(unsigned char red)
{
    m_red = red;
}

void TDK::RGBColor::m_setGreen(unsigned char green)
{
    m_green = green;
}

void TDK::RGBColor::m_setBlue(unsigned char blue)
{
    m_blue = blue;
}

TDK::XColor::XColor(unsigned char code, Layer layer) : m_code(code)
{
    m_layer = s_filterLayer(layer);
}

TDK::XColor::XColor(XColorCode code, Layer layer) : m_code(s_filterCode(code))
{
    m_layer = s_filterLayer(layer);
}

TDK::XColor TDK::XColor::m_invert() const
{
    return InvertColor(this);
}

short TDK::XColor::s_filterCode(XColorCode code)
{
    return (std::max)((std::min)(static_cast<int>(code), 255), static_cast<int>(TDK::XColorCode::Default));
}

short TDK::XColor::m_getCode() const
{
    return m_code;
}

void TDK::XColor::m_setCode(unsigned char code)
{
    m_code = code;
}

void TDK::XColor::m_setCode(XColorCode code)
{
    m_code = s_filterCode(code);
}

std::ostream& TDK::operator<<(std::ostream& stream, Effects effects)
{
    for (int code = 0; code < 10; ++code)
    {
        if (effects.m_getCode() & 1 << code)
        {
            std::cout << "\x1b[" << code + (!effects.m_getIsToEnable() * 20) << "m";
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
    return stream << "\x1b[" << static_cast<int>(color.m_getLayer()) << "8;2;" << static_cast<int>(color.m_getRed())
                  << ";" << static_cast<int>(color.m_getGreen()) << ";" << static_cast<int>(color.m_getBlue()) << "m";
}

std::ostream& TDK::operator<<(std::ostream& stream, XColor color)
{
    CHECK_STREAM_TTY_STATUS();
    return color.m_getCode() == static_cast<int>(TDK::XColorCode::Default)
               ? stream << "\x1b[" << static_cast<int>(color.m_getLayer()) << "9m"
               : stream << "\x1b[" << static_cast<int>(color.m_getLayer()) << "8;5;" << color.m_getCode() << "m";
}

std::ostream& TDK::operator<<(std::ostream& stream, Weight weight)
{
    CHECK_STREAM_TTY_STATUS();
    return weight == Weight::Default ? stream << "\x1b[22m" : stream << "\x1b[22;" << static_cast<int>(weight) << "m";
}

bool TDK::operator==(XColor color0, XColor color1)
{
    return color0.m_getLayer() == color1.m_getLayer() && color0.m_getCode() == color1.m_getCode();
}

bool TDK::operator==(HexColor color0, HexColor color1)
{
    return color0.m_getLayer() == color1.m_getLayer() && color0.m_getCode() == color1.m_getCode();
}

bool TDK::operator==(RGBColor color0, RGBColor color1)
{
    return color0.m_getLayer() == color1.m_getLayer() && color0.m_getRed() == color1.m_getRed() &&
           color0.m_getGreen() == color1.m_getGreen() && color0.m_getBlue() == color1.m_getBlue();
}

bool TDK::operator==(Coordinate coordinate0, Coordinate coordinate1)
{
    return coordinate0.m_getColumn() == coordinate1.m_getColumn() && coordinate0.m_getRow() == coordinate1.m_getRow();
}

bool TDK::operator==(Region region0, Region region1)
{
    return region0.m_getTopLeftCoordinate() == region1.m_getTopLeftCoordinate() &&
           region0.m_getTopRightCoordinate() == region1.m_getTopRightCoordinate() &&
           region0.m_getBottomLeftCoordinate() == region1.m_getBottomLeftCoordinate() &&
           region0.m_getBottomRightCoordinate() == region1.m_getBottomRightCoordinate();
}

int TDK::operator|(EffectCode code0, EffectCode code1)
{
    return 1 << static_cast<int>(code0) | 1 << static_cast<int>(code1);
}

void TDK::clearCursorLine()
{
    WriteANSISequence("\x1b[2K\x1b[1G");
}

void TDK::clearInputBuffer()
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

void TDK::clearWindow()
{
    WriteANSISequence("\x1b[2J\x1b[1;1H");
}

void TDK::closeAlternateWindow()
{
    WriteANSISequence("\x1b[?1049l");
}

int TDK::getCursorCoordinate(Coordinate& coordinate)
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
    clearInputBuffer();
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

int TDK::getWindowRegion(Region& region)
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo) &&
        !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE), &bufferInfo))
    {
        return -1;
    }
    region = Region(bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1,
                    bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1);
#else
    struct winsize size;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) && ioctl(STDIN_FILENO, TIOCGWINSZ, &size) &&
        ioctl(STDERR_FILENO, TIOCGWINSZ, &size))
    {
        return -1;
    }
    region = Region(size.ws_col, size.ws_row);
#endif
    return 0;
}

bool TDK::isTTY(Stream stream)
{
    PrepareStreamsAndCache();
    return static_cast<int>(stream) >= static_cast<int>(TDK::Stream::Input) &&
                   static_cast<int>(stream) <= static_cast<int>(TDK::Stream::Error)
               ? IS_TTY(stream)
               : false;
}

void TDK::openAlternateWindow()
{
    WriteANSISequence("\x1b[?1049h\x1b[2J\x1b[1;1H");
}

void TDK::ringBell()
{
    WriteANSISequence("\7");
}

void TDK::setCursorCoordinate(unsigned short column, unsigned short row)
{
    WriteANSISequence("\x1b[%hu;%huH", row + 1, column + 1);
}

void TDK::setCursorCoordinate(Coordinate coordinate)
{
    setCursorCoordinate(coordinate.m_getColumn(), coordinate.m_getRow());
}

void TDK::setCursorShape(CursorShape shape)
{
    WriteANSISequence(
        "\x1b[%d q",
        static_cast<int>(static_cast<int>(shape) >= static_cast<int>(TDK::CursorShape::Default) &&
                                 static_cast<int>(shape) <= static_cast<int>(TDK::CursorShape::NonBlinkingBar)
                             ? shape
                             : TDK::CursorShape::Default));
}

void TDK::setWindowTitle(std::string title)
{
    WriteANSISequence("\x1b]0;%s\7", title.c_str());
}

void TDK::setCursorVisibility(bool isToShow)
{
    WriteANSISequence("\x1b[?25%c", isToShow ? 'h' : 'l');
}
