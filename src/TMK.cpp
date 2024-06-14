#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#else
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#endif

#include "tmk.hpp"

#ifdef _WIN32
#define IS_TTY(a_streamFileNumber) _isatty(a_streamFileNumber)
#else
#define IS_TTY(a_streamFileNumber) isatty(a_streamFileNumber)
#endif
#define PARSE_KEY(a_condition, a_key)                                                                                                                                              \
    if (a_condition)                                                                                                                                                               \
    {                                                                                                                                                                              \
        key = a_key;                                                                                                                                                               \
        goto l_keyReadingEnd;                                                                                                                                                      \
    }

namespace TMK
{
    /**
     * @brief A boolean that states the stream TTY cache has been filled.
     */
    bool g_hasStreamTTYCache = false;
    /**
     * @brief A boolean that states the terminal input stream is a TTY.
     */
    bool g_isInputStreamTTY = false;
    /**
     * @brief A boolean that states the terminal output straem is a TTY.
     */
    bool g_isOutputStreamTTY = false;
    /**
     * @brief A boolean that states the terminal error stream is a TTY.
     */
    bool g_isErrorStreamTTY = false;

    /** @brief Contains methods to setup the environment and library functions. */
    class Setup
    {
    public:
        /**
         * @brief Initiatializes the terminal stream TTY cache.
         */
        static void initStreamTTYCache()
        {
            if (g_hasStreamTTYCache)
            {
                return;
            }
            g_hasStreamTTYCache = true;
            g_isInputStreamTTY = IS_TTY(0);
            g_isOutputStreamTTY = IS_TTY(1);
            g_isErrorStreamTTY = IS_TTY(2);
#ifdef _WIN32
            HANDLE handle;
            DWORD mode;
            SetConsoleOutputCP(CP_UTF8);
            (GetConsoleMode((handle = GetStdHandle(STD_OUTPUT_HANDLE)), &mode) || GetConsoleMode((handle = GetStdHandle(STD_ERROR_HANDLE)), &mode)) &&
                SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
        }

        /**
         * @brief Writes an ANSI escape sequence to the terminal output or error streams.
         * @param format The format to be used. It accepts the same specifiers as the printf function family.
         * @param arguments The arguments to be formatted.
         * @throws NoValidTTYException Thrown whenever the standard output and error streams are not TTY.
         * @throws WideCharacterOrientationException Thrown whenever the standard output or error stream is wide character oriented.
         */
        static void writeANSIEscapeSequence(std::string format, std::va_list arguments)
        {
            if (!Terminal::Output::isTTY() && !Terminal::Error::isTTY())
            {
                throw NoValidTTYException();
            }
            if (std::vfprintf(Terminal::Output::isTTY() ? stdout : stderr, format.c_str(), arguments) < 0)
            {
                throw WideCharacterOrientationException();
            }
        }

        /**
         * @brief Writes an ANSI escape sequence to the terminal output or error streams.
         * @param format The format to be used. It accepts the same specifiers as the printf function family.
         * @param ... The arguments to be formatted.
         * @throws NoValidTTYException Thrown whenever the standard output and error streams are not TTY.
         * @throws WideCharacterOrientationException Thrown whenever the standard output or error stream is wide character oriented.
         */
        static void writeANSIEscapeSequence(std::string format, ...)
        {
            std::va_list arguments;
            va_start(arguments, format);
            Setup::writeANSIEscapeSequence(format, arguments);
            va_end(arguments);
        }

        /**
         * @brief Formats and writes a string to a terminal stream.
         * @param file The file of the stream.
         * @param format The format to be used. It accepts the same specifiers as the printf function family.
         * @param arguments The arguments to be formatted.
         * @param hasNewLine A boolean that states a newline character should be appended at the end of the string.
         */
        static void write(std::FILE* file, const char* format, std::va_list arguments, bool hasNewLine)
        {
            initStreamTTYCache();
            if (file == stderr)
            {
                Terminal::Output::flush();
            }
            if (format && std::vfprintf(file, format, arguments) < 0)
            {
                throw WideCharacterOrientationException();
            }
            if (hasNewLine && std::fputc('\n', file) == EOF)
            {
                throw WideCharacterOrientationException();
            }
        }

