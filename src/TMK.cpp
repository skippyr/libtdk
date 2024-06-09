#include "TMK.hpp"

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

#ifdef _WIN32
#define TTY_CACHE(a_stream) (!!_isatty(a_stream::GetFileNumber()) << a_stream::GetFileNumber())
#else
#define TTY_CACHE(a_stream) (isatty(a_stream::GetFileNumber()) << a_stream::GetFileNumber())
#endif
#define IS_TTY(a_stream) (g_ttyCache & 1 << a_stream::GetFileNumber())
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

        static void WriteANSISequence(std::string format, ...)
        {
            InitEnvironment();
            if (!IS_TTY(Terminal::Output) && !IS_TTY(Terminal::Error))
            {
                return;
            }
            std::va_list arguments;
            va_start(arguments, format);
            std::vfprintf(IS_TTY(Terminal::Output) ? Terminal::Output::GetFile() : Terminal::Error::GetFile(), format.c_str(), arguments);
            va_end(arguments);
        }

        static EventInfo ReadEvent(bool allowMouseCapture, int waitInMilliseconds, std::function<bool(EventInfo&)> filter)
        {
            InitEnvironment();
            if (!IS_TTY(Terminal::Input) || (!IS_TTY(Terminal::Output) && !IS_TTY(Terminal::Error)))
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
    CMDArguments::CMDArguments(int totalArguments, char** utf8Arguments, wchar_t** utf16Arguments)
        : m_totalArguments(totalArguments), m_utf8Arguments(utf8Arguments), m_utf16Arguments(utf16Arguments)
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

    Dimensions::Dimensions() : m_totalColumns(0), m_totalRows(0)
    {
    }

    Dimensions::Dimensions(unsigned short totalColumns, unsigned short totalRows) : m_totalColumns(totalColumns), m_totalRows(totalRows)
    {
    }

    unsigned short Dimensions::GetTotalColumns() const
    {
        return m_totalColumns;
    }

    unsigned short Dimensions::GetTotalRows() const
    {
        return m_totalRows;
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

    ResizeEvent::ResizeEvent() : m_dimensions(Terminal::Window::GetDimensions())
    {
    }

    Dimensions ResizeEvent::GetDimensions() const
    {
        return m_dimensions;
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

    std::FILE* Terminal::Input::GetFile()
    {
        return stdin;
    }

    int Terminal::Input::GetFileNumber()
    {
        return 0;
    }

    bool Terminal::Input::IsTTY()
    {
        return IS_TTY(Input);
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

    void Terminal::Output::WriteLine(std::string format, ...)
    {
        Setup::InitEnvironment();
        std::va_list arguments;
        va_start(arguments, format);
        if (std::vprintf(format.c_str(), arguments) < 0)
        {
            throw WideCharacterOrientationException();
        }
        std::putchar('\n');
        va_end(arguments);
    }

    void Terminal::Output::WriteLine()
    {
        Setup::InitEnvironment();
        if (std::putchar('\n') == EOF)
        {
            throw WideCharacterOrientationException();
        }
    }

    void Terminal::Output::Write(std::string format, ...)
    {
        Setup::InitEnvironment();
        std::va_list arguments;
        va_start(arguments, format);
        if (std::vprintf(format.c_str(), arguments) < 0)
        {
            throw WideCharacterOrientationException();
        }
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
        return IS_TTY(Output);
    }

    void Terminal::Error::WriteLine(std::string format, ...)
    {
        Setup::InitEnvironment();
        Terminal::Output::Flush();
        std::va_list arguments;
        va_start(arguments, format);
        if (std::vfprintf(GetFile(), format.c_str(), arguments) < 0)
        {
            throw WideCharacterOrientationException();
        }
        std::fputc('\n', GetFile());
        va_end(arguments);
    }

    void Terminal::Error::WriteLine()
    {
        Setup::InitEnvironment();
        Terminal::Output::Flush();
        if (std::fputc('\n', GetFile()) == EOF)
        {
            throw WideCharacterOrientationException();
        }
    }

    void Terminal::Error::Write(std::string format, ...)
    {
        Setup::InitEnvironment();
        Terminal::Output::Flush();
        std::va_list arguments;
        va_start(arguments, format);
        if (std::vfprintf(GetFile(), format.c_str(), arguments) < 0)
        {
            throw WideCharacterOrientationException();
        }
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
        return IS_TTY(Error);
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
        return CMDArguments(rawTotalCMDArguments, utf8Arguments, utf16Arguments);
#else
        return CMDArguments(rawTotalCMDArguments, rawCMDArguments);
#endif
    }

    void Terminal::Process::Exit(int exitCode)
    {
        std::exit(exitCode);
    }

    Dimensions Terminal::Window::GetDimensions()
    {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo) && !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE), &bufferInfo))
        {
            throw NoValidTTYException();
        }
        return Dimensions(bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1, bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1);
#else
        struct winsize size;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) && ioctl(STDERR_FILENO, TIOCGWINSZ, &size))
        {
            throw NoValidTTYException();
        }
        return Dimensions(size.ws_col, size.ws_row);
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
        if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo) && !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE), &bufferInfo))
        {
            throw NoValidTTYException();
        }
        return Coordinate(bufferInfo.dwCursorPosition.X - bufferInfo.srWindow.Left, bufferInfo.dwCursorPosition.Y - bufferInfo.srWindow.Top);
#else
        struct termios attributes;
        ClearInputBuffer();
        if (WriteANSISequence("\x1b[6n") || tcgetattr(STDIN_FILENO, &attributes))
        {
            throw NoValidTTYException();
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
            throw NoValidTTYException();
        }
        return Coordinate(--column, --row);
#endif
    }

    void Terminal::Cursor::SetCoordinate(unsigned short column, unsigned short row)
    {
        TMK::Dimensions dimensions;
        try
        {
            dimensions = TMK::Terminal::Window::GetDimensions();
        }
        catch (TMK::NoValidTTYException&)
        {
            return;
        }
        if (column >= dimensions.GetTotalColumns() || row >= dimensions.GetTotalRows())
        {
            throw OutOfRangeException();
        }
        Setup::WriteANSISequence("\x1b[%hu;%huH", row + 1, column + 1);
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

    int TMK::operator|(FontEffect effect0, FontEffect effect1)
    {
        return static_cast<int>(effect0) | static_cast<int>(effect1);
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
