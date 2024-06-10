#pragma once

#include <cstdarg>
#include <functional>
#include <iostream>
#include <string>

/**
 * @brief An open-source C++ terminal manipulation library made to develop cross-platform apps to for Windows and Linux. It uses an object-oriented philosophy to handle terminal
 * attributes, colors, effects, arguments and event readings. It primarily uses UTF-8 encoding, but has some exceptions that use UTF-16 encoding to fit within the Windows runtime
 * ecosystem.
 */
namespace TMK
{
    enum class ExitCode
    {
        OperationNotPermitted = 1,
        NoSuchFileOrDirectory = 2,
        InputOutputError = 5,
        PermissionDenied = 13,
        InvalidArgument = 22,
        CommandInvokedCannotExecute = 126,
        CommandNotFound = 127,
    };

    /**
     * @brief Contains the available terminal font weights.
     */
    enum class FontWeight
    {
        /**
         * @brief Usually rendered as bold weight and/or with bright colors.
         */
        Bold = 1,
        /**
         * @brief Usually rendered with faded colors.
         */
        Dim
    };

    /**
     * @brief Contains the available terminal font layers.
     */
    enum class FontLayer
    {
        /**
         * @brief Refers to the graphemes.
         */
        Foreground = 3,
        /**
         * @brief Refers to the background of the graphemes.
         */
        Background
    };

    /**
     * @brief Represents the available terminal effects.
     */
    enum class FontEffect
    {
        /**
         * @brief Makes the text curly.
         */
        Italic = 1 << 3,
        /**
         * @brief Draws a horizontal line crossing below the text.
         */
        Underline = 1 << 4,
        /**
         * @brief Makes the text blink indefinitely.
         */
        Blink = 1 << 5,
        /**
         * @brief Swaps the background and foreground colors.
         */
        Negative = 1 << 7,
        /**
         * @brief Makes the text hard to see or invisible.
         */
        Hidden = 1 << 8,
        /**
         * @brief Draws a horizontal line crossing through the middle of the text.
         */
        Strike = 1 << 9
    };

    /**
     * @brief Contains the ANSI codes of the first 16 colors of the XTerm color palette.
     */
    enum class XColor
    {
        /**
         * @brief The dark variant of the black color.
         */
        DarkBlack,
        /**
         * @brief The dark variant of the red color.
         */
        DarkRed,
        /**
         * @brief The dark variant of the green color.
         */
        DarkGreen,
        /**
         * @brief The dark variant of the yellow color.
         */
        DarkYellow,
        /**
         * @brief The dark variant of the blue color.
         */
        DarkBlue,
        /**
         * @brief The dark variant of the magenta color.
         */
        DarkMagenta,
        /**
         * @brief The dark variant of the cyan color.
         */
        DarkCyan,
        /**
         * @brief The dark variant of the white color.
         */
        DarkWhite,
        /**
         * @brief The light variant of the black color.
         */
        LightBlack,
        /**
         * @brief The light variant of the red color.
         */
        LightRed,
        /**
         * @brief The light variant of the green color.
         */
        LightGreen,
        /**
         * @brief The light variant of the yellow color.
         */
        LightYellow,
        /**
         * @brief The light variant of the blue color.
         */
        LightBlue,
        /**
         * @brief The light variant of the magenta color.
         */
        LightMagenta,
        /**
         * @brief The light variant of the cyan color.
         */
        LightCyan,
        /**
         * @brief The light variant of the white color.
         */
        LightWhite
    };

    /**
     * @brief Contains the available terminal cursor shapes.
     */
    enum class CursorShape
    {
        /**
         * @brief Fills the whole character cell.
         */
        Block = 2,
        /**
         * @brief Fills a small region at the bottom of the character cell.
         */
        Underline = 4,
        /**
         * @brief Fills a small region at the left of the character cell.
         */
        Bar = 6
    };

    /**
     * @brief Represents the available terminal event types.
     */
    enum class EventType
    {
        /**
         * @brief No event is available.
         */
        None,
        /**
         * @brief The event timer ran out.
         */
        TimeOut,
        /**
         * @brief The terminal window was resized.
         */
        Resize,
        /**
         * @brief The terminal window gained/lost focus.
         */
        Focus,
        /**
         * @brief The mouse was moved or clicked.
         */
        Mouse,
        /**
         * @brief A keyboard key was pressed.
         */
        Key
    };

    /**
     * @brief Contains the available mouse buttons.
     */
    enum class MouseButton
    {
        /**
         * @brief No button.
         */
        None,
        /**
         * @brief Left mouse button.
         */
        Left,
        /**
         * @brief Mouse wheel button.
         */
        Wheel,
        /**
         * @brief Mouse wheel scroll upwards.
         */
        WheelUp,
        /**
         * @brief Mouse wheel scroll downwards.
         */
        WheelDown,
        /**
         * @brief Right mouse button.
         */
        Right
    };