        /**
         * @brief Reads a terminal event.
         * @param allowMouseCapture A boolean that states mouse events should be captured. If enabled, mouse selection will be disabled until it returns.
         * @param wait The time to wait for an event. If zero, it returns immediately. If negative, it waits until an event become available.
         * @param filter A function to be used to filter events while the timer is running.
         * @returns The information about the event read.
         */
        static EventInfo readEvent(bool allowMouseCapture, std::chrono::milliseconds wait, std::function<bool(EventInfo&)> filter)
        {
            if (!Terminal::Input::isTTY() || (!Terminal::Output::isTTY() && !Terminal::Error::isTTY()))
            {
                throw NoValidTTYException();
            }
            if (std::fwide(stdin, 0) > 0)
            {
                throw WideCharacterOrientationException();
            }
            EventInfo eventInfo = EventType::None;
#ifdef _WIN32
            HANDLE timerHandle = nullptr;
            HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
            DWORD mode;
            GetConsoleMode(inputHandle, &mode);
            SetConsoleMode(inputHandle, allowMouseCapture ? (mode | ENABLE_MOUSE_INPUT) & ~(ENABLE_QUICK_EDIT_MODE | ENABLE_PROCESSED_INPUT) : mode & ~ENABLE_PROCESSED_INPUT);
            while (true)
            {
                if (!wait.count())
                {
                    DWORD totalEventsAvailable;
                    GetNumberOfConsoleInputEvents(inputHandle, &totalEventsAvailable);
                    if (!totalEventsAvailable)
                    {
                        eventInfo = EventType::None;
                        break;
                    }
                }
                if (wait.count() > 0)
                {
                    if (!timerHandle)
                    {
                        timerHandle = CreateWaitableTimerW(nullptr, true, nullptr);
                        LARGE_INTEGER duration;
                        duration.QuadPart = -10000 * wait.count();
                        SetWaitableTimer(timerHandle, &duration, 1, nullptr, nullptr, false);
                    }
                    HANDLE handles[] = {timerHandle, inputHandle};
                    if (WaitForMultipleObjects(2, handles, false, INFINITE) == WAIT_OBJECT_0)
                    {
                        eventInfo = EventType::TimeOut;
                        break;
                    }
                }
                INPUT_RECORD record;
                DWORD totalEventsRead;
                ReadConsoleInputW(inputHandle, &record, 1, &totalEventsRead);
                if (record.EventType == FOCUS_EVENT)
                {
                    eventInfo = FocusEvent(record.Event.FocusEvent.bSetFocus);
                }
                else if (record.EventType == WINDOW_BUFFER_SIZE_EVENT)
                {
                    eventInfo = ResizeEvent();
                }
                else if (record.EventType == MOUSE_EVENT)
                {
                    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
                    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo) || GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE), &bufferInfo);
                    eventInfo = MouseEvent(
                        Coordinate(record.Event.MouseEvent.dwMousePosition.X - bufferInfo.srWindow.Left, record.Event.MouseEvent.dwMousePosition.Y - bufferInfo.srWindow.Top),
                        record.Event.MouseEvent.dwEventFlags & MOUSE_WHEELED ? record.Event.MouseEvent.dwButtonState & 0x1000000 ? MouseButton::WheelDown : MouseButton::WheelUp
                        : record.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED ? MouseButton::Left
                        : record.Event.MouseEvent.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED ? MouseButton::Wheel
                        : record.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED     ? MouseButton::Right
                                                                                               : MouseButton::None,
                        record.Event.MouseEvent.dwEventFlags & MOUSE_MOVED, record.Event.MouseEvent.dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED),
                        record.Event.MouseEvent.dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED), record.Event.MouseEvent.dwControlKeyState & SHIFT_PRESSED);
                }
                else if (record.EventType == KEY_EVENT)
                {
                    if (!record.Event.KeyEvent.bKeyDown || record.Event.KeyEvent.wVirtualKeyCode == VK_CONTROL || record.Event.KeyEvent.wVirtualKeyCode == VK_SHIFT ||
                        record.Event.KeyEvent.wVirtualKeyCode == VK_MENU || record.Event.KeyEvent.wVirtualKeyCode == VK_CAPITAL ||
                        record.Event.KeyEvent.wVirtualKeyCode == VK_NUMLOCK || record.Event.KeyEvent.wVirtualKeyCode == VK_SCROLL)
                    {
                        continue;
                    }
                    int key = 0;
                    int buffer;
                    if ((buffer = record.Event.KeyEvent.uChar.UnicodeChar))
                    {
                        if (buffer <= 26 && buffer != KeyboardKey::Tab && buffer != KeyboardKey::Enter)
                        {
                            key = buffer + 96;
                        }
                        else if (buffer >= HIGH_SURROGATE_START && buffer <= HIGH_SURROGATE_END)
                        {
                            ReadConsoleInputW(inputHandle, &record, 1, &totalEventsRead);
                            ReadConsoleInputW(inputHandle, &record, 1, &totalEventsRead);
                            *(reinterpret_cast<short*>(&buffer) + 1) = record.Event.KeyEvent.uChar.UnicodeChar;
                            WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<wchar_t*>(&buffer), 2, reinterpret_cast<char*>(&key), 4, nullptr, nullptr);
                        }
                        else
                        {
                            WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<wchar_t*>(&buffer), 1, reinterpret_cast<char*>(&key), 4, nullptr, nullptr);
                        }
                        goto l_keyReadingEnd;
                    }
                    PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_LEFT && record.Event.KeyEvent.wVirtualKeyCode <= VK_DOWN,
                              record.Event.KeyEvent.wVirtualKeyCode - VK_LEFT + static_cast<int>(KeyboardKey::LeftArrow));
                    PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_PRIOR && record.Event.KeyEvent.wVirtualKeyCode <= VK_HOME,
                              record.Event.KeyEvent.wVirtualKeyCode - VK_PRIOR + static_cast<int>(KeyboardKey::PageUp));
                    PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_INSERT && record.Event.KeyEvent.wVirtualKeyCode <= VK_DELETE,
                              record.Event.KeyEvent.wVirtualKeyCode - VK_INSERT + static_cast<int>(KeyboardKey::Insert));
                    PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_F1 && record.Event.KeyEvent.wVirtualKeyCode <= VK_F12,
                              record.Event.KeyEvent.wVirtualKeyCode - VK_F1 + static_cast<int>(KeyboardKey::F1));
                    continue;
                l_keyReadingEnd:
                    eventInfo = KeyEvent(key, record.Event.KeyEvent.dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED),
                                         record.Event.KeyEvent.dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED), record.Event.KeyEvent.dwControlKeyState & SHIFT_PRESSED);
                }
                if (eventInfo.getType() != EventType::None && eventInfo.getType() != EventType::TimeOut)
                {
                    if (filter && !filter(eventInfo))
                    {
                        continue;
                    }
                    break;
                }
            }
            if (timerHandle)
            {
                CloseHandle(timerHandle);
            }
            SetConsoleMode(inputHandle, mode);
