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

#include "tmk.hpp"

/**
 * @brief Checks if a standard terminal stream is a TTY.
 * @param a_stream The stream to be checked. It must be a value from the
 * tmk::Stream enum class.
 * @returns A boolean that states the stream is a TTY.
 */
#define IS_TTY(a_stream)                                                       \
  static_cast<bool>(g_cache & 1 << static_cast<int>(a_stream))
/**
 * @brief Checks the TTY status of a standard terminal stream being targeted and
 * cause an early return if it is not a TTY.
 */
#define CHECK_STREAM_TTY_STATUS()                                              \
  prepareCacheAndStreams();                                                    \
  if ((stream.rdbuf() == std::cout.rdbuf() && !IS_TTY(tmk::Stream::Output)) || \
      (stream.rdbuf() == std::cerr.rdbuf() && !IS_TTY(tmk::Stream::Error))) {  \
    return stream;                                                             \
  }
/** @brief A bitmask flag that states the cache has already been filled up. */
#define HAS_CACHED_TTY_FLAG (1 << 7)
#ifdef _WIN32
/**
 * @brief Creates the TTY status cache of a standard terminal stream.
 * @param a_stream The stream to be checked. It must be a value from the
 * tmk::Stream enum class.
 * @returns The TTY status cache of the stream.
 */
#define TTY_CACHE(a_stream)                                                    \
  (!!(_isatty(_fileno(a_stream == tmk::Stream::Input    ? stdin                \
                      : a_stream == tmk::Stream::Output ? stdout               \
                                                        : stderr)))            \
   << static_cast<int>(a_stream))
#else
/**
 * @brief Creates the TTY status cache of a standard terminal stream.
 * @param a_stream The stream to be checked. It must be a value from the
 * tmk::Stream enum class.
 * @returns The TTY status cache of the stream.
 */
#define TTY_CACHE(a_stream)                                                    \
  (isatty(fileno(a_stream == tmk::Stream::Input    ? stdin                     \
                 : a_stream == tmk::Stream::Output ? stdout                    \
                                                   : stderr))                  \
   << static_cast<int>(a_stream))
#endif

/**
 * @brief Inverts the layer where a color applies on.
 * @tparam The type of the color.
 * @param color The color to affected.
 * @returns The color with its layer inverted.
 */
template <class T> static T invertColor(const T *color);
/**
 * @brief Caches information about the TTY statuses of the standard terminal
 * streams and sets the ENABLE_VIRTUAL_TERMINAL_PROCESSING mode flag on Windows.
 */
static void prepareCacheAndStreams();
/**
 * @brief Formats and writes an ANSI escape sequence to a valid TTY stream.
 * @param format The format to be used. It uses the same specifiers as the
 * printf function family.
 * @param ... The arguments to be formatted.
 * @returns 0 if successful or -1 otherwise.
 */
static int writeANSISequence(const char *format, ...);

/**
 * @brief A cache containing the TTY statuses of the standard terminal streams.
 */
static char g_cache = 0;

template <class T> static T invertColor(const T *color) {
  T copy = *color;
  copy.setLayer(copy.getLayer() == tmk::Layer::Foreground
                    ? tmk::Layer::Background
                    : tmk::Layer::Foreground);
  return copy;
}

