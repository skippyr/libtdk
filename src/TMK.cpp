#include "TMK.hpp"

#ifdef _WIN32
#include <io.h>
#else
#include <sys/ioctl.h>
#endif

#ifdef _WIN32
#define IS_TTY(a_stream) _isatty(a_stream::GetFileNumber())
#else
#define IS_TTY(a_stream) isatty(a_stream::GetFileNumber())
#endif
#define PARSE_KEY(a_condition, a_key)                                                                                                                                              \
    if (a_condition)                                                                                                                                                               \
    {                                                                                                                                                                              \
        key = a_key;                                                                                                                                                               \
        goto l_keyReadingEnd;                                                                                                                                                      \
    }

namespace TMK
{
    bool g_hasStreamTTYCache = false;
    bool g_isInputStreamTTY = false;
    bool g_isOutputStreamTTY = false;
    bool g_isErrorStreamTTY = false;

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

    unsigned short Region::GetTotalColumns() const
    {
        return m_totalColumns;
    }

    unsigned short Region::GetTotalRows() const
    {
        return m_totalRows;
    }

    unsigned int Region::GetArea() const
    {
        return m_area;
    }

    Coordinate Region::GetTopLeftCoordinate() const
    {
        return m_topLeftCoordinate;
    }

    Coordinate Region::GetTopRightCoordinate() const
    {
        return m_topRightCoordinate;
    }

    Coordinate Region::GetBottomLeftCoordinate() const
    {
        return m_bottomLeftCoordinate;
    }

    Coordinate Region::GetBottomRightCoordinate() const
    {
        return m_bottomRightCoordinate;
    }

    bool Region::Contains(unsigned short column, unsigned short row) const
    {
        return column >= m_topLeftCoordinate.GetColumn() && column <= m_topRightCoordinate.GetColumn() && row >= m_topLeftCoordinate.GetRow() &&
               row <= m_bottomLeftCoordinate.GetRow();
    }

    bool Region::Contains(Coordinate coordinate) const
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

    ResizeEvent::ResizeEvent() : m_windowRegion(Terminal::Window::GetRegion())
    {
    }

