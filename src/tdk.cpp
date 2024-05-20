#include "tdk.hpp"

static char g_cache = 0;

#define IS_TTY(stream)                                                         \
  static_cast<bool>(g_cache & 1 << static_cast<int>(stream))
#define CHECK_STREAM_TTY_STATUS()                                              \
  prepareStreamsAndCache();                                                    \
  if ((stream.rdbuf() == std::cout.rdbuf() && !IS_TTY(tdk::Stream::Output)) || \
      (stream.rdbuf() == std::cerr.rdbuf() && !IS_TTY(tdk::Stream::Error))) {  \
    return stream;                                                             \
  }
#define HAS_CACHED_TTY_BIT (1 << 7)
#define PARSE_KEY(condition, key)                                              \
  if (condition) {                                                             \
    event.m_key = key;                                                         \
    break;                                                                     \
  }
#ifdef _WIN32
#define TTY_CACHE(stream)                                                      \
  (!!(_isatty(_fileno(!static_cast<int>(stream)       ? stdin                  \
                      : static_cast<int>(stream) == 1 ? stdout                 \
                                                      : stderr)))              \
   << static_cast<int>(stream))
#else
#define TTY_CACHE(stream)                                                      \
  (isatty(fileno(!static_cast<int>(stream)       ? stdin                       \
                 : static_cast<int>(stream) == 1 ? stdout                      \
                                                 : stderr))                    \
   << static_cast<int>(stream))
#endif

static void prepareStreamsAndCache();
static int writeANSI(const char *format, ...);

