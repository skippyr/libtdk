#include "TMK.hpp"

#ifdef _WIN32
#include <io.h>
#else
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
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
        static void InitEnvironment()
        {
            if (!(g_ttyCache & TTY_CACHE_HAS_BEEN_FILLED_FLAG))
            {
#ifdef _WIN32
                HANDLE handle;
                DWORD mode;
                SetConsoleOutputCP(CP_UTF8);
                (GetConsoleMode((handle = GetStdHandle(STD_OUTPUT_HANDLE)), &mode) || GetConsoleMode((handle = GetStdHandle(STD_ERROR_HANDLE)), &mode)) &&
                    SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
                g_ttyCache |= TTY_CACHE(Terminal::Input) | TTY_CACHE(Terminal::Output) | TTY_CACHE(Terminal::Error) | TTY_CACHE_HAS_BEEN_FILLED_FLAG;
            }
        }

        static int WriteANSISequence(std::string format, ...)
        {
            if (!Terminal::Output::IsTTY() && !Terminal::Error::IsTTY())
            {
                return -1;
            }
            std::va_list arguments;
            va_start(arguments, format);
            int totalBytesWritten = std::vfprintf(Terminal::Output::IsTTY() ? Terminal::Output::GetFile() : Terminal::Error::GetFile(), format.c_str(), arguments);
            va_end(arguments);
            return -(totalBytesWritten < 0);
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

        static EventInfo ReadEvent(bool allowMouseCapture, int waitInMilliseconds, std::function<bool(EventInfo&)> filter)
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
            HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
            HANDLE timerHandle = nullptr;
            DWORD inputMode;
            GetConsoleMode(inputHandle, &inputMode);
            SetConsoleMode(inputHandle,
                           allowMouseCapture ? (inputMode | ENABLE_MOUSE_INPUT) & ~(ENABLE_QUICK_EDIT_MODE | ENABLE_PROCESSED_INPUT) : inputMode & ~ENABLE_PROCESSED_INPUT);
            while (true)
            {
                if (!waitInMilliseconds)
                {
                    DWORD totalEventsAvailable;
                    GetNumberOfConsoleInputEvents(inputHandle, &totalEventsAvailable);
                    if (!totalEventsAvailable)
                    {
                        eventInfo = EventType::None;
                        break;
                    }
                }
                if (waitInMilliseconds > 0)
                {
                    if (!timerHandle)
                    {
                        timerHandle = CreateWaitableTimerW(nullptr, true, nullptr);
                        LARGE_INTEGER duration;
                        duration.QuadPart = -10000 * waitInMilliseconds;
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
            SetConsoleMode(inputHandle, inputMode);
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
        for (int offset = 0; offset < m_totalArguments; ++offset)
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
        if (code > 0xffffff)
        {
            throw OutOfRangeException();
        }
        m_code = code;
    }

    HexColor::HexColor(RGBColor color) : m_code(color.GetRed() << 16 | color.GetGreen() << 8 | color.GetBlue())
    {
    }

    unsigned int HexColor::HexColor::GetCodeAsNumber() const
    {
        return m_code;
    }

    std::string HexColor::HexColor::GetCodeAsString(bool hasPrefix, bool hasZeroPadding, bool isUpperCase) const
    {
        char buffer[7];
        std::snprintf(buffer, sizeof(buffer), hasZeroPadding ? isUpperCase ? "%06X" : "%06x" : isUpperCase ? "%X" : "%x", m_code);
        return hasPrefix ? std::string("0x") + buffer : std::string(buffer);
    }

    RGBColor::RGBColor(unsigned char red, unsigned char green, unsigned char blue) : m_red(red), m_green(green), m_blue(blue)
    {
    }

    RGBColor::RGBColor(HexColor color) : m_red(color.GetCodeAsNumber() >> 16 & 0xff), m_green(color.GetCodeAsNumber() >> 8 & 0xff), m_blue(color.GetCodeAsNumber() & 0xff)
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
        DWORD mode;
        if (!GetConsoleMode(GetHandle(), &mode))
        {
            throw NoValidTTYException();
        }
        return mode;
    }

    void Terminal::Input::SetMode(DWORD mode)
    {
        if (!IsTTY())
        {
            throw NoValidTTYException();
        }
        if (!SetConsoleMode(GetHandle(), mode))
        {
            throw InvalidStreamModeException();
        }
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
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
#else
        struct termios attributes;
        if (tcgetattr(GetFileNumber(), &attributes))
        {
            return;
        }
        int flags = fcntl(GetFileNumber(), F_GETFL);
        attributes.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(GetFileNumber(), TCSANOW, &attributes);
        fcntl(GetFileNumber(), F_SETFL, flags | O_NONBLOCK);
        while (std::getchar() != EOF)
        {
        }
        attributes.c_lflag |= ICANON | ECHO;
        tcsetattr(GetFileNumber(), TCSANOW, &attributes);
        fcntl(GetFileNumber(), F_SETFL, flags);
#endif
    }

    bool Terminal::Input::IsTTY()
    {
        Setup::InitEnvironment();
        return IS_TTY(Input::GetFileNumber());
    }

    char Terminal::Input::ReadByte()
    {
        return std::getchar();
    }

    EventInfo Terminal::Input::ReadEvent(bool allowMouseCapture)
    {
        return Setup::ReadEvent(allowMouseCapture, -1, nullptr);
    }

    EventInfo Terminal::Input::ReadEvent(bool allowMouseCapture, unsigned short waitInMilliseconds)
    {
        return Setup::ReadEvent(allowMouseCapture, waitInMilliseconds, nullptr);
    }

    EventInfo Terminal::Input::ReadEvent(bool allowMouseCapture, unsigned short waitInMilliseconds, std::function<bool(EventInfo&)> filter)
    {
        return Setup::ReadEvent(allowMouseCapture, waitInMilliseconds, filter);
    }

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

#ifdef _WIN32
    HANDLE Terminal::Output::GetHandle()
    {
        return GetStdHandle(STD_OUTPUT_HANDLE);
    }
#endif

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
        return IS_TTY(Output::GetFileNumber());
    }

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

#ifdef _WIN32
    HANDLE Terminal::Error::GetHandle()
    {
        return GetStdHandle(STD_ERROR_HANDLE);
    }
#endif

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
        return IS_TTY(Error::GetFileNumber());
    }

    CMDArguments Terminal::Process::GetCMDArguments(int rawTotalCMDArguments, char** rawCMDArguments)
    {
#ifdef _WIN32
        LPWSTR* utf16Arguments = CommandLineToArgvW(GetCommandLineW(), &rawTotalCMDArguments);
        char** utf8Arguments = new char*[rawTotalCMDArguments];
        for (int offset = 0; offset < rawTotalCMDArguments; ++offset)
        {
            int size = WideCharToMultiByte(CP_UTF8, 0, utf16Arguments[offset], -1, nullptr, 0, nullptr, nullptr);
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
        Setup::WriteANSISequence("\x1b[?1049h\x1b[2J\x1b[1;1H");
    }

    void Terminal::Window::CloseAlternateWindow()
    {
        Setup::WriteANSISequence("\x1b[?1049l");
    }

    void Terminal::Window::SetTitle(std::string title)
    {
        Setup::WriteANSISequence("\x1b]0;%s\7", title.c_str());
    }

    void Terminal::Bell::Ring()
    {
        Setup::WriteANSISequence("\7");
    }

    void Terminal::Font::SetWeight(FontWeight weight)
    {
        Setup::WriteANSISequence("\x1b[22;%dm", static_cast<int>(weight));
    }

    void Terminal::Font::SetXColor(unsigned char color, FontLayer layer)
    {
        Setup::WriteANSISequence("\x1b[%d8;5;%hum", static_cast<int>(layer), color);
    }

    void Terminal::Font::SetXColor(XColor color, FontLayer layer)
    {
        Terminal::Font::SetXColor(static_cast<unsigned char>(color), layer);
    }

    void Terminal::Font::SetRGBColor(unsigned char red, unsigned char green, unsigned char blue, FontLayer layer)
    {
        Setup::WriteANSISequence("\x1b[%d8;2;%hu;%hu;%hum", static_cast<int>(layer), red, green, blue);
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
        Setup::WriteANSISequence("\x1b[39;49m");
    }

    void Terminal::Font::ResetWeight()
    {
        Setup::WriteANSISequence("\x1b[22m");
    }

    void Terminal::Font::SetEffects(int effects)
    {
        for (int offset = 6; offset < 32; ++offset)
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
        for (int code = 3; code < 10; ++code)
        {
            if (effects & 1 << code)
            {
                Setup::WriteANSISequence("\x1b[%dm", code);
            }
        }
    }

    void Terminal::Font::SetEffects(FontEffect effect)
    {
        SetEffects(static_cast<int>(effect));
    }

    void Terminal::Font::ResetEffects()
    {
        for (int code = 23; code < 30; ++code)
        {
            if (code != 26)
            {
                Setup::WriteANSISequence("\x1b[%dm", code);
            }
        }
    }

    Coordinate Terminal::Cursor::GetCoordinate()
    {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        if (!GetConsoleScreenBufferInfo(Output::GetHandle(), &bufferInfo) && !GetConsoleScreenBufferInfo(Error::GetHandle(), &bufferInfo))
        {
            throw NoValidTTYException();
        }
        return Coordinate(bufferInfo.dwCursorPosition.X - bufferInfo.srWindow.Left, bufferInfo.dwCursorPosition.Y - bufferInfo.srWindow.Top);
#else
        struct termios attributes;
        Terminal::Input::Clear();
        if (Setup::WriteANSISequence("\x1b[6n") || tcgetattr(Input::GetFileNumber(), &attributes))
        {
            throw NoValidTTYException();
        }
        attributes.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(Input::GetFileNumber(), TCSANOW, &attributes);
        unsigned short column;
        unsigned short row;
        int totalMatchesRead = std::scanf("\x1b[%hu;%huR", &row, &column);
        attributes.c_lflag |= ICANON | ECHO;
        tcsetattr(Input::GetFileNumber(), TCSANOW, &attributes);
        if (totalMatchesRead != 2)
        {
            throw NoValidTTYException();
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
            Setup::WriteANSISequence("\x1b[%hu;%huH", row + 1, column + 1);
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
        Setup::WriteANSISequence("\x1b[%d q", static_cast<int>(shape) - isBlinking);
    }

    void Terminal::Cursor::SetVisibility(bool isVisible)
    {
        Setup::WriteANSISequence("\x1b[?25%c", isVisible ? 'h' : 'l');
    }

    void Terminal::Cursor::ResetShape()
    {
        Setup::WriteANSISequence("\x1b[0 q");
    }

    void Terminal::Cursor::ClearLine()
    {
        Setup::WriteANSISequence("\x1b[2K\x1b[1G");
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
}