    /**
     * @brief Contains the available keyboard keys.
     */
    enum class KeyboardKey
    {
#ifdef _WIN32
        /**
         * @brief The left arrow key.
         */
        LeftArrow = -23,
        /**
         * @brief The up arrow key.
         */
        UpArrow,
        /**
         * @brief The right arrow key.
         */
        RightArrow,
        /**
         * @brief The down arrow key.
         */
        DownArrow,
#else
        /**
         * @brief The up arrow key.
         */
        UpArrow = -23,
        /**
         * @brief The down arrow key.
         */
        DownArrow,
        /**
         * @brief The right arrow key.
         */
        RightArrow,
        /**
         * @brief The left arrow key.
         */
        LeftArrow,
#endif
        /**
         * @brief The F1 key.
         */
        F1,
        /**
         * @brief The F2 key.
         */
        F2,
        /**
         * @brief The F3 key.
         */
        F3,
        /**
         * @brief The F4 key.
         */
        F4,
        /**
         * @brief The F5 key.
         */
        F5,
        /**
         * @brief The F6 key.
         */
        F6,
        /**
         * @brief The F7 key.
         */
        F7,
        /**
         * @brief The F8 key.
         */
        F8,
        /**
         * @brief The F9 key.
         */
        F9,
        /**
         * @brief The F10 key.
         */
        F10,
        /**
         * @brief The F11 key.
         */
        F11,
        /**
         * @brief The F12 key.
         */
        F12,
#ifdef _WIN32
        /**
         * @brief The PageUp key.
         */
        PageUp,
        /**
         * @brief The PageDown key.
         */
        PageDown,
        /**
         * @brief The End key.
         */
        End,
        /**
         * @brief The Home key.
         */
        Home,
        /**
         * @brief The Insert key.
         */
        Insert,
        /**
         * @brief The Delete key.
         */
        Delete,
#else
        /**
         * @brief The Home key.
         */
        Home,
        /**
         * @brief The Insert key.
         */
        Insert,
        /**
         * @brief The Delete key.
         */
        Delete,
        /**
         * @brief The End key.
         */
        End,
        /**
         * @brief The PageUp key.
         */
        PageUp,
        /**
         * @brief The PageDown key.
         */
        PageDown,
#endif
        /**
         * @brief The Tab key.
         */
        Tab = 9,
#if defined(_WIN32) || defined(__APPLE__)
        /**
         * @brief The Enter key.
         */
        Enter = 13,
#else
        /**
         * @brief The Enter key.
         */
        Enter,
#endif
        /**
         * @brief The Escape key.
         */
        Escape = 27,
        /**
         * @brief The Space bar key.
         */
        SpaceBar = 32,
        /**
         * @brief The Backspace key.
         */
        Backspace = 127
    };

    class RGBColor;

    /**
     * @brief Represents an exception thrown whenever a group of streams are wide character oriented.
     */
    class WideCharacterOrientationException final
    {
    };

    /**
     * @brief Represents an exception thrown whenever a group of streams are not TTY.
     */
    class NoValidTTYException final
    {
    };

    /**
     * @brief Represents an exception thrown whenever a value is out of a certain range.
     */
    class OutOfRangeException final
    {
    };

    /**
     * @brief Represents an exception thrown whenever an invalid event type data tries to be read.
     */
    class InvalidEventTypeException final
    {
    };

    /**
     * @brief Represents the command line arguments.
     */
    class CMDArguments final
    {
    public:
#ifdef _WIN32
        /**
         * @brief Creates a new instance of the CMDArguments class.
         * @param totalArguments The total arguments.
         * @param utf16Arguments The arguments in UTF-16 encoding.
         * @param utf8Arguments The arguments in UTF-8 encoding.
         * @returns An instance of the CMDArguments class.
         */
        CMDArguments(int totalArguments, wchar_t** utf16Arguments, char** utf8Arguments);
        /**
         * @brief Destroys an instance of the CMDArguments class.
         */
        ~CMDArguments();
        /**
         * @brief Gets an argument in UTF-16 encoding by using its offset.
         * @param offset The offset to be used.
         * @returns The argument in UTF-16 encoding.
         * @exception OutOfRangeException Thrown whenever the offset is out of the valid range of arguments.
         */
        std::wstring GetUTF16ArgumentByOffset(std::size_t offset) const;
#else
        /**
         * @brief Creates a new instance of the CMDArguments class.
         * @param totalArguments The total arguments.
         * @param utf8Arguments The arguments in UTF8 encoding.
         * @returns An instance of the CMDArguments class.
         */
        CMDArguments(int totalArguments, char** utf8Arguments);
#endif
        /**
         * @brief Gets the total arguments.
         * @returns The total arguments.
         */
        int GetTotalArguments() const;
        /**
         * @brief Gets an argument in UTF-8 encoding by using its offset.
         * @param offset The offset to be used.
         * @returns The argument in UTF-8 encoding.
         * @throws OutOfRangeException Thrown whenever the offset is out of the valid range of arguments.
         */
        std::string GetUTF8ArgumentByOffset(std::size_t offset) const;