static void prepareStreamsAndCache() {
  if (!(g_cache & HAS_CACHED_TTY_BIT)) {
    g_cache = TTY_CACHE(tdk::Stream::Input) | TTY_CACHE(tdk::Stream::Output) |
              TTY_CACHE(tdk::Stream::Error) | HAS_CACHED_TTY_BIT;
  }
#ifdef _WIN32
  HANDLE handle;
  DWORD mode;
  (GetConsoleMode((handle = GetStdHandle(STD_OUTPUT_HANDLE)), &mode) ||
   GetConsoleMode((handle = GetStdHandle(STD_ERROR_HANDLE)), &mode)) &&
      SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

static int writeANSI(const char *format, ...) {
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

tdk::Coordinate::Coordinate() : m_column(0), m_row(0) {}

tdk::Coordinate::Coordinate(unsigned short column, unsigned short row)
    : m_column(column), m_row(row) {}

tdk::Dimensions::Dimensions() : m_totalColumns(0), m_totalRows(0) {}

tdk::Dimensions::Dimensions(unsigned short totalColumns,
                            unsigned short totalRows)
    : m_totalColumns(totalColumns), m_totalRows(totalRows) {}

tdk::HexColor::HexColor(unsigned int code, Layer layer)
    : m_code((std::min)(static_cast<int>(code), 0xffffff)), m_layer(layer) {}

tdk::HexColor tdk::HexColor::m_invert() {
  HexColor color = *this;
  color.m_layer = color.m_layer == tdk::Layer::Foreground
                      ? tdk::Layer::Background
                      : tdk::Layer::Foreground;
  return color;
}

tdk::KeyEvent::KeyEvent() : m_key(0), m_hasAlt(false), m_hasCtrl(false) {}

tdk::RGBColor::RGBColor(unsigned char red, unsigned char green,
                        unsigned char blue, Layer layer)
    : m_red(red), m_green(green), m_blue(blue), m_layer(layer) {}

tdk::RGBColor::RGBColor(HexColor color)
    : m_red(color.m_code >> 16 & 0xff), m_green(color.m_code >> 8 & 0xff),
      m_blue(color.m_code & 0xff), m_layer(color.m_layer) {}

tdk::RGBColor tdk::RGBColor::m_invert() {
  RGBColor color = *this;
  color.m_layer = color.m_layer == tdk::Layer::Foreground
                      ? tdk::Layer::Background
                      : tdk::Layer::Foreground;
  return color;
}

tdk::XColor::XColor(unsigned char code, Layer layer)
    : m_code(code), m_layer(layer) {}

tdk::XColor::XColor(XColorCode code, Layer layer)
    : m_code(static_cast<int>(code)), m_layer(layer) {}

tdk::XColor tdk::XColor::m_invert() {
  XColor color = *this;
  color.m_layer = color.m_layer == tdk::Layer::Foreground
                      ? tdk::Layer::Background
                      : tdk::Layer::Foreground;
  return color;
}

std::ostream &tdk::operator<<(std::ostream &stream, Effect effect) {
  CHECK_STREAM_TTY_STATUS();
  return stream << "\x1b[" << static_cast<int>(effect) << "m";
}

std::ostream &tdk::operator>>(std::ostream &stream, Effect effect) {
  CHECK_STREAM_TTY_STATUS();
  return stream << "\x1b[" << static_cast<int>(effect) + 20 << "m";
}

std::ostream &tdk::operator<<(std::ostream &stream, HexColor color) {
  return stream << RGBColor(color);
}

std::ostream &tdk::operator<<(std::ostream &stream, RGBColor color) {
  CHECK_STREAM_TTY_STATUS();
  return stream << "\x1b[" << static_cast<int>(color.m_layer) << "8;2;"
                << static_cast<int>(color.m_red) << ";"
                << static_cast<int>(color.m_green) << ";"
                << static_cast<int>(color.m_blue) << "m";
}

std::ostream &tdk::operator<<(std::ostream &stream, XColor color) {
  CHECK_STREAM_TTY_STATUS();
  return color.m_code == static_cast<int>(tdk::XColorCode::Default)
             ? stream << "\x1b[" << static_cast<int>(color.m_layer) << "9m"
             : stream << "\x1b[" << static_cast<int>(color.m_layer) << "8;5;"
                      << color.m_code << "m";
}

std::ostream &tdk::operator<<(std::ostream &stream, Weight weight) {
  CHECK_STREAM_TTY_STATUS();
  return weight == Weight::Default
             ? stream << "\x1b[22m"
             : stream << "\x1b[22;" << static_cast<int>(weight) << "m";
}

bool tdk::operator==(int code, Key key) {
  return code == static_cast<int>(key);
}

bool tdk::operator!=(int code, Key key) {
  return code != static_cast<int>(key);
}

void tdk::clearCursorLine() { writeANSI("\x1b[2K\x1b[1G"); }

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

int tdk::getCursorCoordinate(Coordinate &coordinate) {
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
  if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),
                                  &bufferInfo) &&
      !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE),
                                  &bufferInfo)) {
    return -1;
  }
  coordinate.m_column =
      bufferInfo.dwCursorPosition.X - bufferInfo.srWindow.Left;
  coordinate.m_row = bufferInfo.dwCursorPosition.Y - bufferInfo.srWindow.Top;
#else
  struct termios attributes;
  clearInputBuffer();
  if (writeANSI("\x1b[6n") || tcgetattr(STDIN_FILENO, &attributes)) {
    return -1;
  }
  attributes.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
  int totalMatchesRead =
      std::scanf("\x1b[%hu;%huR", &coordinate.m_row, &coordinate.m_column);
  attributes.c_lflag |= ICANON | ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
  if (totalMatchesRead != 2) {
    return -1;
  }
  --coordinate.m_column;
  --coordinate.m_row;
#endif
  return 0;
}

int tdk::getWindowDimensions(Dimensions &dimensions) {
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
  if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),
                                  &bufferInfo) &&
      !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE),
                                  &bufferInfo)) {
    return -1;
  }
  dimensions.m_totalColumns =
      bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1;
  dimensions.m_totalRows =
      bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1;
#else
  struct winsize size;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) &&
      ioctl(STDIN_FILENO, TIOCGWINSZ, &size) &&
      ioctl(STDERR_FILENO, TIOCGWINSZ, &size)) {
    return -1;
  }
  dimensions.m_totalColumns = size.ws_col;
  dimensions.m_totalRows = size.ws_row;
#endif
  return 0;
}

bool tdk::isTTY(Stream stream) {
  prepareStreamsAndCache();
  return IS_TTY(stream);
}

