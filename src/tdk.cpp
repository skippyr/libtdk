#include <cstdarg>
#include <iomanip>

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#else
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif

#include "tdk.hpp"

/**
 * @brief Checks if a standard terminal stream is a TTY.
 * @param a_stream The stream to be checked. It must be a value from the tdk::Stream enum class.
 * @returns A boolean that states the stream is a TTY.
 */
#define IS_TTY(a_stream) static_cast<bool>(g_cache & 1 << static_cast<int>(a_stream))
/**
 * @brief Checks the TTY status of a standard terminal stream being targeted and cause an early
 * return if it is not a TTY.
 */
#define CHECK_STREAM_TTY_STATUS()                                                                  \
    prepareCacheAndStreams();                                                                      \
    if ((stream.rdbuf() == std::cout.rdbuf() && !IS_TTY(tdk::Stream::Output)) ||                   \
        (stream.rdbuf() == std::cerr.rdbuf() && !IS_TTY(tdk::Stream::Error)))                      \
    {                                                                                              \
        return stream;                                                                             \
    }
/** @brief A bitmask flag that states the cache has already been filled up. */
#define HAS_CACHED_TTY_FLAG (1 << 7)
#ifdef _WIN32
/**
 * @brief Creates the TTY status cache of a standard terminal stream.
 * @param a_stream The stream to be checked. It must be a value from the tdk::Stream enum class.
 * @returns The TTY status cache of the stream.
 */
#define TTY_CACHE(a_stream)                                                                        \
    (!!(_isatty(_fileno(a_stream == tdk::Stream::Input    ? stdin                                  \
                        : a_stream == tdk::Stream::Output ? stdout                                 \
                                                          : stderr)))                              \
     << static_cast<int>(a_stream))
#else
/**
 * @brief Creates the TTY status cache of a standard terminal stream.
 * @param a_stream The stream to be checked. It must be a value from the tdk::Stream enum class.
 * @returns The TTY status cache of the stream.
 */
#define TTY_CACHE(a_stream)                                                                        \
    (isatty(fileno(a_stream == tdk::Stream::Input    ? stdin                                       \
                   : a_stream == tdk::Stream::Output ? stdout                                      \
                                                     : stderr))                                    \
     << static_cast<int>(a_stream))
#endif

/**
 * @brief Inverts the layer where a color applies on.
 * @tparam The type of the color.
 * @param color The color to affected.
 * @returns The color with its layer inverted.
 */
template <class T>
static T invertColor(const T* color);
/**
 * @brief Caches information about the TTY statuses of the standard terminal streams and sets the
 * ENABLE_VIRTUAL_TERMINAL_PROCESSING mode flag on Windows.
 */
static void prepareCacheAndStreams();
/**
 * @brief Formats and writes an ANSI escape sequence to a valid TTY stream.
 * @param format The format to be used. It uses the same specifiers as the printf function family.
 * @param ... The arguments to be formatted.
 * @returns 0 if successful or -1 otherwise.
 */
static int writeANSISequence(const char* format, ...);

/** @brief A cache containing the TTY statuses of the standard terminal streams. */
static char g_cache = 0;

template <class T>
static T invertColor(const T* color)
{
    T copy = *color;
    copy.setLayer(copy.getLayer() == tdk::Layer::Foreground ? tdk::Layer::Background
                                                            : tdk::Layer::Foreground);
    return copy;
}