    private:
#ifdef _WIN32
        /**
         * @brief The arguments in UTF-16 encoding.
         */
        wchar_t** m_utf16Arguments;
#endif
        /**
         * @brief The arguments in UTF-8 encoding.
         */
        char** m_utf8Arguments;
        /**
         * @brief The total arguments.
         */
        int m_totalArguments;
    };

    /**
     * @brief Represents a coordinate inside of the terminal window.
     */
    class Coordinate final
    {
    public:
        /**
         * @brief Creates a new instance of the Coordinate class.
         * @returns An instance of the Coordinate class.
         */
        Coordinate();
        /**
         * @brief Creates a new instance of the Coordinate class.
         * @param column The column component of the coordinate.
         * @param row The row component of the coordinate.
         * @returns An instance of the Coordinate class.
         */
        Coordinate(unsigned short column, unsigned short row);
        /**
         * @brief Gets the column component of the coordinate.
         * @returns The column component of the coordinate.
         */
        unsigned short GetColumn() const;
        /**
         * @brief Gets the row component of the coordinate.
         * @returns The row component of the coordinate.
         */
        unsigned short GetRow() const;
        /**
         * @brief Sets the column component of the coordinate.
         * @param column The column component of the coordinate to be set.
         */
        void SetColumn(unsigned short column);
        /**
         * @brief Sets the row component of the coordinate.
         * @param row The row component of the coordinate to be set.
         */
        void SetRow(unsigned short row);
        /**
         * @brief Gets a string representation of the coordinate.
         * @returns A string representation of the coordinate.
         */
        std::string ToString() const;

    private:
        /**
         * @brief The column component of the coordinate.
         */
        unsigned short m_column;
        /**
         * @brief The row component of the coordinate.
         */
        unsigned short m_row;
    };

    /**
     * @brief Represents a geometry inside of the terminal window.
     */
    class Geometry final
    {
    public:
        /**
         * @brief Creates an instance of the Geometry class.
         * @returns An instance of the Geometry class.
         */
        Geometry();
        /**
         * @brief Creates an instance of the Geometry class.
         * @param totalColumns The total columns of the geometry.
         * @param totalRows The total rows of the geometry.
         * @returns An instance of the Geometry class.
         */
        Geometry(unsigned short totalColumns, unsigned short totalRows);
        /**
         * @brief Creates an instance of the Geometry class.
         * @param coordinateI The first coordinate of the geometry.
         * @param coordinateII The second coordinate of the geometry. It must be opposite to the first one.
         * @returns An instance of the Geometry class.
         */
        Geometry(Coordinate coordinateI, Coordinate coordinateII);
        /**
         * @brief Gets the total columns of the geometry.
         * @returns The total columns of the geometry.
         */
        unsigned short GetTotalColumns() const;
        /**
         * @brief Gets the total rows of the geometry.
         * @returns The total rows of the geometry.
         */
        unsigned short GetTotalRows() const;
        /**
         * @brief Gets the area of the geometry.
         * @returns The area of the geometry.
         */
        unsigned int GetArea() const;
        /**
         * @brief Gets the top left coordinate of the geometry.
         * @returns The top left coordinate of the geometry.
         */
        Coordinate GetTopLeftCoordinate() const;
        /**
         * @brief Gets the top right coordinate of the geometry.
         * @returns The top right coordinate of the geometry.
         */
        Coordinate GetTopRightCoordinate() const;
        /**
         * @brief Gets the bottom left coordinate of the geometry.
         * @returns The bottom left coordinate of the geometry.
         */
        Coordinate GetBottomLeftCoordinate() const;
        /**
         * @brief Gets the bottom right coordinate of the geometry.
         * @returns The bottom right coordinate of the geometry.
         */
        Coordinate GetBottomRightCoordinate() const;
        /**
         * @brief Checks if the geometry contains a coordinate.
         * @param column The column component of the coordinate.
         * @param row The row component of the coordinate.
         * @returns A boolean that states the geometry contains the coordinate.
         */
        bool Contains(unsigned short column, unsigned short row) const;
        /**
         * @brief Checks if the geometry contains a coordinate.
         * @param coordinate The coordinate to be checked.
         * @returns A boolean that states the geometry contains the coordinate.
         */
        bool Contains(Coordinate coordinate) const;

    private:
        /**
         * @brief The total columns of the geometry.
         */
        unsigned short m_totalColumns;
        /**
         * @brief The total rows of the geometry.
         */
        unsigned short m_totalRows;
        /**
         * @brief The area of the geometry.
         */
        unsigned int m_area;
        /**
         * @brief The top left coordinate of the geometry.
         */
        Coordinate m_topLeftCoordinate;
        /**
         * @brief The top right coordinate of the geometry.
         */
        Coordinate m_topRightCoordinate;
        /**
         * @brief The bottom left coordinate of the geometry.
         */
        Coordinate m_bottomLeftCoordinate;
        /**
         * @brief The bottom right coordinate of the geometry.
         */
        Coordinate m_bottomRightCoordinate;
    };