#endif
            return eventInfo;
        }

    private:
        Setup() = delete;
    };

#ifdef _WIN32
    CMDArguments::CMDArguments(int totalArguments, wchar_t** utf16Arguments, char** utf8Arguments)
        : m_totalArguments(totalArguments), m_utf16Arguments(utf16Arguments), m_utf8Arguments(utf8Arguments)
    {
    }

    CMDArguments::~CMDArguments()
    {
        LocalFree(m_utf16Arguments);
        for (std::size_t offset = 0; offset < m_totalArguments; ++offset)
        {
            delete[] m_utf8Arguments[offset];
        }
        delete[] m_utf8Arguments;
    }

    std::wstring CMDArguments::GetUTF16ArgumentByOffset(std::size_t offset) const
    {
        if (offset >= m_totalArguments)
        {
            throw OutOfRangeException();
        }
        return m_utf16Arguments[offset];
    }
#else
    CMDArguments::CMDArguments(int totalArguments, char** utf8Arguments) : m_totalArguments(totalArguments), m_utf8Arguments(utf8Arguments)
    {
    }
#endif

    int CMDArguments::GetTotalArguments() const
    {
        return m_totalArguments;
    }

    std::string CMDArguments::GetUTF8ArgumentByOffset(std::size_t offset) const
    {
        if (offset >= m_totalArguments)
        {
            throw OutOfRangeException();
        }
        return m_utf8Arguments[offset];
    }

    Coordinate::Coordinate() : m_column(0), m_row(0)
    {
    }

    Coordinate::Coordinate(unsigned short column, unsigned short row) : m_column(column), m_row(row)
    {
    }

    unsigned short Coordinate::GetColumn() const
    {
        return m_column;
    }

    unsigned short Coordinate::GetRow() const
    {
        return m_row;
    }

    void Coordinate::SetColumn(unsigned short column)
    {
        m_column = column;
    }

    void Coordinate::SetRow(unsigned short row)
    {
        m_row = row;
    }

    std::string Coordinate::ToString() const
    {
        return "column: " + std::to_string(m_column) + ", row: " + std::to_string(m_row);
    }

    Region::Region()
        : m_totalColumns(0), m_totalRows(0), m_area(0), m_topLeftCoordinate(0, 0), m_topRightCoordinate(0, 0), m_bottomLeftCoordinate(0, 0), m_bottomRightCoordinate(0, 0)
    {
    }

    Region::Region(unsigned short totalColumns, unsigned short totalRows)
        : m_totalColumns(totalColumns), m_totalRows(totalRows), m_area(totalColumns * totalRows), m_topLeftCoordinate(0, 0), m_topRightCoordinate(totalColumns - 1, 0),
          m_bottomLeftCoordinate(0, totalRows - 1), m_bottomRightCoordinate(totalColumns - 1, totalRows - 1)
    {
    }

    Region::Region(Coordinate coordinateI, Coordinate coordinateII)
    {
        unsigned short maximumColumn = (std::max)(coordinateI.GetColumn(), coordinateII.GetColumn());
        unsigned short minimumColumn = (std::min)(coordinateI.GetColumn(), coordinateII.GetColumn());
        unsigned short maximumRow = (std::max)(coordinateI.GetRow(), coordinateII.GetRow());
        unsigned short minimumRow = (std::min)(coordinateI.GetRow(), coordinateII.GetRow());
        m_totalColumns = maximumColumn + 1;
        m_totalRows = maximumRow + 1;
        m_area = m_totalColumns * m_totalRows;
        m_topLeftCoordinate = Coordinate(minimumColumn, minimumRow);
        m_topRightCoordinate = Coordinate(maximumColumn, minimumRow);
        m_bottomLeftCoordinate = Coordinate(minimumColumn, maximumRow);
        m_bottomRightCoordinate = Coordinate(maximumColumn, maximumRow);
    }

    unsigned short Region::getTotalColumns() const
    {
        return m_totalColumns;
    }

    unsigned short Region::getTotalRows() const
    {
        return m_totalRows;
    }

    unsigned int Region::getArea() const
    {
        return m_area;
    }

    Coordinate Region::getTopLeftCoordinate() const
    {
        return m_topLeftCoordinate;
    }

    Coordinate Region::getTopRightCoordinate() const
    {
        return m_topRightCoordinate;
    }

    Coordinate Region::getBottomLeftCoordinate() const
    {
        return m_bottomLeftCoordinate;
    }

    Coordinate Region::getBottomRightCoordinate() const
    {
        return m_bottomRightCoordinate;
    }

    bool Region::contains(unsigned short column, unsigned short row) const
    {
        return column >= m_topLeftCoordinate.GetColumn() && column <= m_topRightCoordinate.GetColumn() && row >= m_topLeftCoordinate.GetRow() &&
               row <= m_bottomLeftCoordinate.GetRow();
    }

    bool Region::contains(Coordinate coordinate) const
    {
        return contains(coordinate.GetColumn(), coordinate.GetRow());
    }

    HexColor::HexColor(unsigned int code)
    {
        setCode(code);
    }

    HexColor::HexColor(RGBColor color) : m_code(color.getRed() << 16 | color.getGreen() << 8 | color.getBlue())
    {
    }

    unsigned int HexColor::getCode() const
    {
        return m_code;
    }

    void HexColor::setCode(unsigned int code)
    {
        if (code > 0xffffff)
        {
            throw OutOfRangeException();
        }
        m_code = code;
    }

    std::string HexColor::HexColor::ToString(bool hasPrefix, bool hasZeroPadding, bool isUpperCase) const
    {
        char buffer[7];
        std::snprintf(buffer, sizeof(buffer), hasZeroPadding ? isUpperCase ? "%06X" : "%06x" : isUpperCase ? "%X" : "%x", m_code);
        return hasPrefix ? std::string("0x") + buffer : std::string(buffer);
    }

    RGBColor::RGBColor() : m_red(0), m_green(0), m_blue(0)
    {
    }

    RGBColor::RGBColor(unsigned char red, unsigned char green, unsigned char blue) : m_red(red), m_green(green), m_blue(blue)
    {
    }

    RGBColor::RGBColor(HexColor color) : m_red(color.getCode() >> 16 & 0xff), m_green(color.getCode() >> 8 & 0xff), m_blue(color.getCode() & 0xff)
    {
    }

    unsigned char RGBColor::getRed() const
    {
        return m_red;
    }

    unsigned char RGBColor::getGreen() const
    {
        return m_green;
    }

    unsigned char RGBColor::getBlue() const
    {
        return m_blue;
    }

    void RGBColor::setRed(unsigned char red)
    {
        m_red = red;
    }

    void RGBColor::setGreen(unsigned char green)
    {
        m_green = green;
    }

    void RGBColor::setBlue(unsigned char blue)
    {
        m_blue = blue;
    }

    std::string RGBColor::ToString() const
    {
        return "r: " + std::to_string(m_red) + ", g: " + std::to_string(m_green) + ", b: " + std::to_string(m_blue);
    }

    FocusEvent::FocusEvent(bool hasFocus) : m_hasFocus(hasFocus)
    {
    }

    bool FocusEvent::hasFocus() const
    {
        return m_hasFocus;
    }

    ResizeEvent::ResizeEvent() : m_region(Terminal::Window::getRegion())
    {
    }

    Region ResizeEvent::getRegion() const
    {
        return m_region;
    }

    MouseEvent::MouseEvent(Coordinate coordinate, MouseButton button, bool isDragging, bool hasCtrl, bool hasAlt, bool hasShift)
        : m_coordinate(coordinate), m_button(button), m_isDragging(isDragging), m_hasCtrl(hasCtrl), m_hasAlt(hasAlt), m_hasShift(hasShift)
    {
    }

    Coordinate MouseEvent::getCoordinate() const
    {
        return m_coordinate;
    }

    MouseButton MouseEvent::getButton() const
    {
        return m_button;
    }

    bool MouseEvent::isDragging() const
    {
        return m_isDragging;
    }

    bool MouseEvent::hasCtrl() const
    {
        return m_hasCtrl;
    }

    bool MouseEvent::hasAlt() const
    {
        return m_hasAlt;
    }

    bool MouseEvent::hasShift() const
    {
        return m_hasShift;
    }

    KeyEvent::KeyEvent(int key, bool hasCtrl, bool hasAlt, bool hasShift) : m_key(key), m_hasCtrl(hasCtrl), m_hasAlt(hasAlt), m_hasShift(hasShift)
    {
    }

    int KeyEvent::getKey() const
    {
        return m_key;
    }

    bool KeyEvent::hasCtrl() const
    {
        return m_hasCtrl;
    }

    bool KeyEvent::hasAlt() const
    {
        return m_hasAlt;
    }

    bool KeyEvent::hasShift() const
    {
        return m_hasShift;
    }

    EventInfo::EventInfo(EventType type) : m_type(type)
    {
    }

    EventInfo::EventInfo(FocusEvent focusEvent) : m_type(EventType::Focus), m_focusEvent(focusEvent)
    {
    }

    EventInfo::EventInfo(ResizeEvent resizeEvent) : m_type(EventType::Resize), m_resizeEvent(resizeEvent)
    {
    }

    EventInfo::EventInfo(MouseEvent mouseEvent) : m_type(EventType::Mouse), m_mouseEvent(mouseEvent)
    {
    }

    EventInfo::EventInfo(KeyEvent keyEvent) : m_type(EventType::Key), m_keyEvent(keyEvent)
    {
    }

    EventType EventInfo::getType() const
    {
        return m_type;
    }

    FocusEvent EventInfo::getFocusEvent() const
    {
        if (m_type == EventType::Focus)
        {
            return m_focusEvent;
        }
        throw InvalidEventTypeException();
    }

    ResizeEvent EventInfo::getResizeEvent() const
    {
        if (m_type == EventType::Resize)
        {
            return m_resizeEvent;
        }
        throw InvalidEventTypeException();
    }

    MouseEvent EventInfo::getMouseEvent() const
    {
        if (m_type == EventType::Mouse)
        {
            return m_mouseEvent;
        }
        throw InvalidEventTypeException();
    }

    KeyEvent EventInfo::getKeyEvent() const
    {
        if (m_type == EventType::Key)
        {
            return m_keyEvent;
        }
        throw InvalidEventTypeException();
    }

