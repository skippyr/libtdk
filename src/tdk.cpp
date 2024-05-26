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

#include "tdk.hpp"

#define IS_TTY(stream_a)                                                       \
  static_cast<bool>(cache_g & 1 << static_cast<int>(stream_a))
#define CHECK_STREAM_TTY_STATUS()                                              \
  prepareStreamsAndCache();                                                    \
  if ((stream.rdbuf() == std::cout.rdbuf() && !IS_TTY(tdk::Stream::Output)) || \
      (stream.rdbuf() == std::cerr.rdbuf() && !IS_TTY(tdk::Stream::Error))) {  \
    return stream;                                                             \
  }
#define HAS_CACHED_TTY_FLAG (1 << 7)
#ifdef _WIN32
#define TTY_CACHE(stream_a)                                                    \
  (!!(_isatty(_fileno(!static_cast<int>(stream_a)       ? stdin                \
                      : static_cast<int>(stream_a) == 1 ? stdout               \
                                                        : stderr)))            \
   << static_cast<int>(stream_a))
#else
#define TTY_CACHE(stream_a)                                                    \
  (isatty(fileno(!static_cast<int>(stream_a)       ? stdin                     \
                 : static_cast<int>(stream_a) == 1 ? stdout                    \
                                                   : stderr))                  \
   << static_cast<int>(stream_a))
#endif

static void prepareStreamsAndCache();
static int writeANSISequence(const char *format, ...);

static char cache_g = 0;

static void prepareStreamsAndCache() {
  if (!(cache_g & HAS_CACHED_TTY_FLAG)) {
    cache_g = TTY_CACHE(tdk::Stream::Input) | TTY_CACHE(tdk::Stream::Output) |
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

static int writeANSISequence(const char *format, ...) {
  prepareStreamsAndCache();
  if (!IS_TTY(tdk::Stream::Output) && !IS_TTY(tdk::Stream::Error)) {
    return -1;
  }
  std::va_list arguments;
  va_start(arguments, format);
  int totalBytesWritten = std::vfprintf(
      IS_TTY(tdk::Stream::Output) ? stdout : stderr, format, arguments);
  va_end(arguments);
  return -(totalBytesWritten < 0);
}

tdk::Coordinate::Coordinate() : column_m(0), row_m(0) {}

tdk::Coordinate::Coordinate(unsigned short column, unsigned short row)
    : column_m(column), row_m(row) {}

tdk::Effect::Effect(int code, bool isToEnable)
    : code_m(code), isToEnable_m(isToEnable) {}

tdk::Effect::Effect(tdk::EffectCode code, bool isToEnable)
    : code_m(1 << static_cast<int>(code)), isToEnable_m(isToEnable) {}

tdk::HexColor::HexColor(unsigned int code, Layer layer)
    : code_m((std::min)(static_cast<int>(code), 0xffffff)), layer_m(layer) {}

tdk::HexColor tdk::HexColor::invert_m() {
  HexColor color = *this;
  color.layer_m = color.layer_m == tdk::Layer::Foreground
                      ? tdk::Layer::Background
                      : tdk::Layer::Foreground;
  return color;
}

tdk::Region::Region()
    : totalColumns_m(0), totalRows_m(0), area_m(0), topLeftCoordinate_m(),
      topRightCoordinate_m(), bottomLeftCoordinate_m(),
      bottomRightCoordinate_m() {}

tdk::Region::Region(unsigned short totalColumns, unsigned short totalRows)
    : totalColumns_m(totalColumns), totalRows_m(totalRows),
      area_m(totalColumns * totalRows), topLeftCoordinate_m(0, 0),
      topRightCoordinate_m(totalColumns - 1, 0),
      bottomLeftCoordinate_m(0, totalRows - 1),
      bottomRightCoordinate_m(totalColumns - 1, totalRows - 1) {}

tdk::Region::Region(Coordinate &cornerCoordinate0,
                    Coordinate &cornerCoordinate1) {
  unsigned short maxColumn =
      (std::max)(cornerCoordinate0.column_m, cornerCoordinate1.column_m);
  unsigned short minColumn =
      (std::min)(cornerCoordinate0.column_m, cornerCoordinate1.column_m);
  unsigned short maxRow =
      (std::max)(cornerCoordinate0.row_m, cornerCoordinate1.row_m);
  unsigned short minRow =
      (std::min)(cornerCoordinate0.row_m, cornerCoordinate1.row_m);
  totalColumns_m = maxColumn - minColumn;
  totalRows_m = maxRow - minRow;
  area_m = totalColumns_m * totalRows_m;
  topLeftCoordinate_m = Coordinate(minColumn, minRow);
  topRightCoordinate_m = Coordinate(maxColumn, minRow);
  bottomLeftCoordinate_m = Coordinate(minColumn, maxRow);
  bottomRightCoordinate_m = Coordinate(maxColumn, maxRow);
}

tdk::RGBColor::RGBColor(unsigned char red, unsigned char green,
                        unsigned char blue, Layer layer)
    : red_m(red), green_m(green), blue_m(blue), layer_m(layer) {}

tdk::RGBColor::RGBColor(HexColor &color)
    : red_m(color.code_m >> 16 & 0xff), green_m(color.code_m >> 8 & 0xff),
      blue_m(color.code_m & 0xff), layer_m(color.layer_m) {}

tdk::RGBColor tdk::RGBColor::invert_m() {
  RGBColor color = *this;
  color.layer_m = color.layer_m == tdk::Layer::Foreground
                      ? tdk::Layer::Background
                      : tdk::Layer::Foreground;
  return color;
}

tdk::XColor::XColor(unsigned char code, Layer layer)
    : code_m(code), layer_m(layer) {}

tdk::XColor::XColor(XColorCode code, Layer layer)
    : code_m(static_cast<int>(code)), layer_m(layer) {}

tdk::XColor tdk::XColor::invert_m() {
  XColor color = *this;
  color.layer_m = color.layer_m == tdk::Layer::Foreground
                      ? tdk::Layer::Background
                      : tdk::Layer::Foreground;
  return color;
}

std::ostream &tdk::operator<<(std::ostream &stream, Effect effect) {
  for (int code = 0; code < 10; ++code) {
    if (effect.code_m & 1 << code) {
      std::cout << "\x1b[" << code + (!effect.isToEnable_m * 20) << "m";
    }
  }
  return stream;
}

std::ostream &tdk::operator<<(std::ostream &stream, HexColor &color) {
  return stream << RGBColor(color);
}

std::ostream &tdk::operator<<(std::ostream &stream, RGBColor &color) {
  CHECK_STREAM_TTY_STATUS();
  return stream << "\x1b[" << static_cast<int>(color.layer_m) << "8;2;"
                << static_cast<int>(color.red_m) << ";"
                << static_cast<int>(color.green_m) << ";"
                << static_cast<int>(color.blue_m) << "m";
}

std::ostream &tdk::operator<<(std::ostream &stream, XColor &color) {
  CHECK_STREAM_TTY_STATUS();
  return color.code_m == static_cast<int>(tdk::XColorCode::Default)
             ? stream << "\x1b[" << static_cast<int>(color.layer_m) << "9m"
             : stream << "\x1b[" << static_cast<int>(color.layer_m) << "8;5;"
                      << color.code_m << "m";
}

std::ostream &tdk::operator<<(std::ostream &stream, Weight weight) {
  CHECK_STREAM_TTY_STATUS();
  return weight == Weight::Default
             ? stream << "\x1b[22m"
             : stream << "\x1b[22;" << static_cast<int>(weight) << "m";
}

int tdk::operator|(EffectCode code0, EffectCode code1) {
  return 1 << static_cast<int>(code0) | 1 << static_cast<int>(code1);
}

void tdk::clearCursorLine() { writeANSISequence("\x1b[2K\x1b[1G"); }

void tdk::clearInputBuffer() {
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

void tdk::clearWindow() { writeANSISequence("\x1b[2J\x1b[1;1H"); }

void tdk::closeAlternateWindow() { writeANSISequence("\x1b[?1049l"); }

int tdk::getCursorCoordinate(Coordinate &coordinate) {
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
  if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),
                                  &bufferInfo) &&
      !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE),
                                  &bufferInfo)) {
    return -1;
  }
  coordinate.column_m =
      bufferInfo.dwCursorPosition.X - bufferInfo.srWindow.Left;
  coordinate.row_m = bufferInfo.dwCursorPosition.Y - bufferInfo.srWindow.Top;