    /**
     * @brief Represents a color in hex format.
     */
    class HexColor final
    {
    public:
        /**
         * @brief Creates an instance of the HexColor class.
         * @param code The hex code of the color. It must be a value in range from 0x0 to 0xffffff.
         * @throws OutOfRangeException Thrown whenever the hex code is out of the valid range.
         * @returns An instance of the HexColor class.
         */
        HexColor(unsigned int code);
        /**
         * @brief Creates an instance of the HexColor class.
         * @param color A color in RGB format to be converted.
         * @returns An instance of the HexColor class.
         */
        HexColor(RGBColor color);
        /**
         * @brief Gets number representation of the hex code of the color.
         */
        unsigned int GetCodeAsNumber() const;
        /**
         * @brief Gets the string representation of the hex code of the color.
         * @param hasPrefix A boolean that states the string has the 0x prefix.
         * @param hasZeroPadding A boolean that states the string has a left padding filled by the number zero.
         * @param isUpperCase A boolean that states the string is upper case.
         */
        std::string GetCodeAsString(bool hasPrefix, bool hasZeroPadding, bool isUpperCase) const;

    private:
        /**
         * @brief The hex code of the color.
         */
        unsigned int m_code;
    };

    /**
     * @brief Represents a color in RGB format.
     */
    class RGBColor final
    {
    public:
        /**
         * @brief Creates an instance of the RGBColor class.
         * @param red The red component of the color.
         * @param green The green component of the color.
         * @param blue The blue component of the color.
         * @returns An instance of the RGBColor class.
         */
        RGBColor(unsigned char red, unsigned char green, unsigned char blue);
        /**
         * @brief Creates an instance of the RGBColor class.
         * @param color A color in hex format to be converted.
         */
        RGBColor(HexColor color);
        /**
         * @brief Gets the red component of the color.
         * @returns The red component of the color.
         */
        unsigned char GetRed() const;
        /**
         * @brief Gets the green component of the color.
         * @returns The green component of the color.
         */
        unsigned char GetGreen() const;
        /**
         * @brief Gets the blue component of the color.
         * @returns The blue component of the color.
         */
        unsigned char GetBlue() const;

    private:
        /**
         * @brief The red component of the color.
         */
        unsigned char m_red;
        /**
         * @brief The green component of the color.
         */
        unsigned char m_green;
        /**
         * @brief The blue component of the color.
         */
        unsigned char m_blue;
    };

    /**
     * @brief Represents a terminal focus event.
     */
    class FocusEvent final
    {
    public:
        /**
         * @brief Creates a new instance of the FocusEvent class.
         * @param hasFocus A boolean that states the terminal window has gained focus.
         * @returns An instance of the FocusEvent class.
         */
        FocusEvent(bool hasFocus);
        /**
         * @brief Checks if the terminal window has gained focus.
         * @returns A boolean that states the terminal window has gained focus.
         */
        bool HasFocus() const;

    private:
        /**
         * @brief A boolean that states the terminal window has gained focus.
         */
        bool m_hasFocus;
    };

    /**
     * @brief Represents a terminal window resize event.
     */
    class ResizeEvent final
    {
    public:
        /**
         * @brief Creates a new instance of the ResizeEvent class.
         * @returns An instance of the ResizeEvent class.
         */
        ResizeEvent();
        /**
         * @brief Gets the terminal window geometry.
         * @returns The terminal window geometry.
         */
        Geometry GetWindowGeometry() const;

    private:
        /**
         * @brief The terminal window geometry.
         */
        Geometry m_windowGeometry;
    };

    /**
     * @brief Represents a terminal mouse event.
     */
    class MouseEvent final
    {
    public:
        /**
         * @brief Creates a new instance of the MouseEvent class.
         * @param coordinate The mouse coordinate.
         * @param button The mouse button pressed.
         * @param isDragging A boolean that states the mouse was being dragged.
         * @param hasCtrl A boolean that states the Ctrl key was being holded.
         * @param hasAlt A boolean that states the Alt key was being holded.
         * @param hasShift A boolean that states the Shift key was being holded.
         * @returns An instance of the MouseEvent class.
         */
        MouseEvent(Coordinate coordinate, MouseButton button, bool isDragging, bool hasCtrl, bool hasAlt, bool hasShift);
        /**
         * @brief Gets the mouse coordinate.
         * @returns The mouse coordinate.
         */
        Coordinate GetCoordinate() const;
        /**
         * @brief Gets the mouse button pressed.
         * @returns The mouse button pressed.
         */
        MouseButton GetButton() const;
        /**
         * @brief Checks if the mouse was being dragged.
         * @returns A boolean that states the mouse was being dragged.
         */
        bool IsDragging() const;
        /**
         * @brief Checks if the Ctrl was being holded.
         * @returns A boolean that states the Ctrl key was being holded.
         */
        bool HasCtrl() const;
        /**
         * @brief Checks if the Alt was being holded.
         * @returns A boolean that states the Alt key was being holded.
         */
        bool HasAlt() const;
        /**
         * @brief Checks if the Shift was being holded.
         * @returns A boolean that states the Shift key was being holded.
         */
        bool HasShift() const;