tdk::EventStatus tdk::readKeyEvent(KeyEvent &event) {
  prepareStreamsAndCache();
  if (!IS_TTY(tdk::Stream::Input) || std::fwide(stdin, 0) > 0 ||
      (!IS_TTY(tdk::Stream::Output) && !IS_TTY(tdk::Stream::Error))) {
    return EventStatus::Failure;
  }
#ifdef _WIN32
  HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
  INPUT_RECORD record;
  DWORD mode;
  DWORD totalEventsRead;
  int buffer;
  GetConsoleMode(handle, &mode);
  SetConsoleMode(handle, mode & ~ENABLE_PROCESSED_INPUT);
  event = KeyEvent();
  while (true) {
    ReadConsoleInputW(handle, &record, 1, &totalEventsRead);
    if (record.EventType == WINDOW_BUFFER_SIZE_EVENT) {
      SetConsoleMode(handle, mode);
      return EventStatus::WindowResizeInterrupt;
    } else if (record.EventType != KEY_EVENT ||
               !record.Event.KeyEvent.bKeyDown ||
               record.Event.KeyEvent.wVirtualKeyCode == VK_CONTROL ||
               record.Event.KeyEvent.wVirtualKeyCode == VK_SHIFT ||
               record.Event.KeyEvent.wVirtualKeyCode == VK_MENU ||
               record.Event.KeyEvent.wVirtualKeyCode == VK_CAPITAL ||
               record.Event.KeyEvent.wVirtualKeyCode == VK_NUMLOCK ||
               record.Event.KeyEvent.wVirtualKeyCode == VK_SCROLL) {
      continue;
    }
    if ((buffer = record.Event.KeyEvent.uChar.UnicodeChar)) {
      if (buffer <= 26 && buffer != tdk::Key::Tab &&
          buffer != tdk::Key::Enter) {
        event.m_key = buffer + 96;
      } else if (buffer >= HIGH_SURROGATE_START &&
                 buffer <= HIGH_SURROGATE_END) {
        ReadConsoleInputW(handle, &record, 1, &totalEventsRead);
        ReadConsoleInputW(handle, &record, 1, &totalEventsRead);
        *(reinterpret_cast<short *>(&buffer) + 1) =
            record.Event.KeyEvent.uChar.UnicodeChar;
        WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<wchar_t *>(&buffer), 2,
                            reinterpret_cast<char *>(&event.m_key), 4, NULL,
                            NULL);
      } else {
        WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<wchar_t *>(&buffer), 1,
                            reinterpret_cast<char *>(&event.m_key), 4, NULL,
                            NULL);
      }
      event.m_hasAlt = record.Event.KeyEvent.dwControlKeyState &
                       (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED);
      event.m_hasCtrl = record.Event.KeyEvent.dwControlKeyState &
                        (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED);
      break;
    } else if (record.Event.KeyEvent.dwControlKeyState &
               (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED | LEFT_CTRL_PRESSED |
                RIGHT_CTRL_PRESSED | SHIFT_PRESSED)) {
      continue;
    }
    PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_LEFT &&
                  record.Event.KeyEvent.wVirtualKeyCode <= VK_DOWN,
              record.Event.KeyEvent.wVirtualKeyCode - VK_LEFT +
                  static_cast<int>(tdk::Key::LeftArrow));
    PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_PRIOR &&
                  record.Event.KeyEvent.wVirtualKeyCode <= VK_HOME,
              record.Event.KeyEvent.wVirtualKeyCode - VK_PRIOR +
                  static_cast<int>(tdk::Key::PageUp));
    PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_INSERT &&
                  record.Event.KeyEvent.wVirtualKeyCode <= VK_DELETE,
              record.Event.KeyEvent.wVirtualKeyCode - VK_INSERT +
                  static_cast<int>(tdk::Key::Insert));
    PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_F1 &&
                  record.Event.KeyEvent.wVirtualKeyCode <= VK_F12,
              record.Event.KeyEvent.wVirtualKeyCode - VK_F1 +
                  static_cast<int>(tdk::Key::F1));
  }
  SetConsoleMode(handle, mode);