#ifdef _WIN32
    std::string Terminal::Encoding::convertUTF16ToUTF8(std::wstring utf16String)
    {
        int utf8Size = WideCharToMultiByte(CP_UTF8, 0, utf16String.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::unique_ptr<char[]> utf8Buffer = std::make_unique<char[]>(utf8Size);
        WideCharToMultiByte(CP_UTF8, 0, utf16String.c_str(), -1, utf8Buffer.get(), utf8Size, nullptr, nullptr);
        return utf8Buffer.get();
    }

    std::wstring Terminal::Encoding::convertUTF8ToUTF16(std::string utf8String)
    {
        int utf16Size = MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, nullptr, 0);
        std::unique_ptr<WCHAR[]> utf16Buffer = std::make_unique<WCHAR[]>(utf16Size);
        MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, utf16Buffer.get(), utf16Size);
        return utf16Buffer.get();
    }
#endif

    void Terminal::Input::clear()
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

    bool Terminal::Input::isTTY()
    {
        Setup::initStreamTTYCache();
        return g_isInputStreamTTY;
    }

    EventInfo Terminal::Input::readEvent(bool allowMouseCapture)
    {
        return Setup::readEvent(allowMouseCapture, -1ms, nullptr);
    }

    EventInfo Terminal::Input::readEvent(bool allowMouseCapture, std::chrono::milliseconds wait)
    {
        return Setup::readEvent(allowMouseCapture, wait, nullptr);
    }

    EventInfo Terminal::Input::readEvent(bool allowMouseCapture, std::chrono::milliseconds wait, std::function<bool(EventInfo&)> filter)
    {
        return Setup::readEvent(allowMouseCapture, wait, filter);
    }

    void Terminal::Output::flush()
    {
        std::fflush(stdout);
    }

    void Terminal::Output::writeLine(std::string format, std::va_list arguments)
    {
        Setup::write(stdout, format.c_str(), arguments, true);
    }

    void Terminal::Output::writeLine(std::string format, ...)
    {
        std::va_list arguments;
        va_start(arguments, format);
        Setup::write(stdout, format.c_str(), arguments, true);
        va_end(arguments);
    }

    void Terminal::Output::writeLine()
    {
        Setup::write(stdout, nullptr, nullptr, true);
    }

    void Terminal::Output::write(std::string format, std::va_list arguments)
    {
        Setup::write(stdout, format.c_str(), arguments, false);
    }

    void Terminal::Output::write(std::string format, ...)
    {
        std::va_list arguments;
        va_start(arguments, format);
        Setup::write(stdout, format.c_str(), arguments, false);
        va_end(arguments);
    }

    bool Terminal::Output::isTTY()
    {
        Setup::initStreamTTYCache();
        return g_isOutputStreamTTY;
    }

    void Terminal::Error::writeLine(std::string format, std::va_list arguments)
    {
        Setup::write(stderr, format.c_str(), arguments, true);
    }

    void Terminal::Error::writeLine(std::string format, ...)
    {
        std::va_list arguments;
        va_start(arguments, format);
        Setup::write(stderr, format.c_str(), arguments, true);
        va_end(arguments);
    }

    void Terminal::Error::writeLine()
    {
        Setup::write(stderr, nullptr, nullptr, true);
    }

    void Terminal::Error::write(std::string format, std::va_list arguments)
    {
        Setup::write(stderr, format.c_str(), arguments, false);
    }

    void Terminal::Error::write(std::string format, ...)
    {
        std::va_list arguments;
        va_start(arguments, format);
        Setup::write(stderr, format.c_str(), arguments, false);
        va_end(arguments);
    }

    bool Terminal::Error::isTTY()
    {
        Setup::initStreamTTYCache();
        return g_isErrorStreamTTY;
    }

    CMDArguments Terminal::Process::getCMDArguments(int totalRawCMDArguments, char** rawCMDArguments)
    {
#ifdef _WIN32
        LPWSTR* utf16Arguments = CommandLineToArgvW(GetCommandLineW(), &totalRawCMDArguments);
        char** utf8Arguments = new char*[totalRawCMDArguments];
        for (std::size_t offset = 0; offset < totalRawCMDArguments; ++offset)
        {
            std::size_t size = WideCharToMultiByte(CP_UTF8, 0, utf16Arguments[offset], -1, nullptr, 0, nullptr, nullptr);
            utf8Arguments[offset] = new char[size];
            WideCharToMultiByte(CP_UTF8, 0, utf16Arguments[offset], -1, utf8Arguments[offset], size, nullptr, nullptr);
        }
        return CMDArguments(totalRawCMDArguments, utf16Arguments, utf8Arguments);
#else
        return CMDArguments(totalRawCMDArguments, rawCMDArguments);
#endif
    }

    void Terminal::Process::exit(int exitCode)
    {
        if (exitCode < 0 || exitCode > 255)
        {
            throw OutOfRangeException();
        }
        std::exit(exitCode);
    }

    void Terminal::Process::exit(ExitCode exitCode)
    {
        exit(static_cast<int>(exitCode));
    }

    Region Terminal::Window::getRegion()
    {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo) && !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE), &bufferInfo))
        {
            throw NoValidTTYException();
        }
        return Region(bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1, bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1);