    private:
        /**
         * @brief The mouse coordinate.
         */
        Coordinate m_coordinate;
        /**
         * @brief The mouse button pressed.
         */
        MouseButton m_button;
        /**
         * @brief A boolean that states the mouse was being dragged.
         */
        bool m_isDragging;
        /**
         * @brief A boolean that states the Ctrl key was being holded.
         */
        bool m_hasCtrl;
        /**
         * @brief A boolean that states the Alt key was being holded.
         */
        bool m_hasAlt;
        /**
         * @brief A boolean that states the Shift key was being holded.
         */
        bool m_hasShift;
    };

    /**
     * @brief Represents a terminal key event.
     */
    class KeyEvent final
    {
    public:
        /**
         * @brief Creates a new instance of the KeyEvent class.
         * @param key The key pressed. It may be an UTF-8 grapheme or an enumerator from the KeyboardKey enum class.
         * @param hasCtrl A boolean that states the Ctrl key was being holded.
         * @param hasAlt A boolean that states the Alt key was being holded.
         * @param hasShift A boolean that states the Shift key was being holded.
         * @returns An instance of the KeyEvent class.
         */
        KeyEvent(int key, bool hasCtrl, bool hasAlt, bool hasShift);
        /**
         * @brief Gets the key pressed.
         * @returns The key pressed. It may be an UTF-8 grapheme or an enumerator from the KeyboardKey enum class.
         */
        int GetKey() const;
        /**
         * @brief Checks if the Ctrl key was being holded.
         * @returns A boolean that states the Ctrl key was being holded.
         */
        bool HasCtrl() const;
        /**
         * @brief Checks if the Alt was being holded.
         * @returns A boolean that states the Alt key was being holded.
         */
        bool HasAlt() const;
        /**
         * @brief Checks if the Shift was being holded.
         * @returns A boolean that states the Shift key was being holded.
         */
        bool HasShift() const;

    private:
        /**
         * @brief The key pressed. It may be an UTF-8 grapheme or an enumerator from the KeyboardKey enum class.
         */
        int m_key;
        /**
         * @brief A boolean that states the Ctrl key was being holded.
         */
        bool m_hasCtrl;
        /**
         * @brief A boolean that states the Alt key was being holded.
         */
        bool m_hasAlt;
        /**
         * @brief A boolean that states the Shift key was being holded.
         */
        bool m_hasShift;
    };

    /**
     * @brief Represents the information of a terminal event.
     */
    class EventInfo final
    {
    public:
        /**
         * @brief Creates a new instance of the EventInfo class.
         * @param type The event type.
         * @returns An instance of the EventInfo class.
         */
        EventInfo(EventType type);
        /**
         * @brief Creates a new instance of the EventInfo class.
         * @param focusEvent A focus event to be converted.
         * @returns An instance of the EventInfo class.
         */
        EventInfo(FocusEvent focusEvent);
        /**
         * @brief Creates a new instance of the EventInfo class.
         * @param resizeEvent A resize event to be converted.
         * @returns An instance of the EventInfo class.
         */
        EventInfo(ResizeEvent resizeEvent);
        /**
         * @brief Creates a new instance of the EventInfo class.
         * @param mouseEvent A mouse event to be converted.
         * @returns An instance of the EventInfo class.
         */
        EventInfo(MouseEvent mouseEvent);
        /**
         * @brief Creates a new instance of the EventInfo class.
         * @param keyEvent A key event to be converted.
         * @returns An instance of the EventInfo class.
         */
        EventInfo(KeyEvent keyEvent);
        /**
         * @brief Gets the event type.
         * @returns The event type.
         */
        EventType GetType() const;
        /**
         * @brief Gets the focus event read.
         * @returns The focus event read.
         * @throws InvalidEventTypeException Thrown whenever the event type is not a focus event.
         */
        FocusEvent GetFocusEvent() const;
        /**
         * @brief Gets the resize event read.
         * @returns The resize event read.
         * @throws InvalidEventTypeException Thrown whenever the event type is not a resize event.
         */
        ResizeEvent GetResizeEvent() const;
        /**
         * @brief Gets the mouse event read.
         * @returns The mouse event read.
         * @throws InvalidEventTypeException Thrown whenever the event type is not a mouse event.
         */
        MouseEvent GetMouseEvent() const;
        /**
         * @brief Gets the key event read.
         * @returns The key event read.
         * @throws InvalidEventTypeException Thrown whenever the event type is not a key event.
         */
        KeyEvent GetKeyEvent() const;