static void prepareCacheAndStreams() {
  if (!(g_cache & HAS_CACHED_TTY_FLAG)) {
    g_cache = TTY_CACHE(tmk::Stream::Input) | TTY_CACHE(tmk::Stream::Output) |
              TTY_CACHE(tmk::Stream::Error) | HAS_CACHED_TTY_FLAG;
  }
#ifdef _WIN32
  HANDLE handle;
  DWORD mode;
  (GetConsoleMode((handle = GetStdHandle(STD_OUTPUT_HANDLE)), &mode) ||
   GetConsoleMode((handle = GetStdHandle(STD_ERROR_HANDLE)), &mode)) &&
      SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

static int writeANSISequence(const char *format, ...) {
  prepareCacheAndStreams();
  if (!IS_TTY(tmk::Stream::Output) && !IS_TTY(tmk::Stream::Error)) {
    return -1;
  }
  std::va_list arguments;
  va_start(arguments, format);
  int totalBytesWritten = std::vfprintf(
      IS_TTY(tmk::Stream::Output) ? stdout : stderr, format, arguments);
  va_end(arguments);
  return -(totalBytesWritten < 0);
}

template <class T> tmk::Color<T>::Color() {}

template <class T> tmk::Layer tmk::Color<T>::getLayer() const {
  return m_layer;
}

template <class T> void tmk::Color<T>::setLayer(tmk::Layer layer) {
  m_layer = filterLayer(layer);
}

template <class T> tmk::Layer tmk::Color<T>::filterLayer(Layer layer) {
  return layer == tmk::Layer::Foreground || layer == tmk::Layer::Background
             ? layer
             : tmk::Layer::Foreground;
}

tmk::Coordinate::Coordinate() : m_column(0), m_row(0) {}

tmk::Coordinate::Coordinate(unsigned short column, unsigned short row)
    : m_column(column), m_row(row) {}

unsigned short tmk::Coordinate::getColumn() const { return m_column; }

unsigned short tmk::Coordinate::getRow() const { return m_row; }

void tmk::Coordinate::setColumn(unsigned short column) { m_column = column; }

void tmk::Coordinate::setRow(unsigned short row) { m_row = row; }

tmk::Dimensions::Dimensions() : m_totalColumns(0), m_totalRows(0) {}

tmk::Dimensions::Dimensions(unsigned short totalColumns,
                            unsigned short totalRows)
    : m_totalColumns(totalColumns), m_totalRows(totalRows) {}

unsigned short tmk::Dimensions::getTotalColumns() const {
  return m_totalColumns;
}

unsigned short tmk::Dimensions::getTotalRows() const { return m_totalRows; }

tmk::Effects::Effects(int code, bool isToEnable)
    : m_code(filterCode(code)), m_isToEnable(isToEnable) {}

tmk::Effects::Effects(EffectCode code, bool isToEnable)
    : m_code(filterCode(1 << static_cast<int>(code))),
      m_isToEnable(isToEnable) {}

int tmk::Effects::filterCode(int code) {
  int filteredCodes = 0;
  for (int ansiCode = 0; ansiCode < 10; ++ansiCode) {
    if (ansiCode != 6 && code & 1 << ansiCode) {
      filteredCodes |= 1 << ansiCode;
    }
  }
  return filteredCodes;
}

int tmk::Effects::getCode() const { return m_code; }

bool tmk::Effects::getIsToEnable() const { return m_isToEnable; }

tmk::HexColor::HexColor(unsigned int code, Layer layer)
    : m_code(filterCode(code)) {
  m_layer = filterLayer(layer);
}

tmk::HexColor::HexColor(RGBColor color)
    : m_code(color.getRed() << 16 | color.getGreen() << 8 | color.getBlue()) {
  m_layer = color.getLayer();
}

unsigned int tmk::HexColor::filterCode(unsigned int code) {
  return (std::min)(static_cast<int>(code), 0xffffff);
}

tmk::HexColor tmk::HexColor::invert() const { return invertColor(this); }

std::string tmk::HexColor::toString(bool isUpper, bool hasPrefix) const {
  std::ostringstream stringStream;
  stringStream << std::hex << std::setw(6) << std::setfill('0') << m_code;
  std::string string = stringStream.str();
  if (isUpper) {
    for (int offset = 0; offset < string.length(); ++offset) {
      if (string[offset] >= 97 && string[offset] <= 122) {
        string[offset] = string[offset] - 32;
      }
    }
  }
  if (hasPrefix) {
    string = "0x" + string;
  }
  return string;
}

unsigned int tmk::HexColor::getCode() const { return m_code; }

void tmk::HexColor::setCode(unsigned int code) { m_code = filterCode(code); }

tmk::RGBColor::RGBColor(unsigned char red, unsigned char green,
                        unsigned char blue, Layer layer)
    : m_red(red), m_green(green), m_blue(blue) {
  m_layer = filterLayer(layer);
}

tmk::RGBColor::RGBColor(HexColor color)
    : m_red(color.getCode() >> 16 & 0xff), m_green(color.getCode() >> 8 & 0xff),
      m_blue(color.getCode() & 0xff) {
  m_layer = color.getLayer();
}

tmk::RGBColor tmk::RGBColor::invert() const { return invertColor(this); }

unsigned char tmk::RGBColor::getRed() const { return m_red; }

unsigned char tmk::RGBColor::getGreen() const { return m_green; }

unsigned char tmk::RGBColor::getBlue() const { return m_blue; }

void tmk::RGBColor::setRed(unsigned char red) { m_red = red; }

void tmk::RGBColor::setGreen(unsigned char green) { m_green = green; }

void tmk::RGBColor::setBlue(unsigned char blue) { m_blue = blue; }

tmk::XColor::XColor(unsigned char code, Layer layer) : m_code(code) {
  m_layer = filterLayer(layer);
}

tmk::XColor::XColor(XColorCode code, Layer layer) : m_code(filterCode(code)) {
  m_layer = filterLayer(layer);
}

tmk::XColor tmk::XColor::invert() const { return invertColor(this); }

short tmk::XColor::filterCode(XColorCode code) {
  return (std::max)((std::min)(static_cast<int>(code), 255),
                    static_cast<int>(tmk::XColorCode::Default));
}

short tmk::XColor::getCode() const { return m_code; }

void tmk::XColor::setCode(unsigned char code) { m_code = code; }

void tmk::XColor::setCode(XColorCode code) { m_code = filterCode(code); }

std::ostream &tmk::operator<<(std::ostream &stream, Effects effects) {
  for (int ansiCode = 0; ansiCode < 10; ++ansiCode) {
    if (effects.getCode() & 1 << ansiCode) {
      std::cout << "\x1b[" << ansiCode + (!effects.getIsToEnable() * 20) << "m";
    }
  }
  return stream;
}

std::ostream &tmk::operator<<(std::ostream &stream, HexColor color) {
  return stream << RGBColor(color);
}

std::ostream &tmk::operator<<(std::ostream &stream, RGBColor color) {
  CHECK_STREAM_TTY_STATUS();
  return stream << "\x1b[" << static_cast<int>(color.getLayer()) << "8;2;"
                << static_cast<int>(color.getRed()) << ";"
                << static_cast<int>(color.getGreen()) << ";"
                << static_cast<int>(color.getBlue()) << "m";
}

std::ostream &tmk::operator<<(std::ostream &stream, XColor color) {
  CHECK_STREAM_TTY_STATUS();
  return color.getCode() == static_cast<int>(tmk::XColorCode::Default)
             ? stream << "\x1b[" << static_cast<int>(color.getLayer()) << "9m"
             : stream << "\x1b[" << static_cast<int>(color.getLayer()) << "8;5;"
                      << color.getCode() << "m";
}

std::ostream &tmk::operator<<(std::ostream &stream, Weight weight) {
  CHECK_STREAM_TTY_STATUS();
  return weight == Weight::Default
             ? stream << "\x1b[22m"
             : stream << "\x1b[22;" << static_cast<int>(weight) << "m";
}

bool tmk::operator==(XColor color0, XColor color1) {
  return color0.getLayer() == color1.getLayer() &&
         color0.getCode() == color1.getCode();
}

bool tmk::operator==(HexColor color0, HexColor color1) {
  return color0.getLayer() == color1.getLayer() &&
         color0.getCode() == color1.getCode();
}

bool tmk::operator==(RGBColor color0, RGBColor color1) {
  return color0.getLayer() == color1.getLayer() &&
         color0.getRed() == color1.getRed() &&
         color0.getGreen() == color1.getGreen() &&
         color0.getBlue() == color1.getBlue();
}

bool tmk::operator==(HexColor hexColor, RGBColor rgbColor) {
  return hexColor == HexColor(rgbColor);
}

bool tmk::operator==(RGBColor rgbColor, HexColor hexColor) {
  return HexColor(rgbColor) == hexColor;
}

bool tmk::operator==(Coordinate coordinate0, Coordinate coordinate1) {
  return coordinate0.getColumn() == coordinate1.getColumn() &&
         coordinate0.getRow() == coordinate1.getRow();
}

int tmk::operator|(EffectCode code0, EffectCode code1) {
  return 1 << static_cast<int>(code0) | 1 << static_cast<int>(code1);
}

void tmk::clearCursorLine() { writeANSISequence("\x1b[2K\x1b[1G"); }

void tmk::clearInputBuffer() {
#ifdef _WIN32
  FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
#else
  struct termios attributes;
  int flags = fcntl(STDIN_FILENO, F_GETFL);
  if (tcgetattr(STDIN_FILENO, &attributes)) {
    return;
  }
  attributes.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
  fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
  while (std::getchar() != EOF) {
  }
  attributes.c_lflag |= ICANON | ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
  fcntl(STDIN_FILENO, F_SETFL, flags);
#endif
}

void tmk::clearWindow() { writeANSISequence("\x1b[2J\x1b[1;1H"); }

void tmk::closeAlternateWindow() { writeANSISequence("\x1b[?1049l"); }

int tmk::getCursorCoordinate(Coordinate &coordinate) {
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
  if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),
                                  &bufferInfo) &&
      !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE),
                                  &bufferInfo)) {
    return -1;
  }
  coordinate =
      Coordinate(bufferInfo.dwCursorPosition.X - bufferInfo.srWindow.Left,
                 bufferInfo.dwCursorPosition.Y - bufferInfo.srWindow.Top);
