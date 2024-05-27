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

static void PrepareStreamsAndCache();
static int WriteANSISequence(const char* format, ...);

static char g_cache = 0;

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

template <typename T>
TDK::Color<T>::Color()
{
}

template <typename T>
TDK::Layer TDK::Color<T>::FilterLayer(Layer layer)
{
    return layer == TDK::Layer::Foreground || layer == TDK::Layer::Background ? layer : TDK::Layer::Foreground;
}

TDK::Coordinate::Coordinate() : m_column(0), m_row(0)
{
}

TDK::Coordinate::Coordinate(unsigned short column, unsigned short row) : m_column(column), m_row(row)
{
}

unsigned short TDK::Coordinate::GetColumn() const
{
    return m_column;
}

unsigned short TDK::Coordinate::GetRow() const
{
    return m_row;
}

void TDK::Coordinate::SetColumn(unsigned short column)
{
    m_column = column;
}

void TDK::Coordinate::SetRow(unsigned short row)
{
    m_row = row;
}

TDK::Effect::Effect(int code, bool isToEnable) : m_code(FilterCode(code)), m_isToEnable(isToEnable)
{
}

TDK::Effect::Effect(TDK::EffectCode code, bool isToEnable)
    : m_code(FilterCode(1 << static_cast<int>(code))), m_isToEnable(isToEnable)
{
}

TDK::XColor TDK::XColor::Invert() const
{
    XColor color = *this;
    color.m_layer = color.m_layer == TDK::Layer::Foreground ? TDK::Layer::Background : TDK::Layer::Foreground;
    return color;
}

int TDK::Effect::GetCode() const
{
    return m_code;
}

bool TDK::Effect::IsToEnable() const
{
    return m_isToEnable;
}

int TDK::Effect::FilterCode(int code)
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

TDK::Region::Region(Coordinate& cornerCoordinate0, Coordinate& cornerCoordinate1)
{
    unsigned short maxColumn = (std::max)(cornerCoordinate0.GetColumn(), cornerCoordinate1.GetColumn());
    unsigned short minColumn = (std::min)(cornerCoordinate0.GetColumn(), cornerCoordinate1.GetColumn());
    unsigned short maxRow = (std::max)(cornerCoordinate0.GetRow(), cornerCoordinate1.GetRow());
    unsigned short minRow = (std::min)(cornerCoordinate0.GetRow(), cornerCoordinate1.GetRow());
    m_totalColumns = maxColumn - minColumn;
    m_totalRows = maxRow - minRow;
    m_area = m_totalColumns * m_totalRows;
    m_topLeftCoordinate = Coordinate(minColumn, minRow);
    m_topRightCoordinate = Coordinate(maxColumn, minRow);
    m_bottomLeftCoordinate = Coordinate(minColumn, maxRow);
    m_bottomRightCoordinate = Coordinate(maxColumn, maxRow);
}

unsigned short TDK::Region::GetTotalColumns() const
{
    return m_totalColumns;
}

unsigned short TDK::Region::GetTotalRows() const
{
    return m_totalRows;
}

unsigned int TDK::Region::GetArea() const
{
    return m_area;
}

TDK::Coordinate TDK::Region::GetTopLeftCoordinate() const
{
    return m_topLeftCoordinate;
}

TDK::Coordinate TDK::Region::GetTopRightCoordinate() const
{
    return m_topRightCoordinate;
}

TDK::Coordinate TDK::Region::GetBottomLeftCoordinate() const
{
    return m_bottomLeftCoordinate;
}

TDK::Coordinate TDK::Region::GetBottomRightCoordinate() const
{
    return m_bottomRightCoordinate;
}

TDK::RGBColor::RGBColor(unsigned char red, unsigned char green, unsigned char blue, Layer layer)
    : m_red(red), m_green(green), m_blue(blue), m_layer(FilterLayer(layer))
{
}

TDK::RGBColor::RGBColor(HexColor& color)
    : m_red(color.m_code >> 16 & 0xff), m_green(color.m_code >> 8 & 0xff), m_blue(color.m_code & 0xff),
      m_layer(color.m_layer)
{
}

TDK::RGBColor TDK::RGBColor::Invert() const
{
    RGBColor color = *this;
    color.m_layer = color.m_layer == TDK::Layer::Foreground ? TDK::Layer::Background : TDK::Layer::Foreground;
    return color;
}

unsigned char TDK::RGBColor::GetRed() const
{
    return m_red;
}

unsigned char TDK::RGBColor::GetGreen() const
{
    return m_green;
}

unsigned char TDK::RGBColor::GetBlue() const
{
    return m_blue;
}

TDK::Layer TDK::RGBColor::GetLayer() const
{
    return m_layer;
}