    private:
        /**
         * @brief The event type.
         */
        EventType m_type;
        union {
            /**
             * @brief The focus event read.
             */
            FocusEvent m_focusEvent;
            /**
             * @brief The resize event read.
             */
            ResizeEvent m_resizeEvent;
            /**
             * @brief The mouse event read.
             */
            MouseEvent m_mouseEvent;
            /**
             * @brief The key event read.
             */
            KeyEvent m_keyEvent;
        };
    };

    /**
     * @brief Represents the terminal.
     */
    class Terminal final
    {
    public:
#ifdef _WIN32
        /**
         * @brief Represents the terminal encoding.
         */
        class Encoding final
        {
        public:
            /**
             * @brief Converts an UTF-16 encoded string to UTF-8.
             * @param utf16String The UTF-16 encoded string to be converted.
             * @returns The converted UTF-8 encoded string.
             */
            static std::string ConvertUTF16ToUTF8(std::wstring utf16String);
            /**
             * @brief Converts an UTF-8 encoded string to UTF-16.
             * @param utf8String The UTF-8 encoded string to be converted.
             * @returns The converted UTF-16 encoded string.
             */
            static std::wstring ConvertUTF8ToUTF16(std::string utf8String);

        private:
            Encoding() = delete;
        };
#endif

        /**
         * @brief Represents the standard input stream.
         */
        class Input final
        {
        public:
#ifdef _WIN32
            /**
             * @brief Gets the handle associated with the standard input stream.
             * @returns The handle associated with the standard input stream. 
             */
            static HANDLE GetHandle();
#endif
            /**
             * @brief Gets the file descriptor associated with the standard input stream.
             * @param The file descriptor associated with the standard input stream.
             */
            static std::FILE* GetFile();
            /**
             * @brief Gets the file descriptor number associated with the standard input stream.
             * @param The file descriptor number associated with the standard input stream.
             */
            static int GetFileNumber();
            /**
             * @brief Clears the standard input buffer.
             */
            static void Clear();
            /**
             * @brief Checks if the standard input stream is connected to an interactive terminal (TTY).
             * @param A boolean that states the standard input stream is a TTY.
             */
            static bool IsTTY();
            /**
             * @brief Reads a byte.
             * @returns The byte read or EOF if the standard input stream is closed or it is wide character oriented.
             */
            static char ReadByte();
            /**
             * @brief Reads a terminal event.
             * @param allowMouseCapture A boolean that states mouse events should be captured. If enabled, mouse selection will be disabled until it returns.
             * @returns The information about the event read.
             */
            static EventInfo ReadEvent(bool allowMouseCapture);
            /**
             * @brief Reads a terminal event.
             * @param allowMouseCapture A boolean that states mouse events should be captured. If enabled, mouse selection will be disabled until it returns.
             * @param waitInMilliseconds The time to wait for an event. If zero, it returns immediately.
             * @returns The information about the event read.
             */
            static EventInfo ReadEvent(bool allowMouseCapture, unsigned short waitInMilliseconds);
            /**
             * @brief Reads a terminal event.
             * @param allowMouseCapture A boolean that states mouse events should be captured. If enabled, mouse selection will be disabled until it returns.
             * @param waitInMilliseconds The time to wait for an event. If zero, it returns immediately.
             * @param filter A function used to filter events while the timer is running.
             * @returns The information about the event read.
             */
            static EventInfo ReadEvent(bool allowMouseCapture, unsigned short waitInMilliseconds, std::function<bool(EventInfo&)> filter);

        private:
            Input() = delete;
        };

        /**
         * @brief Represents the standard output stream.
         */
        class Output final
        {
        public:
            /**
             * @brief Flushes the standard output stream buffer.
             */
            static void Flush();
            /**
             * @brief Formats and writes arguments to the standard output stream with a newline character appended at its end.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param arguments The arguments to be formatted.
             * @throws WideCharacterOrientationException Thrown whenever the standard output stream is wide character oriented.
             */
            static void WriteLine(std::string format, std::va_list arguments);
            /**
             * @brief Formats and writes arguments to the standard output stream with a newline character appended at its end.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param ... The arguments to be formatted.
             * @throws WideCharacterOrientationException Thrown whenever the standard output stream is wide character oriented.
             */
            static void WriteLine(std::string format, ...);
            /**
             * @brief Writes a newline character to the standard output stream.
             * @throws WideCharacterOrientationException Thrown whenever the standard output stream is wide character oriented.
             */
            static void WriteLine();
            /**
             * @brief Formats and writes arguments to the standard output stream.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param arguments The arguments to be formatted.
             * @throws WideCharacterOrientationException Thrown whenever the standard output stream is wide character oriented.
             */
            static void Write(std::string format, std::va_list arguments);
            /**
             * @brief Formats and writes arguments to the standard output stream.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param ... The arguments to be formatted.
             * @throws WideCharacterOrientationException Thrown whenever the standard output stream is wide character oriented.
             */
            static void Write(std::string format, ...);
#ifdef _WIN32
            /**
             * @brief Gets the handle associated with the standard output stream.
             * @returns The handle associated with the standard output stream. 
             */
            static HANDLE GetHandle();
#endif
            /**
             * @brief Gets the file descriptor associated with the standard output stream.
             * @returns The file descriptor associated with the standard output stream.
             */
            static std::FILE* GetFile();
            /**
             * @brief Gets the file descriptor number associated with the standard output stream.
             * @returns The file descriptor number associated with the standard output stream.
             */
            static int GetFileNumber();
            /**
             * @brief Checks if the standard output stream is connected to an interactive terminal (TTY).
             * @returns A boolean that states the standard output stream is a TTY.
             */
            static bool IsTTY();

