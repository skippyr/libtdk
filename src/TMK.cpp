#include "TMK.hpp"

#ifdef _WIN32
#include <io.h>
#else
#include <sys/ioctl.h>
#endif

#ifdef _WIN32
#define TTY_CACHE(a_stream) (!!_isatty(a_stream::GetFileNumber()) << a_stream::GetFileNumber())
#else
#define TTY_CACHE(a_stream) (isatty(a_stream::GetFileNumber()) << a_stream::GetFileNumber())
#endif
#define IS_TTY(a_streamFileNumber) (g_ttyCache & 1 << a_streamFileNumber)
#define TTY_CACHE_HAS_BEEN_FILLED_FLAG (1 << 7)
#define PARSE_KEY(a_condition, a_key)                                                                                                                                              \
    if (a_condition)                                                                                                                                                               \
    {                                                                                                                                                                              \
        key = a_key;                                                                                                                                                               \
        goto l_keyReadingEnd;                                                                                                                                                      \
    }

namespace TMK
{
    char g_ttyCache = 0;

    class Setup
    {
    public:
#ifdef _WIN32
        static DWORD GetStreamMode(HANDLE handle)
        {
            DWORD mode;
            if (!GetConsoleMode(handle, &mode))
            {
                throw NoValidTTYException();
            }
            return mode;
        }

        static CONSOLE_SCREEN_BUFFER_INFO GetStreamScreenBufferInfo(HANDLE handle)
        {
            CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
            if (!GetConsoleScreenBufferInfo(handle, &bufferInfo))
            {
                throw NoValidTTYException();
            }
            return bufferInfo;
        }

        static void SetStreamMode(HANDLE handle, bool isTTY, DWORD mode)
        {
            if (!isTTY)
            {
                throw NoValidTTYException();
            }
            if (!SetConsoleMode(handle, mode))
            {
                throw InvalidStreamAttributesException();
            }
        }
#endif