#else
  struct termios attributes;
  clearInputBuffer();
  if (writeANSISequence("\x1b[6n") || tcgetattr(STDIN_FILENO, &attributes)) {
    return -1;
  }
  attributes.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
  int totalMatchesRead =
      std::scanf("\x1b[%hu;%huR", &coordinate.row_m, &coordinate.column_m);
  attributes.c_lflag |= ICANON | ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
  if (totalMatchesRead != 2) {
    return -1;
  }
  --coordinate.column_m;
  --coordinate.row_m;
#endif
  return 0;
}

int tdk::getWindowRegion(Region &region) {
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
  if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),
                                  &bufferInfo) &&
      !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE),
                                  &bufferInfo)) {
    return -1;
  }
  region = Region(bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1,
                  bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1);
#else
  struct winsize size;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) &&
      ioctl(STDIN_FILENO, TIOCGWINSZ, &size) &&
      ioctl(STDERR_FILENO, TIOCGWINSZ, &size)) {
    return -1;
  }
  region = Region(size.ws_col, size.ws_row);
#endif
  return 0;
}

bool tdk::isTTY(Stream stream) {
  prepareStreamsAndCache();
  return IS_TTY(stream);
}

void tdk::openAlternateWindow() {
  writeANSISequence("\x1b[?1049h\x1b[2J\x1b[1;1H");
}

void tdk::ringBell() { writeANSISequence("\7"); }

void tdk::setCursorCoordinate(unsigned short column, unsigned short row) {
  writeANSISequence("\x1b[%hu;%huH", row + 1, column + 1);
}

void tdk::setCursorCoordinate(Coordinate &coordinate) {
  setCursorCoordinate(coordinate.column_m, coordinate.row_m);
}

void tdk::setCursorShape(CursorShape shape) {
  writeANSISequence("\x1b[%d q", static_cast<int>(shape));
}

void tdk::setWindowTitle(std::string title) {
  writeANSISequence("\x1b]0;%s\7", title.c_str());
}

void tdk::setCursorVisibility(bool isToShow) {
  writeANSISequence("\x1b[?25%c", isToShow ? 'h' : 'l');
}