        private:
            Output() = delete;
        };

        /**
         * @brief Represents the standard error stream.
         */
        class Error final
        {
        public:
            /**
             * @brief Formats and writes arguments to the standard error stream with a newline character appended at its end.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param arguments The arguments to be formatted.
             * @throws WideCharacterOrientationException Thrown whenever the standard error stream is wide character oriented.
             */
            static void WriteLine(std::string format, std::va_list arguments);
            /**
             * @brief Formats and writes arguments to the standard error stream with a newline character appended at its end.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param ... The arguments to be formatted.
             * @throws WideCharacterOrientationException Thrown whenever the standard error stream is wide character oriented.
             */
            static void WriteLine(std::string format, ...);
            /**
             * @brief Writes a newline character to the standard error stream.
             * @throws WideCharacterOrientationException Thrown whenever the standard error stream is wide character oriented.
             */
            static void WriteLine();
            /**
             * @brief Formats and writes arguments to the standard error stream.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param arguments The arguments to be formatted.
             * @throws WideCharacterOrientationException Thrown whenever the standard error stream is wide character oriented.
             */
            static void Write(std::string format, std::va_list arguments);
            /**
             * @brief Formats and writes arguments to the standard error stream.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param ... The arguments to be formatted.
             * @throws WideCharacterOrientationException Thrown whenever the standard error stream is wide character oriented.
             */
            static void Write(std::string format, ...);
#ifdef _WIN32
            /**
             * @brief Gets the handle associated with the standard error stream.
             * @returns The handle associated with the standard error stream. 
             */
            static HANDLE GetHandle();
#endif
            /**
             * @brief Gets the file descriptor associated with the standard error stream.
             * @returns The file descriptor associated with the standard error stream.
             */
            static std::FILE* GetFile();
            /**
             * @brief Gets the file descriptor number associated with the standard error stream.
             * @returns The file descriptor number associated with the standard error stream.
             */
            static int GetFileNumber();
            /**
             * @brief Checks if the standard error stream is connected to an interactive terminal (TTY).
             * @returns A boolean that states the standard error stream is a TTY.
             */
            static bool IsTTY();

        private:
            Error() = delete;
        };

        /**
         * @brief Represents the terminal process.
         */
        class Process final
        {
        public:
            /**
             * @brief Gets and treats the command line arguments.
             * @param rawTotalArguments The total arguments given as first parameter of the main function.
             * @param rawArguments The arguments given as second parameter of the main function.
             * @returns The treated arguments.
             */
            static CMDArguments GetCMDArguments(int rawTotalCMDArguments, char** rawCMDArguments);
            /**
             * @brief Exits the process.
             * @param exitCode The exit code to be used. It must be a value in range from 0 to 255.
             * @throws OutOfRangeException Thrown whenever the exit code is out of the valid range.
             */
            static void Exit(int exitCode);
            /**
             * @brief Exits the process.
             * @param exitCode The exit code to be used.
             */
            static void Exit(ExitCode exitCode);

        private:
            Process() = delete;
        };

        /**
         * @brief Represents the terminal window.
         */
        class Window final
        {
        public:
            /**
             * @brief Gets the terminal window geometry.
             * @returns The terminal window geometry.
             */
            static Geometry GetGeometry();
            /**
             * @brief Opens the alternate window.
             */
            static void OpenAlternateWindow();
            /**
             * @brief Closes the alternate window.
             */
            static void CloseAlternateWindow();
            /**
             * @brief Sets the terminal window title.
             * @param title The title to be set.
             */
            static void SetTitle(std::string title);

        private:
            Window() = delete;
        };

        /**
         * @brief Represents the terminal bell.
         */
        class Bell final
        {
        public:
            /**
             * @brief Rings the terminal bell.
             */
            static void Ring();

        private:
            Bell() = delete;
        };