void TDK::RGBColor::SetRed(unsigned char red)
{
    m_red = red;
}

void TDK::RGBColor::SetGreen(unsigned char green)
{
    m_green = green;
}

void TDK::RGBColor::SetBlue(unsigned char blue)
{
    m_blue = blue;
}

void TDK::RGBColor::SetLayer(TDK::Layer layer)
{
    m_layer = FilterLayer(layer);
}

TDK::XColor::XColor(unsigned char code, Layer layer) : m_code(code), m_layer(FilterLayer(layer))
{
}

TDK::XColor::XColor(XColorCode code, Layer layer) : m_code(FilterCode(code)), m_layer(FilterLayer(layer))
{
}

short TDK::XColor::FilterCode(XColorCode code)
{
    return (std::max)((std::min)(static_cast<int>(code), 255), static_cast<int>(TDK::XColorCode::Default));
}

short TDK::XColor::GetCode() const
{
    return m_code;
}

TDK::Layer TDK::XColor::GetLayer() const
{
    return m_layer;
}

void TDK::XColor::SetCode(unsigned char code)
{
    m_code = code;
}

void TDK::XColor::SetCode(XColorCode code)
{
    m_code = FilterCode(code);
}

void TDK::XColor::SetLayer(Layer layer)
{
    m_layer = FilterLayer(layer);
}

std::ostream& TDK::operator<<(std::ostream& stream, Effect effect)
{
    for (int code = 0; code < 10; ++code)
    {
        if (effect.GetCode() & 1 << code)
        {
            std::cout << "\x1b[" << code + (!effect.IsToEnable() * 20) << "m";
        }
    }
    return stream;
}

std::ostream& TDK::operator<<(std::ostream& stream, HexColor& color)
{
    return stream << RGBColor(color);
}

std::ostream& TDK::operator<<(std::ostream& stream, RGBColor& color)
{
    CHECK_STREAM_TTY_STATUS();
    return stream << "\x1b[" << static_cast<int>(color.GetLayer()) << "8;2;" << static_cast<int>(color.GetRed()) << ";"
                  << static_cast<int>(color.GetGreen()) << ";" << static_cast<int>(color.GetBlue()) << "m";
}

std::ostream& TDK::operator<<(std::ostream& stream, XColor& color)
{
    CHECK_STREAM_TTY_STATUS();
    return color.GetCode() == static_cast<int>(TDK::XColorCode::Default)
               ? stream << "\x1b[" << static_cast<int>(color.GetLayer()) << "9m"
               : stream << "\x1b[" << static_cast<int>(color.GetLayer()) << "8;5;" << color.GetCode() << "m";
}

std::ostream& TDK::operator<<(std::ostream& stream, Weight weight)
{
    CHECK_STREAM_TTY_STATUS();
    return weight == Weight::Default ? stream << "\x1b[22m" : stream << "\x1b[22;" << static_cast<int>(weight) << "m";
}

bool TDK::operator==(XColor& color0, XColor& color1)
{
    return color0.GetLayer() == color1.GetLayer() && color0.GetCode() == color1.GetCode();
}

bool TDK::operator==(HexColor& color0, HexColor& color1)
{
    return color0.m_layer == color1.m_layer && color0.m_code == color1.m_code;
}

bool TDK::operator==(RGBColor& color0, RGBColor& color1)
{
    return color0.GetLayer() == color1.GetLayer() && color0.GetRed() == color1.GetRed() &&
           color0.GetGreen() == color1.GetGreen() && color0.GetBlue() == color1.GetBlue();
}

bool TDK::operator==(Coordinate& coordinate0, Coordinate& coordinate1)
{
    return coordinate0.GetColumn() == coordinate1.GetColumn() && coordinate0.GetRow() == coordinate1.GetRow();
}

bool TDK::operator==(Region& region0, Region& region1)
{
    return region0.GetTopLeftCoordinate() == region1.GetTopLeftCoordinate() &&
           region0.GetTopRightCoordinate() == region1.GetTopRightCoordinate() &&
           region0.GetBottomLeftCoordinate() == region1.GetBottomLeftCoordinate() &&
           region0.GetBottomRightCoordinate() == region1.GetBottomRightCoordinate();
}

int TDK::operator|(EffectCode code0, EffectCode code1)
{
    return 1 << static_cast<int>(code0) | 1 << static_cast<int>(code1);
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

int TDK::GetWindowRegion(Region& region)
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

bool TDK::IsTTY(Stream stream)
{
    PrepareStreamsAndCache();
    return IS_TTY(stream);
}

void TDK::OpenAlternateWindow()
{
    WriteANSISequence("\x1b[?1049h\x1b[2J\x1b[1;1H");
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
    SetCursorCoordinate(coordinate.GetColumn(), coordinate.GetRow());
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