    Region ResizeEvent::GetWindowRegion() const
    {
        return m_windowRegion;
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

    EventType EventInfo::GetType() const
    {
        return m_type;
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

    void Terminal::InitStreamTTYCache()
    {
        if (g_hasStreamTTYCache)
        {
            return;
        }
        g_hasStreamTTYCache = true;
        g_isInputStreamTTY = IS_TTY(InputStream);
        g_isOutputStreamTTY = IS_TTY(OutputStream);
        g_isErrorStreamTTY = IS_TTY(ErrorStream);
#ifdef _WIN32
        Encoding::SetOutputCodePage(CP_UTF8);
        try
        {
            OutputStream::SetMode(OutputStream::GetMode() | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
        catch (NoValidTTYException&)
        {
        }
        try
        {
            ErrorStream::SetMode(ErrorStream::GetMode() | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
        catch (NoValidTTYException&)
        {
        }
#endif
    }

    void Terminal::WriteANSIEscapeSequence(std::string format, std::va_list arguments)
    {
        if (!OutputStream::IsTTY() && !ErrorStream::IsTTY())
        {
            throw NoValidTTYException();
        }
        if (std::vfprintf(OutputStream::IsTTY() ? OutputStream::GetFile() : ErrorStream::GetFile(), format.c_str(), arguments) < 0)
        {
            throw WideCharacterOrientationException();
        }
    }

    void Terminal::WriteANSIEscapeSequence(std::string format, ...)
    {
        std::va_list arguments;
        va_start(arguments, format);
        WriteANSIEscapeSequence(format, arguments);
        va_end(arguments);
    }

    void Terminal::Write(std::FILE* file, const char* format, std::va_list arguments, bool hasNewLine)
    {
        if (file == ErrorStream::GetFile())
        {
            OutputStream::Flush();
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

    EventInfo Terminal::ReadEvent(bool allowMouseCapture, std::chrono::milliseconds wait, std::function<bool(EventInfo&)> filter)
    {
        if (!InputStream::IsTTY() || (!OutputStream::IsTTY() && !ErrorStream::IsTTY()))
        {
            throw NoValidTTYException();
        }
        if (std::fwide(InputStream::GetFile(), 0) > 0)
        {
            throw WideCharacterOrientationException();
        }
        EventInfo eventInfo = EventType::None;
#ifdef _WIN32
        HANDLE timer = nullptr;
        DWORD mode = InputStream::GetMode();
        InputStream::SetMode(allowMouseCapture ? (mode | ENABLE_MOUSE_INPUT) & ~(ENABLE_QUICK_EDIT_MODE | ENABLE_PROCESSED_INPUT) : mode & ~ENABLE_PROCESSED_INPUT);
        while (true)
        {
            if (!wait.count())
            {
                if (!InputStream::GetTotalEventsCached())
                {
                    eventInfo = EventType::None;
                    break;
                }
            }
            if (wait.count() > 0)
            {
                if (!timer)
                {
                    timer = CreateWaitableTimerW(nullptr, true, nullptr);
                    LARGE_INTEGER duration;
                    duration.QuadPart = -10000 * wait.count();
                    SetWaitableTimer(timer, &duration, 1, nullptr, nullptr, false);
                }
                HANDLE handles[] = {timer, InputStream::GetHandle()};
                if (WaitForMultipleObjects(2, handles, false, INFINITE) == WAIT_OBJECT_0)
                {
                    eventInfo = EventType::TimeOut;
                    break;
                }
            }
            INPUT_RECORD record;
            DWORD totalEventsRead;
            ReadConsoleInputW(InputStream::GetHandle(), &record, 1, &totalEventsRead);
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
                    bufferInfo = OutputStream::GetWindowBufferInfo();
                }
                catch (NoValidTTYException&)
                {
                    bufferInfo = ErrorStream::GetWindowBufferInfo();
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
                        ReadConsoleInputW(InputStream::GetHandle(), &record, 1, &totalEventsRead);
                        ReadConsoleInputW(InputStream::GetHandle(), &record, 1, &totalEventsRead);
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
        if (timer)
        {
            CloseHandle(timer);
        }
        InputStream::SetMode(mode);
#endif
        return eventInfo;
    }

#ifdef _WIN32
    DWORD Terminal::GetStreamMode(HANDLE handle)
    {
        DWORD mode;
        if (!GetConsoleMode(handle, &mode))
        {
            throw NoValidTTYException();
        }
        return mode;
    }

    CONSOLE_SCREEN_BUFFER_INFO Terminal::GetStreamWindowBufferInfo(HANDLE handle)
    {
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        if (!GetConsoleScreenBufferInfo(handle, &bufferInfo))
        {
            throw NoValidTTYException();
        }
        return bufferInfo;
    }

    void Terminal::SetStreamMode(HANDLE handle, bool isTTY, DWORD mode)
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
    HANDLE Terminal::InputStream::GetHandle()
    {
        return GetStdHandle(STD_INPUT_HANDLE);
    }

    DWORD Terminal::InputStream::GetMode()
    {
        return GetStreamMode(GetHandle());
    }

    DWORD Terminal::InputStream::GetTotalEventsCached()
    {
        DWORD totalEvents;
        if (!GetNumberOfConsoleInputEvents(GetHandle(), &totalEvents))
        {
            throw NoValidTTYException();
        }
        return totalEvents;
    }

    void Terminal::InputStream::SetMode(DWORD mode)
    {
        SetStreamMode(GetHandle(), IsTTY(), mode);
    }
#else
    struct termios Terminal::InputStream::GetTermiosAttributes()
    {
        struct termios attributes;
        if (tcgetattr(GetFileNumber(), &attributes))
        {
            throw NoValidTTYException();
        }
        return attributes;
    }

    void Terminal::InputStream::SetTermiosAttributes(struct termios& attributes)
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

    void Terminal::InputStream::SetFCNTLBlockingState(bool isToEnable)
    {
        int flags = fcntl(GetFileNumber(), F_GETFL);
        fcntl(GetFileNumber(), F_SETFL, isToEnable ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
    }
#endif

    std::FILE* Terminal::InputStream::GetFile()
    {
        return stdin;
    }

    int Terminal::InputStream::GetFileNumber()
    {
        return 0;
    }

    void Terminal::InputStream::Clear()
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

    bool Terminal::InputStream::IsTTY()
    {
        InitStreamTTYCache();
        return g_isInputStreamTTY;
    }

    char Terminal::InputStream::ReadByte()
    {
        return std::getchar();
    }

    EventInfo Terminal::InputStream::ReadEvent(bool allowMouseCapture)
    {
        return Terminal::ReadEvent(allowMouseCapture, -1ms, nullptr);
    }

    EventInfo Terminal::InputStream::ReadEvent(bool allowMouseCapture, std::chrono::milliseconds wait)
    {
        return Terminal::ReadEvent(allowMouseCapture, wait, nullptr);
    }

    EventInfo Terminal::InputStream::ReadEvent(bool allowMouseCapture, std::chrono::milliseconds wait, std::function<bool(EventInfo&)> filter)
    {
        return Terminal::ReadEvent(allowMouseCapture, wait, filter);
    }

#ifdef _WIN32
    HANDLE Terminal::OutputStream::GetHandle()
    {
        return GetStdHandle(STD_OUTPUT_HANDLE);
    }

    DWORD Terminal::OutputStream::GetMode()
    {
        return GetStreamMode(GetHandle());
    }

    CONSOLE_SCREEN_BUFFER_INFO Terminal::OutputStream::GetWindowBufferInfo()
    {
        return GetStreamWindowBufferInfo(GetHandle());
    }

    void Terminal::OutputStream::SetMode(DWORD mode)
    {
        SetStreamMode(GetHandle(), IsTTY(), mode);
    }
#endif

    void Terminal::OutputStream::Flush()
    {
        std::fflush(GetFile());
    }

    void Terminal::OutputStream::WriteLine(std::string format, std::va_list arguments)
    {
        Terminal::Write(GetFile(), format.c_str(), arguments, true);
    }

    void Terminal::OutputStream::WriteLine(std::string format, ...)
    {
        std::va_list arguments;
        va_start(arguments, format);
        Terminal::Write(GetFile(), format.c_str(), arguments, true);
        va_end(arguments);
    }

    void Terminal::OutputStream::WriteLine()
    {
        Terminal::Write(GetFile(), nullptr, nullptr, true);
    }

    void Terminal::OutputStream::Write(std::string format, std::va_list arguments)
    {
        Terminal::Write(GetFile(), format.c_str(), arguments, false);
    }

    void Terminal::OutputStream::Write(std::string format, ...)
    {
        std::va_list arguments;
        va_start(arguments, format);
        Terminal::Write(GetFile(), format.c_str(), arguments, false);
        va_end(arguments);
    }

    std::FILE* Terminal::OutputStream::GetFile()
    {
        return stdout;
    }

    int Terminal::OutputStream::GetFileNumber()
    {
        return 1;
    }

    bool Terminal::OutputStream::IsTTY()
    {
        InitStreamTTYCache();
        return g_isOutputStreamTTY;
    }

#ifdef _WIN32
    HANDLE Terminal::ErrorStream::GetHandle()
    {
        return GetStdHandle(STD_ERROR_HANDLE);
    }

    DWORD Terminal::ErrorStream::GetMode()
    {
        return GetStreamMode(GetHandle());
    }

    CONSOLE_SCREEN_BUFFER_INFO Terminal::ErrorStream::GetWindowBufferInfo()
    {
        return GetStreamWindowBufferInfo(GetHandle());
    }

    void Terminal::ErrorStream::SetMode(DWORD mode)
    {
        SetStreamMode(GetHandle(), IsTTY(), mode);
    }
#endif

    void Terminal::ErrorStream::WriteLine(std::string format, std::va_list arguments)
    {
        Terminal::Write(GetFile(), format.c_str(), arguments, true);
    }

    void Terminal::ErrorStream::WriteLine(std::string format, ...)
    {
        std::va_list arguments;
        va_start(arguments, format);
        Terminal::Write(GetFile(), format.c_str(), arguments, true);
        va_end(arguments);
    }

    void Terminal::ErrorStream::WriteLine()
    {
        Terminal::Write(GetFile(), nullptr, nullptr, true);
    }

    void Terminal::ErrorStream::Write(std::string format, std::va_list arguments)
    {
        Terminal::Write(GetFile(), format.c_str(), arguments, false);
    }

    void Terminal::ErrorStream::Write(std::string format, ...)
    {
        std::va_list arguments;
        va_start(arguments, format);
        Terminal::Write(GetFile(), format.c_str(), arguments, false);
        va_end(arguments);
    }

    std::FILE* Terminal::ErrorStream::GetFile()
    {
        return stderr;
    }

    int Terminal::ErrorStream::GetFileNumber()
    {
        return 2;
    }

    bool Terminal::ErrorStream::IsTTY()
    {
        InitStreamTTYCache();
        return g_isErrorStreamTTY;
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

    Region Terminal::Window::GetRegion()
    {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        if (!GetConsoleScreenBufferInfo(OutputStream::GetHandle(), &bufferInfo) && !GetConsoleScreenBufferInfo(ErrorStream::GetHandle(), &bufferInfo))
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

    void Terminal::Window::OpenAlternateWindow()
    {
        try
        {
            Terminal::WriteANSIEscapeSequence("\x1b[?1049h\x1b[2J\x1b[1;1H");
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Window::CloseAlternateWindow()
    {
        try
        {
            Terminal::WriteANSIEscapeSequence("\x1b[?1049l");
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Window::SetTitle(std::string format, std::va_list arguments)
    {
        try
        {
            Terminal::WriteANSIEscapeSequence("\x1b]0;");
            Terminal::WriteANSIEscapeSequence(format, arguments);
            Terminal::WriteANSIEscapeSequence("\7");
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
            Terminal::WriteANSIEscapeSequence("\7");
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Font::SetWeight(FontWeight weight)
    {
        try
        {
            Terminal::WriteANSIEscapeSequence("\x1b[22;%dm", static_cast<int>(weight));
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Font::SetXColor(unsigned char color, FontLayer layer)
    {
        try
        {
            Terminal::WriteANSIEscapeSequence("\x1b[%d8;5;%hum", static_cast<int>(layer), color);
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
            Terminal::WriteANSIEscapeSequence("\x1b[%d8;2;%hu;%hu;%hum", static_cast<int>(layer), red, green, blue);
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
                    Terminal::WriteANSIEscapeSequence("\x1b[%dm", code);
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

    void Terminal::Font::ResetColors()
    {
        try
        {
            Terminal::WriteANSIEscapeSequence("\x1b[39;49m");
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Font::ResetWeight()
    {
        try
        {
            Terminal::WriteANSIEscapeSequence("\x1b[22m");
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Font::ResetEffects()
    {
        for (std::size_t code = 23; code < 30; ++code)
        {
            if (code != 26)
            {
                try
                {
                    Terminal::WriteANSIEscapeSequence("\x1b[%dm", code);
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
            bufferInfo = OutputStream::GetWindowBufferInfo();
        }
        catch (NoValidTTYException&)
        {
            bufferInfo = ErrorStream::GetWindowBufferInfo();
        }
        return Coordinate(bufferInfo.dwCursorPosition.X - bufferInfo.srWindow.Left, bufferInfo.dwCursorPosition.Y - bufferInfo.srWindow.Top);
#else
        struct termios attributes = InputStream::GetTermiosAttributes();
        InputStream::Clear();
        Terminal::WriteANSIEscapeSequence("\x1b[6n");
        attributes.c_lflag &= ~(ICANON | ECHO);
        InputStream::SetTermiosAttributes(attributes);
        unsigned short column;
        unsigned short row;
        int totalMatchesRead = std::scanf("\x1b[%hu;%huR", &row, &column);
        attributes.c_lflag |= ICANON | ECHO;
        InputStream::SetTermiosAttributes(attributes);
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
            if (!Window::GetRegion().Contains(column, row))
            {
                throw OutOfRangeException();
            }
            Terminal::WriteANSIEscapeSequence("\x1b[%hu;%huH", row + 1, column + 1);
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
            Terminal::WriteANSIEscapeSequence("\x1b[%d q", static_cast<int>(shape) - isBlinking);
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Cursor::SetVisibility(bool isVisible)
    {
        try
        {
            Terminal::WriteANSIEscapeSequence("\x1b[?25%c", isVisible ? 'h' : 'l');
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Cursor::ResetShape()
    {
        try
        {
            Terminal::WriteANSIEscapeSequence("\x1b[0 q");
        }
        catch (NoValidTTYException&)
        {
        }
    }

    void Terminal::Cursor::ClearLine()
    {
        try
        {
            Terminal::WriteANSIEscapeSequence("\x1b[2K\x1b[1G");
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