        static void InitEnvironment()
        {
            if (!(g_ttyCache & TTY_CACHE_HAS_BEEN_FILLED_FLAG))
            {
                g_ttyCache |= TTY_CACHE(Terminal::Input) | TTY_CACHE(Terminal::Output) | TTY_CACHE(Terminal::Error) | TTY_CACHE_HAS_BEEN_FILLED_FLAG;
#ifdef _WIN32
                Terminal::Encoding::SetOutputCodePage(CP_UTF8);
                try
                {
                    Terminal::Output::SetMode(Terminal::Output::GetMode() | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
                }
                catch (NoValidTTYException&)
                {
                }
                try
                {
                    Terminal::Error::SetMode(Terminal::Error::GetMode() | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
                }
                catch (NoValidTTYException&)
                {
                }
#endif
            }
        }

        /**
         * @brief Writes an ANSI escape sequence to the standard output or error streams.
         * @param format The format to be used. It accepts the same specifiers as the printf function family.
         * @param arguments The arguments to be formatted.
         * @throws NoValidTTYException Thrown whenever the standard output and error streams are not TTY.
         * @throws WideCharacterOrientationException Thrown whenever the standard output or error stream is wide character oriented.
         */
        static void WriteANSIEscapeSequence(std::string format, std::va_list arguments)
        {
            if (!Terminal::Output::IsTTY() && !Terminal::Error::IsTTY())
            {
                throw NoValidTTYException();
            }
            if (std::vfprintf(Terminal::Output::IsTTY() ? Terminal::Output::GetFile() : Terminal::Error::GetFile(), format.c_str(), arguments) < 0)
            {
                throw WideCharacterOrientationException();
            }
        }

        /**
         * @brief Writes an ANSI escape sequence to the standard output or error streams.
         * @param format The format to be used. It accepts the same specifiers as the printf function family.
         * @param ... The arguments to be formatted.
         * @throws NoValidTTYException Thrown whenever the standard output and error streams are not TTY.
         * @throws WideCharacterOrientationException Thrown whenever the standard output or error stream is wide character oriented.
         */
        static void WriteANSIEscapeSequence(std::string format, ...)
        {
            std::va_list arguments;
            va_start(arguments, format);
            WriteANSIEscapeSequence(format, arguments);
            va_end(arguments);
        }

        static void Write(std::FILE* streamFile, const char* format, std::va_list arguments, bool hasNewLine)
        {
            if (streamFile == Terminal::Error::GetFile())
            {
                Terminal::Output::Flush();
            }
            if (format && std::vfprintf(streamFile, format, arguments) < 0)
            {
                throw WideCharacterOrientationException();
            }
            if (hasNewLine && std::fputc('\n', streamFile) == EOF)
            {
                throw WideCharacterOrientationException();
            }
        }

        static EventInfo ReadEvent(bool allowMouseCapture, std::chrono::milliseconds wait, std::function<bool(EventInfo&)> filter)
        {
            if (!Terminal::Input::IsTTY() || (!Terminal::Output::IsTTY() && !Terminal::Error::IsTTY()))
            {
                throw NoValidTTYException();
            }
            if (std::fwide(Terminal::Input::GetFile(), 0) > 0)
            {
                throw WideCharacterOrientationException();
            }
            EventInfo eventInfo = EventType::None;
#ifdef _WIN32
            HANDLE timerHandle = nullptr;
            DWORD inputMode = Terminal::Input::GetMode();
            Terminal::Input::SetMode(allowMouseCapture ? (inputMode | ENABLE_MOUSE_INPUT) & ~(ENABLE_QUICK_EDIT_MODE | ENABLE_PROCESSED_INPUT)
                                                       : inputMode & ~ENABLE_PROCESSED_INPUT);
            while (true)
            {
                if (!wait.count())
                {
                    if (!Terminal::Input::GetTotalEventsCached())
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
                    HANDLE handles[] = {timerHandle, Terminal::Input::GetHandle()};
                    if (WaitForMultipleObjects(2, handles, false, INFINITE) == WAIT_OBJECT_0)
                    {
                        eventInfo = EventType::TimeOut;
                        break;
                    }
                }
                INPUT_RECORD record;
                DWORD totalEventsRead;
                ReadConsoleInputW(Terminal::Input::GetHandle(), &record, 1, &totalEventsRead);
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
                    try
                    {
                        bufferInfo = Terminal::Output::GetScreenBufferInfo();
                    }
                    catch (NoValidTTYException&)
                    {
                        bufferInfo = Terminal::Error::GetScreenBufferInfo();
                    }
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
                            ReadConsoleInputW(Terminal::Input::GetHandle(), &record, 1, &totalEventsRead);
                            ReadConsoleInputW(Terminal::Input::GetHandle(), &record, 1, &totalEventsRead);
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
                if (eventInfo.GetType() != EventType::None && eventInfo.GetType() != EventType::TimeOut)
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
            Terminal::Input::SetMode(inputMode);
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

    Geometry::Geometry()
        : m_totalColumns(0), m_totalRows(0), m_area(0), m_topLeftCoordinate(0, 0), m_topRightCoordinate(0, 0), m_bottomLeftCoordinate(0, 0), m_bottomRightCoordinate(0, 0)
    {
    }

    Geometry::Geometry(unsigned short totalColumns, unsigned short totalRows)
        : m_totalColumns(totalColumns), m_totalRows(totalRows), m_area(totalColumns * totalRows), m_topLeftCoordinate(0, 0), m_topRightCoordinate(totalColumns - 1, 0),
          m_bottomLeftCoordinate(0, totalRows - 1), m_bottomRightCoordinate(totalColumns - 1, totalRows - 1)
    {
    }

    Geometry::Geometry(Coordinate coordinateI, Coordinate coordinateII)
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

    unsigned short Geometry::GetTotalColumns() const
    {
        return m_totalColumns;
    }

    unsigned short Geometry::GetTotalRows() const
    {
        return m_totalRows;
    }

    unsigned int Geometry::GetArea() const
    {
        return m_area;
    }

    Coordinate Geometry::GetTopLeftCoordinate() const
    {
        return m_topLeftCoordinate;
    }

    Coordinate Geometry::GetTopRightCoordinate() const
    {
        return m_topRightCoordinate;
    }

    Coordinate Geometry::GetBottomLeftCoordinate() const
    {
        return m_bottomLeftCoordinate;
    }

    Coordinate Geometry::GetBottomRightCoordinate() const
    {
        return m_bottomRightCoordinate;
    }

    bool Geometry::Contains(unsigned short column, unsigned short row) const
    {
        return column >= m_topLeftCoordinate.GetColumn() && column <= m_topRightCoordinate.GetColumn() && row >= m_topLeftCoordinate.GetRow() &&
               row <= m_bottomLeftCoordinate.GetRow();
    }

    bool Geometry::Contains(Coordinate coordinate) const
    {
        return Contains(coordinate.GetColumn(), coordinate.GetRow());
    }

    HexColor::HexColor(unsigned int code)
    {
        SetCode(code);
    }

    HexColor::HexColor(RGBColor color) : m_code(color.GetRed() << 16 | color.GetGreen() << 8 | color.GetBlue())
    {
    }

    unsigned int HexColor::GetCode() const
    {
        return m_code;
    }

    void HexColor::SetCode(unsigned int code)
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

    RGBColor::RGBColor(unsigned char red, unsigned char green, unsigned char blue) : m_red(red), m_green(green), m_blue(blue)
    {
    }

    RGBColor::RGBColor(HexColor color) : m_red(color.GetCode() >> 16 & 0xff), m_green(color.GetCode() >> 8 & 0xff), m_blue(color.GetCode() & 0xff)
    {
    }

    RGBColor::RGBColor(HSLColor color)
    {
    }

    unsigned char RGBColor::GetRed() const
    {
        return m_red;
    }

    unsigned char RGBColor::GetGreen() const
    {
        return m_green;
    }

    unsigned char RGBColor::GetBlue() const
    {
        return m_blue;
    }

    void RGBColor::SetRed(unsigned char red)
    {
        m_red = red;
    }

    void RGBColor::SetGreen(unsigned char green)
    {
        m_green = green;
    }

    void RGBColor::SetBlue(unsigned char blue)
    {
        m_blue = blue;
    }

    std::string RGBColor::ToString() const
    {
        return "r: " + std::to_string(m_red) + ", g: " + std::to_string(m_green) + ", b: " + std::to_string(m_blue);
    }

    HSLColor::HSLColor(unsigned short hue, unsigned char saturation, unsigned char lightness)
    {
        SetHue(hue);
        SetSaturation(saturation);
        SetLightness(lightness);
    }

    unsigned short HSLColor::GetHue() const
    {
        return m_hue;
    }

    unsigned char HSLColor::GetSaturation() const
    {
        return m_saturation;
    }

    unsigned char HSLColor::GetLightness() const
    {
        return m_lightness;
    }

    void HSLColor::SetHue(unsigned short hue)
    {
        if (hue > 360)
        {
            throw OutOfRangeException();
        }
        m_hue = hue;
    }

    void HSLColor::SetSaturation(unsigned char saturation)
    {
        if (saturation > 100)
        {
            throw OutOfRangeException();
        }
        m_saturation = saturation;
    }

    void HSLColor::SetLightness(unsigned char lightness)
    {
        if (lightness > 100)
        {
            throw OutOfRangeException();
        }
        m_lightness = lightness;
    }

    std::string HSLColor::ToString() const
    {
        return "h: " + std::to_string(m_hue) + ", s: " + std::to_string(m_saturation) + ", l: " + std::to_string(m_saturation);
    }

    FocusEvent::FocusEvent(bool hasFocus) : m_hasFocus(hasFocus)
    {
    }

    bool FocusEvent::HasFocus() const
    {
        return m_hasFocus;
    }

    ResizeEvent::ResizeEvent() : m_windowGeometry(Terminal::Window::GetGeometry())
    {
    }

    Geometry ResizeEvent::GetWindowGeometry() const
    {
        return m_windowGeometry;
    }

    MouseEvent::MouseEvent(Coordinate coordinate, MouseButton button, bool isDragging, bool hasCtrl, bool hasAlt, bool hasShift)
        : m_coordinate(coordinate), m_button(button), m_isDragging(isDragging), m_hasCtrl(hasCtrl), m_hasAlt(hasAlt), m_hasShift(hasShift)
    {
    }

    Coordinate MouseEvent::GetCoordinate() const
    {
        return m_coordinate;
    }

    MouseButton MouseEvent::GetButton() const
    {
        return m_button;
    }

    bool MouseEvent::IsDragging() const
    {
        return m_isDragging;
    }

    bool MouseEvent::HasCtrl() const
    {
        return m_hasCtrl;
    }

    bool MouseEvent::HasAlt() const
    {
        return m_hasAlt;
    }

    bool MouseEvent::HasShift() const
    {
        return m_hasShift;
    }

    KeyEvent::KeyEvent(int key, bool hasCtrl, bool hasAlt, bool hasShift) : m_key(key), m_hasCtrl(hasCtrl), m_hasAlt(hasAlt), m_hasShift(hasShift)
    {
    }

    int KeyEvent::GetKey() const
    {
        return m_key;
    }

    bool KeyEvent::HasCtrl() const
    {
        return m_hasCtrl;
    }

    bool KeyEvent::HasAlt() const
    {
        return m_hasAlt;
    }

    bool KeyEvent::HasShift() const
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

    FocusEvent EventInfo::GetFocusEvent() const
    {
        if (m_type == EventType::Focus)
        {
            return m_focusEvent;
        }
        throw InvalidEventTypeException();
    }

    ResizeEvent EventInfo::GetResizeEvent() const
    {
        if (m_type == EventType::Resize)
        {
            return m_resizeEvent;
        }
        throw InvalidEventTypeException();
    }

    MouseEvent EventInfo::GetMouseEvent() const
    {
        if (m_type == EventType::Mouse)
        {
            return m_mouseEvent;
        }
        throw InvalidEventTypeException();
    }

    KeyEvent EventInfo::GetKeyEvent() const
    {
        if (m_type == EventType::Key)
        {
            return m_keyEvent;
        }
        throw InvalidEventTypeException();
    }

    EventType EventInfo::GetType() const
    {
        return m_type;
    }

#ifdef _WIN32
    void Terminal::Encoding::SetOutputCodePage(UINT codePage)
    {
        if (!SetConsoleOutputCP(codePage))
        {
            throw InvalidCodePageException();
        }
    }

    std::string Terminal::Encoding::ConvertUTF16ToUTF8(std::wstring utf16String)
    {
        int utf8Size = WideCharToMultiByte(CP_UTF8, 0, utf16String.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::unique_ptr<char[]> utf8Buffer = std::make_unique<char[]>(utf8Size);
        WideCharToMultiByte(CP_UTF8, 0, utf16String.c_str(), -1, utf8Buffer.get(), utf8Size, nullptr, nullptr);
        return utf8Buffer.get();
    }

    std::wstring Terminal::Encoding::ConvertUTF8ToUTF16(std::string utf8String)
    {
        int utf16Size = MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, nullptr, 0);
        std::unique_ptr<WCHAR[]> utf16Buffer = std::make_unique<WCHAR[]>(utf16Size);
        MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, utf16Buffer.get(), utf16Size);
        return utf16Buffer.get();
    }
#endif

#ifdef _WIN32
    HANDLE Terminal::Input::GetHandle()
    {
        return GetStdHandle(STD_INPUT_HANDLE);
    }

    DWORD Terminal::Input::GetMode()
    {
        return Setup::GetStreamMode(GetHandle());
    }

    DWORD Terminal::Input::GetTotalEventsCached()
    {
        DWORD totalEvents;
        if (!GetNumberOfConsoleInputEvents(GetHandle(), &totalEvents))
        {
            throw NoValidTTYException();
        }
        return totalEvents;
    }

    void Terminal::Input::SetMode(DWORD mode)
    {
        Setup::SetStreamMode(GetHandle(), IsTTY(), mode);
    }
#else
    struct termios Terminal::Input::GetTermiosAttributes()
    {
        struct termios attributes;
        if (tcgetattr(GetFileNumber(), &attributes))
        {
            throw NoValidTTYException();
        }
        return attributes;
    }

    void Terminal::Input::SetTermiosAttributes(struct termios& attributes)
    {
        if (!IsTTY())
        {
            throw NoValidTTYException();
        }
        if (tcsetattr(GetFileNumber(), TCSANOW, &attributes))
        {
            throw InvalidStreamAttributesException();
        }
    }

    void Terminal::Input::SetFCNTLBlockingState(bool isToEnable)
    {
        int flags = fcntl(GetFileNumber(), F_GETFL);
        fcntl(GetFileNumber(), F_SETFL, isToEnable ? flags &~ O_NONBLOCK : flags | O_NONBLOCK);
    }
#endif

    std::FILE* Terminal::Input::GetFile()
    {
        return stdin;
    }

    int Terminal::Input::GetFileNumber()
    {
        return 0;
    }

    void Terminal::Input::Clear()
    {
#ifdef _WIN32
        FlushConsoleInputBuffer(GetHandle());
#else
        struct termios attributes;
        try
        {
            attributes = GetTermiosAttributes();
        }
        catch (NoValidTTYException&)
        {
            return;
        }
        attributes.c_lflag &= ~(ICANON | ECHO);
        SetTermiosAttributes(attributes);
        SetFCNTLBlockingState(false);
        while (ReadByte() != EOF)
        {
        }
        attributes.c_lflag |= ICANON | ECHO;
        SetTermiosAttributes(attributes);
        SetFCNTLBlockingState(true);
#endif
    }

    bool Terminal::Input::IsTTY()
    {
        Setup::InitEnvironment();
        return IS_TTY(GetFileNumber());
    }

    char Terminal::Input::ReadByte()
    {
        return std::getchar();
    }

    EventInfo Terminal::Input::ReadEvent(bool allowMouseCapture)
    {
        return Setup::ReadEvent(allowMouseCapture, -1ms, nullptr);
    }

    EventInfo Terminal::Input::ReadEvent(bool allowMouseCapture, std::chrono::milliseconds wait)
    {
        return Setup::ReadEvent(allowMouseCapture, wait, nullptr);
    }

    EventInfo Terminal::Input::ReadEvent(bool allowMouseCapture, std::chrono::milliseconds wait, std::function<bool(EventInfo&)> filter)
    {
        return Setup::ReadEvent(allowMouseCapture, wait, filter);
    }

#ifdef _WIN32
    HANDLE Terminal::Output::GetHandle()
    {
        return GetStdHandle(STD_OUTPUT_HANDLE);
    }

    DWORD Terminal::Output::GetMode()
    {
        return Setup::GetStreamMode(GetHandle());
    }

    CONSOLE_SCREEN_BUFFER_INFO Terminal::Output::GetScreenBufferInfo()
    {
        return Setup::GetStreamScreenBufferInfo(GetHandle());
    }

    void Terminal::Output::SetMode(DWORD mode)
    {
        Setup::SetStreamMode(GetHandle(), IsTTY(), mode);
    }
#endif

    void Terminal::Output::Flush()
    {
        std::fflush(GetFile());
    }

    void Terminal::Output::WriteLine(std::string format, std::va_list arguments)
    {
        Setup::Write(GetFile(), format.c_str(), arguments, true);
    }

    void Terminal::Output::WriteLine(std::string format, ...)
    {
        std::va_list arguments;
        va_start(arguments, format);
        Setup::Write(GetFile(), format.c_str(), arguments, true);
        va_end(arguments);
    }

    void Terminal::Output::WriteLine()
    {
        Setup::Write(GetFile(), nullptr, nullptr, true);
    }

    void Terminal::Output::Write(std::string format, std::va_list arguments)
    {
        Setup::Write(GetFile(), format.c_str(), arguments, false);
    }

    void Terminal::Output::Write(std::string format, ...)
    {
        std::va_list arguments;
        va_start(arguments, format);
        Setup::Write(GetFile(), format.c_str(), arguments, false);
        va_end(arguments);
    }

    std::FILE* Terminal::Output::GetFile()
    {
        return stdout;
    }

    int Terminal::Output::GetFileNumber()
    {
        return 1;
    }

    bool Terminal::Output::IsTTY()
    {
        Setup::InitEnvironment();
        return IS_TTY(GetFileNumber());
    }

#ifdef _WIN32
    HANDLE Terminal::Error::GetHandle()
    {
        return GetStdHandle(STD_ERROR_HANDLE);
    }

    DWORD Terminal::Error::GetMode()
    {
        return Setup::GetStreamMode(GetHandle());
    }

    CONSOLE_SCREEN_BUFFER_INFO Terminal::Error::GetScreenBufferInfo()
    {
        return Setup::GetStreamScreenBufferInfo(GetHandle());
    }

    void Terminal::Error::SetMode(DWORD mode)
    {
        Setup::SetStreamMode(GetHandle(), IsTTY(), mode);
    }
#endif

    void Terminal::Error::WriteLine(std::string format, std::va_list arguments)
    {
        Setup::Write(GetFile(), format.c_str(), arguments, true);
    }

    void Terminal::Error::WriteLine(std::string format, ...)
    {
        std::va_list arguments;
        va_start(arguments, format);
        Setup::Write(GetFile(), format.c_str(), arguments, true);
        va_end(arguments);
    }

    void Terminal::Error::WriteLine()
    {
        Setup::Write(GetFile(), nullptr, nullptr, true);
    }

    void Terminal::Error::Write(std::string format, std::va_list arguments)
    {
        Setup::Write(GetFile(), format.c_str(), arguments, false);
    }

    void Terminal::Error::Write(std::string format, ...)
    {
        std::va_list arguments;
        va_start(arguments, format);
        Setup::Write(GetFile(), format.c_str(), arguments, false);
        va_end(arguments);
    }

    std::FILE* Terminal::Error::GetFile()
    {
        return stderr;
    }

    int Terminal::Error::GetFileNumber()
    {
        return 2;
    }

    bool Terminal::Error::IsTTY()
    {
        Setup::InitEnvironment();
        return IS_TTY(GetFileNumber());
    }

    CMDArguments Terminal::Process::GetCMDArguments(int rawTotalCMDArguments, char** rawCMDArguments)
    {
#ifdef _WIN32
        LPWSTR* utf16Arguments = CommandLineToArgvW(GetCommandLineW(), &rawTotalCMDArguments);
        char** utf8Arguments = new char*[rawTotalCMDArguments];
        for (std::size_t offset = 0; offset < rawTotalCMDArguments; ++offset)
        {
            std::size_t size = WideCharToMultiByte(CP_UTF8, 0, utf16Arguments[offset], -1, nullptr, 0, nullptr, nullptr);
            utf8Arguments[offset] = new char[size];
            WideCharToMultiByte(CP_UTF8, 0, utf16Arguments[offset], -1, utf8Arguments[offset], size, nullptr, nullptr);
        }
        return CMDArguments(rawTotalCMDArguments, utf16Arguments, utf8Arguments);
#else
        return CMDArguments(rawTotalCMDArguments, rawCMDArguments);
#endif
    }

    void Terminal::Process::Exit(int exitCode)
    {
        if (exitCode < 0 || exitCode > 255)
        {
            throw OutOfRangeException();
        }
        std::exit(exitCode);
    }

    void Terminal::Process::Exit(POSIXExitCode exitCode)
    {
        Exit(static_cast<int>(exitCode));
    }

    Geometry Terminal::Window::GetGeometry()
    {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        if (!GetConsoleScreenBufferInfo(Output::GetHandle(), &bufferInfo) && !GetConsoleScreenBufferInfo(Error::GetHandle(), &bufferInfo))
        {
            throw NoValidTTYException();
        }
        return Geometry(bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1, bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1);
#else
        struct winsize size;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) && ioctl(STDERR_FILENO, TIOCGWINSZ, &size))
        {
            throw NoValidTTYException();
        }
        return Geometry(size.ws_col, size.ws_row);
#endif
    }

    void Terminal::Window::OpenAlternateWindow()
    {
        try
        {
            Setup::WriteANSIEscapeSequence("\x1b[?1049h\x1b[2J\x1b[1;1H");
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Window::CloseAlternateWindow()
    {
        try
        {
            Setup::WriteANSIEscapeSequence("\x1b[?1049l");
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Window::SetTitle(std::string format, std::va_list arguments)
    {
        try
        {
            Setup::WriteANSIEscapeSequence("\x1b]0;");
            Setup::WriteANSIEscapeSequence(format, arguments);
            Setup::WriteANSIEscapeSequence("\7");
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Window::SetTitle(std::string format, ...)
    {
        std::va_list arguments;
        va_start(arguments, format);
        SetTitle(format, arguments);
        va_end(arguments);
    }

    void Terminal::Bell::Ring()
    {
        try
        {
            Setup::WriteANSIEscapeSequence("\7");
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Font::SetWeight(FontWeight weight)
    {
        try
        {
            Setup::WriteANSIEscapeSequence("\x1b[22;%dm", static_cast<int>(weight));
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Font::SetXColor(unsigned char color, FontLayer layer)
    {
        try
        {
            Setup::WriteANSIEscapeSequence("\x1b[%d8;5;%hum", static_cast<int>(layer), color);
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Font::SetXColor(XColor color, FontLayer layer)
    {
        try
        {
            Terminal::Font::SetXColor(static_cast<unsigned char>(color), layer);
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Font::SetRGBColor(unsigned char red, unsigned char green, unsigned char blue, FontLayer layer)
    {
        try
        {
            Setup::WriteANSIEscapeSequence("\x1b[%d8;2;%hu;%hu;%hum", static_cast<int>(layer), red, green, blue);
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Font::SetRGBColor(RGBColor color, FontLayer layer)
    {
        SetRGBColor(color.GetRed(), color.GetGreen(), color.GetBlue(), layer);
    }

    void Terminal::Font::SetHexColor(unsigned int hex, FontLayer layer)
    {
        SetRGBColor(RGBColor(HexColor(hex)), layer);
    }

    void Terminal::Font::SetHexColor(HexColor color, FontLayer layer)
    {
        SetRGBColor(RGBColor(color), layer);
    }

    void Terminal::Font::ResetColors()
    {
        try
        {
            Setup::WriteANSIEscapeSequence("\x1b[39;49m");
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Font::ResetWeight()
    {
        try
        {
            Setup::WriteANSIEscapeSequence("\x1b[22m");
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Font::SetEffects(int effects)
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
                    Setup::WriteANSIEscapeSequence("\x1b[%dm", code);
                }
                catch (NoValidTTYException&)
                {
                }
            }
        }
    }

    void Terminal::Font::SetEffects(FontEffect effect)
    {
        SetEffects(static_cast<int>(effect));
    }

    void Terminal::Font::ResetEffects()
    {
        for (std::size_t code = 23; code < 30; ++code)
        {
            if (code != 26)
            {
                try
                {
                    Setup::WriteANSIEscapeSequence("\x1b[%dm", code);
                }
                catch (NoValidTTYException&)
                {
                }
            }
        }
    }

    Coordinate Terminal::Cursor::GetCoordinate()
    {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        try
        {
            bufferInfo = Output::GetScreenBufferInfo();
        }
        catch (NoValidTTYException&)
        {
            bufferInfo = Error::GetScreenBufferInfo();
        }
        return Coordinate(bufferInfo.dwCursorPosition.X - bufferInfo.srWindow.Left, bufferInfo.dwCursorPosition.Y - bufferInfo.srWindow.Top);
#else
        struct termios attributes = Input::GetTermiosAttributes();
        Input::Clear();
        Setup::WriteANSIEscapeSequence("\x1b[6n");
        attributes.c_lflag &= ~(ICANON | ECHO);
        Input::SetTermiosAttributes(attributes);
        unsigned short column;
        unsigned short row;
        int totalMatchesRead = std::scanf("\x1b[%hu;%huR", &row, &column);
        attributes.c_lflag |= ICANON | ECHO;
        Input::SetTermiosAttributes(attributes);
        if (totalMatchesRead != 2)
        {
            throw WideCharacterOrientationException();
        }
        return Coordinate(--column, --row);
#endif
    }

    void Terminal::Cursor::SetCoordinate(unsigned short column, unsigned short row)
    {
        try
        {
            if (!Window::GetGeometry().Contains(column, row))
            {
                throw OutOfRangeException();
            }
            Setup::WriteANSIEscapeSequence("\x1b[%hu;%huH", row + 1, column + 1);
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Cursor::SetCoordinate(Coordinate coordinate)
    {
        SetCoordinate(coordinate.GetColumn(), coordinate.GetRow());
    }

    void Terminal::Cursor::SetShape(CursorShape shape, bool isBlinking)
    {
        try
        {
            Setup::WriteANSIEscapeSequence("\x1b[%d q", static_cast<int>(shape) - isBlinking);
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Cursor::SetVisibility(bool isVisible)
    {
        try
        {
            Setup::WriteANSIEscapeSequence("\x1b[?25%c", isVisible ? 'h' : 'l');
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Cursor::ResetShape()
    {
        try
        {
            Setup::WriteANSIEscapeSequence("\x1b[0 q");
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Cursor::ClearLine()
    {
        try
        {
            Setup::WriteANSIEscapeSequence("\x1b[2K\x1b[1G");
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
        return colorI.GetCode() == colorII.GetCode();
    }

    bool operator!=(HexColor colorI, HexColor colorII)
    {
        return colorI.GetCode() != colorII.GetCode();
    }

    bool operator==(RGBColor colorI, RGBColor colorII)
    {
        return colorI.GetRed() == colorII.GetRed() && colorI.GetGreen() == colorII.GetGreen() && colorI.GetBlue() == colorII.GetBlue();
    }

    bool operator!=(RGBColor colorI, RGBColor colorII)
    {
        return colorI.GetRed() != colorII.GetRed() && colorI.GetGreen() != colorII.GetGreen() && colorI.GetBlue() != colorII.GetBlue();
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