static void prepareCacheAndStreams()
{
    if (!(g_cache & HAS_CACHED_TTY_FLAG))
    {
        g_cache = TTY_CACHE(tdk::Stream::Input) | TTY_CACHE(tdk::Stream::Output) |
                  TTY_CACHE(tdk::Stream::Error) | HAS_CACHED_TTY_FLAG;
    }
#ifdef _WIN32
    HANDLE handle;
    DWORD mode;
    (GetConsoleMode((handle = GetStdHandle(STD_OUTPUT_HANDLE)), &mode) ||
     GetConsoleMode((handle = GetStdHandle(STD_ERROR_HANDLE)), &mode)) &&
        SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

static int writeANSISequence(const char* format, ...)
{
    prepareCacheAndStreams();
    if (!IS_TTY(tdk::Stream::Output) && !IS_TTY(tdk::Stream::Error))
    {
        return -1;
    }
    std::va_list arguments;
    va_start(arguments, format);
    int totalBytesWritten =
        std::vfprintf(IS_TTY(tdk::Stream::Output) ? stdout : stderr, format, arguments);
    va_end(arguments);
    return -(totalBytesWritten < 0);
}

template <class T>
tdk::Color<T>::Color()
{
}

template <class T>
tdk::Layer tdk::Color<T>::getLayer() const
{
    return m_layer;
}

template <class T>
void tdk::Color<T>::setLayer(tdk::Layer layer)
{
    m_layer = filterLayer(layer);
}

template <class T>
tdk::Layer tdk::Color<T>::filterLayer(Layer layer)
{
    return layer == tdk::Layer::Foreground || layer == tdk::Layer::Background
               ? layer
               : tdk::Layer::Foreground;
}

tdk::Coordinate::Coordinate() : m_column(0), m_row(0)
{
}

tdk::Coordinate::Coordinate(unsigned short column, unsigned short row)
    : m_column(column), m_row(row)
{
}

unsigned short tdk::Coordinate::getColumn() const
{
    return m_column;
}

unsigned short tdk::Coordinate::getRow() const
{
    return m_row;
}

void tdk::Coordinate::setColumn(unsigned short column)
{
    m_column = column;
}

void tdk::Coordinate::setRow(unsigned short row)
{
    m_row = row;
}

tdk::Dimensions::Dimensions() : m_totalColumns(0), m_totalRows(0)
{
}

tdk::Dimensions::Dimensions(unsigned short totalColumns, unsigned short totalRows)
    : m_totalColumns(totalColumns), m_totalRows(totalRows)
{
}

unsigned short tdk::Dimensions::getTotalColumns() const
{
    return m_totalColumns;
}

unsigned short tdk::Dimensions::getTotalRows() const
{
    return m_totalRows;
}

tdk::Effects::Effects(int code, bool isToEnable)
    : m_code(filterCode(code)), m_isToEnable(isToEnable)
{
}

tdk::Effects::Effects(EffectCode code, bool isToEnable)
    : m_code(filterCode(1 << static_cast<int>(code))), m_isToEnable(isToEnable)
{
}

int tdk::Effects::filterCode(int code)
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

int tdk::Effects::getCode() const
{
    return m_code;
}

bool tdk::Effects::getIsToEnable() const
{
    return m_isToEnable;
}

tdk::HexColor::HexColor(unsigned int code, Layer layer) : m_code(filterCode(code))
{
    m_layer = filterLayer(layer);
}

tdk::HexColor::HexColor(RGBColor color)
    : m_code(color.getRed() << 16 | color.getGreen() << 8 | color.getBlue())
{
    m_layer = color.getLayer();
}

unsigned int tdk::HexColor::filterCode(unsigned int code)
{
    return (std::min)(static_cast<int>(code), 0xffffff);
}

tdk::HexColor tdk::HexColor::invert() const
{
    return invertColor(this);
}

std::string tdk::HexColor::toString(bool isUpper, bool hasPrefix) const
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

unsigned int tdk::HexColor::getCode() const
{
    return m_code;
}

void tdk::HexColor::setCode(unsigned int code)
{
    m_code = filterCode(code);
}

tdk::RGBColor::RGBColor(unsigned char red, unsigned char green, unsigned char blue, Layer layer)
    : m_red(red), m_green(green), m_blue(blue)
{
    m_layer = filterLayer(layer);
}

tdk::RGBColor::RGBColor(HexColor color)
    : m_red(color.getCode() >> 16 & 0xff), m_green(color.getCode() >> 8 & 0xff),
      m_blue(color.getCode() & 0xff)
{
    m_layer = color.getLayer();
}

tdk::RGBColor tdk::RGBColor::invert() const
{
    return invertColor(this);
}

unsigned char tdk::RGBColor::getRed() const
{
    return m_red;
}

unsigned char tdk::RGBColor::getGreen() const
{
    return m_green;
}

unsigned char tdk::RGBColor::getBlue() const
{
    return m_blue;
}

void tdk::RGBColor::setRed(unsigned char red)
{
    m_red = red;
}

void tdk::RGBColor::setGreen(unsigned char green)
{
    m_green = green;
}

void tdk::RGBColor::setBlue(unsigned char blue)
{
    m_blue = blue;
}

tdk::XColor::XColor(unsigned char code, Layer layer) : m_code(code)
{
    m_layer = filterLayer(layer);
}

tdk::XColor::XColor(XColorCode code, Layer layer) : m_code(filterCode(code))
{
    m_layer = filterLayer(layer);
}

tdk::XColor tdk::XColor::invert() const
{
    return invertColor(this);
}

short tdk::XColor::filterCode(XColorCode code)
{
    return (std::max)((std::min)(static_cast<int>(code), 255),
                      static_cast<int>(tdk::XColorCode::Default));
}

short tdk::XColor::getCode() const
{
    return m_code;
}

void tdk::XColor::setCode(unsigned char code)
{
    m_code = code;
}

void tdk::XColor::setCode(XColorCode code)
{
    m_code = filterCode(code);
}

std::ostream& tdk::operator<<(std::ostream& stream, Effects effects)
{
    for (int ansiCode = 0; ansiCode < 10; ++ansiCode)
    {
        if (effects.getCode() & 1 << ansiCode)
        {
            std::cout << "\x1b[" << ansiCode + (!effects.getIsToEnable() * 20) << "m";
        }
    }
    return stream;
}

std::ostream& tdk::operator<<(std::ostream& stream, HexColor color)
{
    return stream << RGBColor(color);
}

std::ostream& tdk::operator<<(std::ostream& stream, RGBColor color)
{
    CHECK_STREAM_TTY_STATUS();
    return stream << "\x1b[" << static_cast<int>(color.getLayer()) << "8;2;"
                  << static_cast<int>(color.getRed()) << ";" << static_cast<int>(color.getGreen())
                  << ";" << static_cast<int>(color.getBlue()) << "m";
}

std::ostream& tdk::operator<<(std::ostream& stream, XColor color)
{
    CHECK_STREAM_TTY_STATUS();
    return color.getCode() == static_cast<int>(tdk::XColorCode::Default)
               ? stream << "\x1b[" << static_cast<int>(color.getLayer()) << "9m"
               : stream << "\x1b[" << static_cast<int>(color.getLayer()) << "8;5;"
                        << color.getCode() << "m";
}

std::ostream& tdk::operator<<(std::ostream& stream, Weight weight)
{
    CHECK_STREAM_TTY_STATUS();
    return weight == Weight::Default ? stream << "\x1b[22m"
                                     : stream << "\x1b[22;" << static_cast<int>(weight) << "m";
}

bool tdk::operator==(XColor color0, XColor color1)
{
    return color0.getLayer() == color1.getLayer() && color0.getCode() == color1.getCode();
}

bool tdk::operator==(HexColor color0, HexColor color1)
{
    return color0.getLayer() == color1.getLayer() && color0.getCode() == color1.getCode();
}

bool tdk::operator==(RGBColor color0, RGBColor color1)
{
    return color0.getLayer() == color1.getLayer() && color0.getRed() == color1.getRed() &&
           color0.getGreen() == color1.getGreen() && color0.getBlue() == color1.getBlue();
}

bool tdk::operator==(HexColor hexColor, RGBColor rgbColor)
{
    return hexColor == HexColor(rgbColor);
}

bool tdk::operator==(RGBColor rgbColor, HexColor hexColor)
{
    return HexColor(rgbColor) == hexColor;
}

bool tdk::operator==(Coordinate coordinate0, Coordinate coordinate1)
{
    return coordinate0.getColumn() == coordinate1.getColumn() &&
           coordinate0.getRow() == coordinate1.getRow();
}

int tdk::operator|(EffectCode code0, EffectCode code1)
{
    return 1 << static_cast<int>(code0) | 1 << static_cast<int>(code1);
}

void tdk::clearCursorLine()
{
    writeANSISequence("\x1b[2K\x1b[1G");
}

void tdk::clearInputBuffer()
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

void tdk::clearWindow()
{
    writeANSISequence("\x1b[2J\x1b[1;1H");
}

void tdk::closeAlternateWindow()
{
    writeANSISequence("\x1b[?1049l");
}

int tdk::getCursorCoordinate(Coordinate& coordinate)
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
    if (writeANSISequence("\x1b[6n") || tcgetattr(STDIN_FILENO, &attributes))
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

int tdk::getWindowDimensions(Dimensions& dimensions)
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

bool tdk::isTTY(Stream stream)
{
    prepareCacheAndStreams();
    return static_cast<int>(stream) >= static_cast<int>(tdk::Stream::Input) &&
                   static_cast<int>(stream) <= static_cast<int>(tdk::Stream::Error)
               ? IS_TTY(stream)
               : false;
}

void tdk::openAlternateWindow()
{
    writeANSISequence("\x1b[?1049h\x1b[2J\x1b[1;1H");
}

void tdk::ringBell()
{
    writeANSISequence("\7");
}

void tdk::setCursorCoordinate(unsigned short column, unsigned short row)
{
    writeANSISequence("\x1b[%hu;%huH", row + 1, column + 1);
}

void tdk::setCursorCoordinate(Coordinate coordinate)
{
    setCursorCoordinate(coordinate.getColumn(), coordinate.getRow());
}

void tdk::setCursorShape(CursorShape shape)
{
    writeANSISequence(
        "\x1b[%d q",
        static_cast<int>(static_cast<int>(shape) >= static_cast<int>(tdk::CursorShape::Default) &&
                                 static_cast<int>(shape) <= static_cast<int>(tdk::CursorShape::Bar)
                             ? shape
                             : tdk::CursorShape::Default));
}

void tdk::setWindowTitle(std::string title)
{
    writeANSISequence("\x1b]0;%s\7", title.c_str());
}

void tdk::setCursorVisibility(bool isToShow)
{
    writeANSISequence("\x1b[?25%c", isToShow ? 'h' : 'l');
}