#else
  struct termios attributes;
  clearInputBuffer();
  if (writeANSISequence("\x1b[6n") || tcgetattr(STDIN_FILENO, &attributes)) {
    return -1;
  }
  attributes.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
  unsigned short column;
  unsigned short row;
  int totalMatchesRead = std::scanf("\x1b[%hu;%huR", &row, &column);
  attributes.c_lflag |= ICANON | ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
  if (totalMatchesRead != 2) {
    return -1;
  }
  coordinate = Coordinate(--column, --row);
#endif
  return 0;
}

int tmk::getWindowDimensions(Dimensions &dimensions) {
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
  if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),
                                  &bufferInfo) &&
      !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE),
                                  &bufferInfo)) {
    return -1;
  }
  dimensions =
      Dimensions(bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1,
                 bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1);
#else
  struct winsize size;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) &&
      ioctl(STDIN_FILENO, TIOCGWINSZ, &size) &&
      ioctl(STDERR_FILENO, TIOCGWINSZ, &size)) {
    return -1;
  }
  dimensions = Dimensions(size.ws_col, size.ws_row);
#endif
  return 0;
}

bool tmk::isTTY(Stream stream) {
  prepareCacheAndStreams();
  return static_cast<int>(stream) >= static_cast<int>(tmk::Stream::Input) &&
                 static_cast<int>(stream) <=
                     static_cast<int>(tmk::Stream::Error)
             ? IS_TTY(stream)
             : false;
}

void tmk::openAlternateWindow() {
  writeANSISequence("\x1b[?1049h\x1b[2J\x1b[1;1H");
}

void tmk::ringBell() { writeANSISequence("\7"); }

void tmk::setCursorCoordinate(unsigned short column, unsigned short row) {
  writeANSISequence("\x1b[%hu;%huH", row + 1, column + 1);
}

void tmk::setCursorCoordinate(Coordinate coordinate) {
  setCursorCoordinate(coordinate.getColumn(), coordinate.getRow());
}

void tmk::setCursorShape(CursorShape shape) {
  writeANSISequence(
      "\x1b[%d q",
      static_cast<int>(
          static_cast<int>(shape) >=
                      static_cast<int>(tmk::CursorShape::Default) &&
                  static_cast<int>(shape) <=
                      static_cast<int>(tmk::CursorShape::Bar)
              ? shape
              : tmk::CursorShape::Default));
}

void tmk::setWindowTitle(std::string title) {
  writeANSISequence("\x1b]0;%s\7", title.c_str());
}

void tmk::setCursorVisibility(bool isToShow) {
  writeANSISequence("\x1b[?25%c", isToShow ? 'h' : 'l');
}