#else
        struct winsize size;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) && ioctl(STDERR_FILENO, TIOCGWINSZ, &size))
        {
            throw NoValidTTYException();
        }
        return Region(size.ws_col, size.ws_row);
#endif
    }

    void Terminal::Window::openAlternateWindow()
    {
        try
        {
            Setup::writeANSIEscapeSequence("\x1b[?1049h\x1b[2J\x1b[1;1H");
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Window::closeAlternateWindow()
    {
        try
        {
            Setup::Setup::writeANSIEscapeSequence("\x1b[?1049l");
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Window::setTitle(std::string format, std::va_list arguments)
    {
        try
        {
            Setup::writeANSIEscapeSequence("\x1b]0;");
            Setup::writeANSIEscapeSequence(format, arguments);
            Setup::writeANSIEscapeSequence("\7");
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Window::setTitle(std::string format, ...)
    {
        std::va_list arguments;
        va_start(arguments, format);
        setTitle(format, arguments);
        va_end(arguments);
    }

    void Terminal::Bell::ring()
    {
        try
        {
            Setup::writeANSIEscapeSequence("\7");
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Font::setWeight(FontWeight weight)
    {
        try
        {
            Setup::writeANSIEscapeSequence("\x1b[22;%dm", static_cast<int>(weight));
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Font::setXColor(unsigned char color, FontLayer layer)
    {
        try
        {
            Setup::writeANSIEscapeSequence("\x1b[%d8;5;%hum", static_cast<int>(layer), color);
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Font::setXColor(XColor color, FontLayer layer)
    {
        try
        {
            Terminal::Font::setXColor(static_cast<unsigned char>(color), layer);
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Font::setRGBColor(unsigned char red, unsigned char green, unsigned char blue, FontLayer layer)
    {
        try
        {
            Setup::writeANSIEscapeSequence("\x1b[%d8;2;%hu;%hu;%hum", static_cast<int>(layer), red, green, blue);
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Font::setRGBColor(RGBColor color, FontLayer layer)
    {
        setRGBColor(color.getRed(), color.getGreen(), color.getBlue(), layer);
    }

    void Terminal::Font::setHexColor(unsigned int hex, FontLayer layer)
    {
        setRGBColor(RGBColor(HexColor(hex)), layer);
    }

    void Terminal::Font::setHexColor(HexColor color, FontLayer layer)
    {
        setRGBColor(RGBColor(color), layer);
    }

    void Terminal::Font::setEffects(int effects)
    {
        for (std::size_t offset = 6; offset < 32; ++offset)
        {
            if (effects & 1 << offset)
            {
                throw OutOfRangeException();
            }
            if (offset == 6)
            {
                offset = 9;
            }
        }
        for (std::size_t code = 3; code < 10; ++code)
        {
            if (effects & 1 << code)
            {
                try
                {
                    Setup::writeANSIEscapeSequence("\x1b[%dm", code);
                }
                catch (NoValidTTYException&)
                {
                }
            }
        }
    }

    void Terminal::Font::setEffects(FontEffect effect)
    {
        setEffects(static_cast<int>(effect));
    }

    void Terminal::Font::resetColors()
    {
        try
        {
            Setup::writeANSIEscapeSequence("\x1b[39;49m");
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Font::resetWeight()
    {
        try
        {
            Setup::writeANSIEscapeSequence("\x1b[22m");
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Font::resetEffects()
    {
        for (std::size_t code = 23; code < 30; ++code)
        {
            if (code != 26)
            {
                try
                {
                    Setup::writeANSIEscapeSequence("\x1b[%dm", code);
                }
                catch (NoValidTTYException&)
                {
                }
            }
        }
    }

    Coordinate Terminal::Cursor::getCoordinate()
    {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo) || GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE), &bufferInfo);
        return Coordinate(bufferInfo.dwCursorPosition.X - bufferInfo.srWindow.Left, bufferInfo.dwCursorPosition.Y - bufferInfo.srWindow.Top);
#else
        struct termios attributes;
        if (tcgetattr(STDIN_FILENO, &attributes))
        {
            throw NoValidTTYException();
        }
        InputStream::clear();
        Setup::writeANSIEscapeSequence("\x1b[6n");
        attributes.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
        unsigned short column;
        unsigned short row;
        int totalMatchesRead = std::scanf("\x1b[%hu;%huR", &row, &column);
        attributes.c_lflag |= ICANON | ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
        if (totalMatchesRead != 2)
        {
            throw WideCharacterOrientationException();
        }
        return Coordinate(--column, --row);
#endif
    }

    void Terminal::Cursor::setCoordinate(unsigned short column, unsigned short row)
    {
        try
        {
            if (!Window::getRegion().contains(column, row))
            {
                throw OutOfRangeException();
            }
            Setup::writeANSIEscapeSequence("\x1b[%hu;%huH", row + 1, column + 1);
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Cursor::setCoordinate(Coordinate coordinate)
    {
        setCoordinate(coordinate.GetColumn(), coordinate.GetRow());
    }

    void Terminal::Cursor::setShape(CursorShape shape, bool isBlinking)
    {
        try
        {
            Setup::writeANSIEscapeSequence("\x1b[%d q", static_cast<int>(shape) - isBlinking);
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Cursor::setVisibility(bool isVisible)
    {
        try
        {
            Setup::writeANSIEscapeSequence("\x1b[?25%c", isVisible ? 'h' : 'l');
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Cursor::resetShape()
    {
        try
        {
            Setup::writeANSIEscapeSequence("\x1b[0 q");
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Cursor::clearLine()
    {
        try
        {
            Setup::writeANSIEscapeSequence("\x1b[2K\x1b[1G");
        }
        catch (NoValidTTYException&)
        {
        }
    }

    int operator|(FontEffect effectI, FontEffect effectII)
    {
        return static_cast<int>(effectI) | static_cast<int>(effectII);
    }

    bool operator==(int code, KeyboardKey key)
    {
        return code == static_cast<int>(key);
    }
    bool operator==(KeyboardKey key, int code)
    {
        return static_cast<int>(key) == code;
    }

    bool operator!=(int code, KeyboardKey key)
    {
        return code != static_cast<int>(key);
    }

    bool operator!=(KeyboardKey key, int code)
    {
        return static_cast<int>(key) != code;
    }

    bool operator==(HexColor colorI, HexColor colorII)
    {
        return colorI.getCode() == colorII.getCode();
    }

    bool operator!=(HexColor colorI, HexColor colorII)
    {
        return colorI.getCode() != colorII.getCode();
    }

    bool operator==(RGBColor colorI, RGBColor colorII)
    {
        return colorI.getRed() == colorII.getRed() && colorI.getGreen() == colorII.getGreen() && colorI.getBlue() == colorII.getBlue();
    }

    bool operator!=(RGBColor colorI, RGBColor colorII)
    {
        return colorI.getRed() != colorII.getRed() && colorI.getGreen() != colorII.getGreen() && colorI.getBlue() != colorII.getBlue();
    }

    bool operator==(HexColor hexColor, RGBColor rgbColor)
    {
        return hexColor == HexColor(rgbColor);
    }

    bool operator!=(HexColor hexColor, RGBColor rgbColor)
    {
        return hexColor != HexColor(rgbColor);
    }

    bool operator==(RGBColor rgbColor, HexColor hexColor)
    {
        return rgbColor == RGBColor(hexColor);
    }

    bool operator!=(RGBColor rgbColor, HexColor hexColor)
    {
        return rgbColor != RGBColor(hexColor);
    }
}