#else
  struct termios attributes;
  int flags = fcntl(STDIN_FILENO, F_GETFL);
  tcgetattr(STDIN_FILENO, &attributes);
  attributes.c_lflag &= ~(ICANON | ECHO | ISIG);
  attributes.c_iflag &= ~IXON;
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
  event = KeyEvent();
  while (true) {
    char buffer[] = {0, 0, 0, 0, 0};
    buffer[0] = std::getchar();
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    if (buffer[0] == 27 && (buffer[1] = std::getchar()) == 79 ||
        buffer[1] == 91) {
      for (std::size_t offset = 2; offset < 5; ++offset) {
        buffer[offset] = std::getchar();
      }
      while (std::getchar() != EOF) {
      }
      PARSE_KEY(buffer[2] >= 65 && buffer[2] <= 68,
                buffer[2] - 65 + static_cast<int>(tdk::Key::UpArrow));
      PARSE_KEY(buffer[3] == 126,
                buffer[2] - 49 + static_cast<int>(tdk::Key::Home));
      PARSE_KEY(buffer[3] == 104 || (buffer[1] == 91 && buffer[2] == 80),
                !(buffer[3] == 104) + static_cast<int>(tdk::Key::Insert));
      PARSE_KEY(buffer[2] == 70 || buffer[2] == 72,
                buffer[2] == 72 ? static_cast<int>(tdk::Key::Home)
                                : static_cast<int>(tdk::Key::End));
      PARSE_KEY(buffer[2] >= 80 && buffer[2] <= 83,
                buffer[2] - 80 + static_cast<int>(tdk::Key::F1));
      PARSE_KEY(buffer[3] >= 65 && buffer[3] <= 69,
                buffer[3] - 65 + static_cast<int>(tdk::Key::F1));
      PARSE_KEY(buffer[4] == 126,
                buffer[3] == 53 ? static_cast<int>(tdk::Key::F5)
                : buffer[3] >= 55 && buffer[3] <= 57
                    ? buffer[3] - 55 + static_cast<int>(tdk::Key::F6)
                : buffer[3] == 48 || buffer[3] == 49
                    ? buffer[3] - 48 + static_cast<int>(tdk::Key::F9)
                    : buffer[3] - 51 + static_cast<int>(tdk::Key::F11));
      fcntl(STDIN_FILENO, F_SETFL, flags);
      continue;
    }
    if (buffer[0] & 1 << 7) {
      for (std::size_t offset = 1;
           offset < 1 + !!(buffer[0] & 1 << 6) + !!(buffer[0] & 1 << 5) +
                        !!(buffer[0] & 1 << 4);
           ++offset) {
        buffer[offset] = std::getchar();
      }
      event.m_key = *reinterpret_cast<int *>(buffer);
    } else if ((event.m_key =
                    (event.m_hasAlt = buffer[0] == 27 && buffer[1] != EOF)
                        ? buffer[1]
                        : buffer[0]) >= 0 &&
               event.m_key <= 26 && event.m_key != tdk::Key::Tab &&
               event.m_key != tdk::Key::Enter) {
      event.m_key = !event.m_key ? static_cast<int>(tdk::Key::Spacebar)
                                 : event.m_key + 96;
      event.m_hasCtrl = true;
    }
    break;
  }
  attributes.c_lflag |= ICANON | ECHO | ISIG;
  attributes.c_iflag |= IXON;
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
  fcntl(STDIN_FILENO, F_SETFL, flags);
#endif
  return EventStatus::Success;
}

void tdk::ringBell() { writeANSI("\7"); }

void tdk::setAlternateWindow(bool isToOpen) {
  writeANSI(isToOpen ? "\x1b[?1049h\x1b[2J\x1b[1;1H" : "\x1b[?1049l");
}

void tdk::setCursorCoordinate(unsigned short column, unsigned short row) {
  writeANSI("\x1b[%hu;%huH", row + 1, column + 1);
}

void tdk::setCursorCoordinate(Coordinate &coordinate) {
  setCursorCoordinate(coordinate.m_column, coordinate.m_row);
}

void tdk::setCursorShape(Shape shape) {
  writeANSI("\x1b[%d q", static_cast<int>(shape));
}

void tdk::setCursorVisibility(bool isToShow) {
  writeANSI("\x1b[?25%c", isToShow ? 'h' : 'l');
}