        /**
         * @brief Represents the terminal font.
         */
        class Font final
        {
        public:
            /**
             * @brief Sets the terminal font weight.
             * @param weight The weight to be set.
             */
            static void SetWeight(FontWeight weight);
            /**
             * @brief Sets an XTerm color into the terminal font.
             * @param color The ANSI code of the color.
             * @param layer The layer of the font to be affected.
             */
            static void SetXColor(unsigned char color, FontLayer layer);
            /**
             * @brief Sets an XTerm color into the terminal font.
             * @param color The ANSI code of the color.
             * @param layer The layer of the font to be affected.
             */
            static void SetXColor(XColor color, FontLayer layer);
            /**
             * @brief Sets an RGB color into the terminal font.
             * @param red The red component of the color.
             * @param green The green component of the color.
             * @param blue The blue component of the color.
             * @param layer The layer of the font to be affected.
             */
            static void SetRGBColor(unsigned char red, unsigned char green, unsigned char blue, FontLayer layer);
            /**
             * @brief Sets an RGB color into the terminal font.
             * @param color The color to be applied.
             * @param layer The layer of the font to be affected.
             */
            static void SetRGBColor(RGBColor color, FontLayer layer);
            /**
             * @brief Sets a hex color into the terminal font.
             * @param color The hex code of the color.
             * @param layer The layer of the font to be affected.
             */
            static void SetHexColor(unsigned int hex, FontLayer layer);
            /**
             * @brief Sets a hex color into the terminal font.
             * @param color The color to be applied.
             * @param layer The layer of the font to be affected.
             */
            static void SetHexColor(HexColor color, FontLayer layer);
            /**
             * @brief Sets the terminal effects flagged in a bitmask.
             * @param effect The bitmask containing the effects. It must be composed by enumerators from the FontEffect enum class.
             * @throws OutOfRangeException Thrown whenever an invalid effect is used to compose the bitmask.
             */
            static void SetEffects(int effect);
            /**
             * @brief Sets a terminal effect.
             * @param effect The effect to be set.
             */
            static void SetEffects(FontEffect effect);
            /**
             * @brief Resets the terminal font colors.
             */
            static void ResetColors();
            /**
             * @brief Resets the terminal font weight.
             */
            static void ResetWeight();
            /**
             * @brief Resets the terminal effects.
             */
            static void ResetEffects();

        private:
            Font() = delete;
        };

        /**
         * @brief Represents the terminal cursor.
         */
        class Cursor final
        {
        public:
            /**
             * @brief Gets the terminal cursor coordinate. On Linux, as it parses a terminal answer given through the standard input stream, its buffer will always be cleared.
             * @returns The terminal cursor coordinate.
             * @throws NoValidTTYException Thrown, on Windows, whenever the standard output and error streams are not TTY; and, on Linux, whenever the standard input or the
             * standard output and error streams are not TTY.
             */
            static Coordinate GetCoordinate();
            /**
             * @brief Sets the terminal cursor coordinate.
             * @param column The column component of the coordinate.
             * @param row The row component of the coordinate.
             * @throws OutOfRangeException Thrown whenever the coordinate is outside of the terminal window boundaries.
             */
            static void SetCoordinate(unsigned short column, unsigned short row);
            /**
             * @brief Sets the terminal cursor coordinate.
             * @param coordinate The coordinate to be set.
             * @throws OutOfRangeException Thrown whenever the coordinate is outside of the terminal window boundaries.
             */
            static void SetCoordinate(Coordinate coordinate);
            /**
             * @brief Sets the terminal cursor shape.
             * @param shape The shape to be set.
             */
            static void SetShape(CursorShape shape, bool isBlinking);
            /**
             * @brief Sets the terminal cursor visibility.
             * @param isVisible A boolean that states the cursor should be visible.
             */
            static void SetVisibility(bool isVisible);
            /**
             * @brief Resets the terminal cursor shape.
             */
            static void ResetShape();
            /**
             * @brief Clears the terminal cursor line.
             */
            static void ClearLine();

        private:
            Cursor() = delete;
        };

    private:
        Terminal() = delete;
    };

    /**
     * @brief Concatenates two effects together in order to create a bitmask.
     * @param effectI The first effect.
     * @param effectII The second effect.
     * @returns A bitmask containing both effects.
     */
    int operator|(FontEffect effectI, FontEffect effectII);
    /**
     * @brief Checks if a code and a keyboard key are equal.
     * @param A boolean that states both are equal.
     */
    bool operator==(int code, KeyboardKey key);
    /**
     * @brief Checks if a keyboard key and a code are equal.
     * @param A boolean that states both are equal.
     */
    bool operator==(KeyboardKey key, int code);
    /**
     * @brief Checks if a code and a keyboard key are different.
     * @param A boolean that states both are different.
     */
    bool operator!=(int code, KeyboardKey key);
    /**
     * @brief Checks if a keyboard key and a code are different.
     * @param A boolean that states both are different.
     */
    bool operator!=(KeyboardKey key, int code);
}
