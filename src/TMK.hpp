#pragma once

#include <cstdarg>
#include <functional>
#include <iostream>

/**
 * @brief A blazing and open-source C++ terminal manipulation library for Windows and Linux. It is capable of handling terminal attributes, colors, effects,
 * arguments and event readings targetting UTF-8 and UTF-16 encodings. It has a an object oriented philosophy for those who love a touch of complexity.
 */
namespace TMK
{
    /** @brief Contains the available terminal font weights. */
    enum class FontWeight
    {
        /** @brief Usually rendered as bold weight and/or with bright colors. */
        Bold = 1,
        /** @brief Usually rendered with faded colors. */
        Dim
    };

    /** @brief Contains the available terminal font layers. */
    enum class FontLayer
    {
        /** @brief Refers to the letters. */
        Foreground = 3,
        /** @brief Refers to the background of the letters. */
        Background
    };

    /** @brief Contains the available terminal cursor shapes. */
    enum class CursorShape
    {
        /** @brief Fills the whole character cell. */
        Block = 2,
        /** @brief Fills a small region at the bottom of the character cell. */
        Underline = 4,
        /** @brief Fills a small region at the left of the character cell. */
        Bar = 6
    };

    /** @brief Contains the ANSI codes of the first 16 colors of the XTerm color palette. */
    enum class XColor
    {
        /** @brief The dark variant of the black color. */
        DarkBlack,
        /** @brief The dark variant of the red color. */
        DarkRed,
        /** @brief The dark variant of the green color. */
        DarkGreen,
        /** @brief The dark variant of the yellow color. */
        DarkYellow,
        /** @brief The dark variant of the blue color. */
        DarkBlue,
        /** @brief The dark variant of the magenta color. */
        DarkMagenta,
        /** @brief The dark variant of the cyan color. */
        DarkCyan,
        /** @brief The dark variant of the white color. */
        DarkWhite,
        /** @brief The light variant of the black color. */
        LightBlack,
        /** @brief The light variant of the red color. */
        LightRed,
        /** @brief The light variant of the green color. */
        LightGreen,
        /** @brief The light variant of the yellow color. */
        LightYellow,
        /** @brief The light variant of the blue color. */
        LightBlue,
        /** @brief The light variant of the magenta color. */
        LightMagenta,
        /** @brief The light variant of the cyan color. */
        LightCyan,
        /** @brief The light variant of the white color. */
        LightWhite
    };

    /** @brief Represents the available terminal effects. */
    enum class Effect
    {
        /** @brief Makes the text curly. */
        Italic = 1 << 3,
        /** @brief Draws a horizontal line crossing below the text. */
        Underlined = 1 << 4,
        /** @brief Makes the text blink indefinitely. */
        Blinking = 1 << 5,
        /** @brief Swaps the background and foreground colors. */
        Negative = 1 << 7,
        /** @brief Makes the text hard to see or invisible. */
        Hidden = 1 << 8,
        /** @brief Draws a horizontal line crossing through the middle of the text. */
        CrossedOut = 1 << 9
    };

    /** @brief Represents the available terminal event types. */
    enum class EventType
    {
        /** @brief No event is available. */
        None,
        /** @brief The event timer ran out. */
        TimeOut,
        /** @brief The terminal window was resized. */
        Resize,
        /** @brief The terminal window gained/lost focus. */
        Focus,
        /** @brief The mouse was moved or clicked. */
        Mouse,
        /** @brief A keyboard key was pressed. */
        Key
    };

    /** @brief Contains the available mouse buttons. */
    enum class MouseButton
    {
        /** @brief No button. */
        None,
        /** @brief Left mouse button. */
        Left,
        /** @brief Mouse wheel button. */
        Wheel,
        /** @brief Mouse wheel scroll upwards. */
        WheelUp,
        /** @brief Mouse wheel scroll downwards. */
        WheelDown,
        /** @brief Right mouse button. */
        Right
    };

    class RGBColor;

    /** @brief Represents an exception thrown whenever a group of streams are wide character oriented. */
    class WideCharacterOrientationException final
    {
    };

    /** @brief Represents an exception thrown whenever a group of streams are being redirected/piped. */
    class NoValidTTYException final
    {
    };

    /** @brief Represents an exception thrown whenever a value is out of a certain range. */
    class OutOfRangeException final
    {
    };

    /** @brief Represents an exception thrown whenever an invalid event type data tries to be read. */
    class InvalidEventTypeException final
    {
    };

    /** @brief Represents the command line arguments in both UTF-8 and UTF-16 encodings. */
    class Arguments final
    {
    public:
#ifdef _WIN32
        /**
         * @brief Creates a new instance of the Arguments class. It is only available on Windows.
         * @param totalArguments The total arguments.
         * @param utf8Arguments The arguments in UTF-8 encoding.
         * @param utf16Arguments The arguments in UTF-16 encoding.
         */
        Arguments(int totalArguments, char** utf8Arguments, wchar_t** utf16Arguments);
        /**
         * @brief Gets an argument in UTF-16 encoding by using its offset. It is only available on Windows.
         * @param offset The offset to be used.
         * @returns The argument in UTF-16 encoding.
         * @exception OutOfRangeException Gets throws whenever the offset is out of the valid range of arguments.
         */
        std::wstring GetUTF16ArgumentByOffset(std::size_t offset) const;
#else
        /**
         * @brief Creates a new instance of the Arguments class. It is only available on Linux.
         * @param totalArguments The total arguments.
         * @param utf8Arguments The arguments in UTF8 encoding.
         */
        Arguments(int totalArguments, char** utf8Arguments);
#endif
        /** @brief Destroys an instance of the Arguments class. */
        ~Arguments();
        /**
         * @brief Gets the total arguments.
         * @returns The total arguments.
         */
        int GetTotalArguments() const;
        /**
         * @brief Gets an argument in UTF-8 encoding by using its offset.
         * @param offset The offset to be used.
         * @returns The argument in UTF-8 encoding.
         * @throws OutOfRangeException Gets thrown whenever the offset is out of the valid range of arguments.
         */
        std::string GetUTF8ArgumentByOffset(std::size_t offset) const;

    private:
#ifdef _WIN32
        /** @brief The arguments in UTF-16 encoding. It is only available on Windows. */
        wchar_t** m_utf16Arguments;
#endif
        /** @brief The arguments in UTF-8 encoding. */
        char** m_utf8Arguments;
        /** @brief The total arguments. */
        int m_totalArguments;
    };

    /** @brief Represents terminal dimensions. */
    class Dimensions final
    {
    public:
        /** @brief Creates an instance of the Dimensions class. */
        Dimensions();
        /**
         * @brief Creates an instance of the Dimensions class.
         * @param totalColumns The total columns of the dimensions.
         * @param totalRows The total rows of the dimensions.
         */
        Dimensions(unsigned short totalColumns, unsigned short totalRows);
        /**
         * @brief Gets the total columns of the dimensions.
         * @returns The total columns of the dimensions.
         */
        unsigned short GetTotalColumns() const;
        /**
         * @brief Gets the total rows of the dimensions.
         * @returns The total rows of the dimensions.
         */
        unsigned short GetTotalRows() const;

    private:
        /** @brief The total columns of the dimensions. */
        unsigned short m_totalColumns;
        /** @brief The total rows of the dimensions. */
        unsigned short m_totalRows;
    };

    /** @brief Represents a color in hex format. */
    class HexColor final
    {
    public:
        /**
         * @brief Creates an instance of the HexColor class.
         * @param The hex code of the color. It must be a value in range from 0x0 to 0xffffff.
         * @throws OutOfRangeException Gets thrown whenever the hex code is out of the valid range.
         */
        HexColor(unsigned int code);
        /**
         * @brief Creates an instance of the HexColor class.
         * @param A color in RGB format to be converted.
         */
        HexColor(RGBColor color);

        /** @brief Gets the hex code of the color as a number. */
        unsigned int GetCodeAsNumber() const;

    private:
        /** @brief The hex code of the color. */
        unsigned int m_code;
    };

    /** @brief Represents a color in RGB format. */
    class RGBColor final
    {
    public:
        /**
         * @brief Creates an instance of the RGBColor class.
         * @param red The red component of the color.
         * @param green The green component of the color.
         * @param blue The blue component of the color.
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
        /** @brief The red component of the color. */
        unsigned char m_red;
        /** @brief The green component of the color. */
        unsigned char m_green;
        /** @brief The blue component of the color. */
        unsigned char m_blue;
    };

    /** @brief Represents a terminal coordinate. */
    class Coordinate final
    {
    public:
        /** @brief Creates a new instance of the Coordinate class. */
        Coordinate();
        /**
         * @brief Creates a new instance of the Coordinate class.
         * @param column The column component of the coordinate.
         * @param row The row component of the coordinate.
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

    private:
        /** @brief The column component of the coordinate. */
        unsigned short m_column;
        /** @brief The row component of the coordinate. */
        unsigned short m_row;
    };

    /** @brief Represents a terminal focus event. */
    class FocusEvent final
    {
    public:
        /**
         * @brief Creates a new instance of the FocusEvent class.
         * @param hasFocus A boolean that states the terminal window has gained focus.
         */
        FocusEvent(bool hasFocus);
        /**
         * @brief Checks if the terminal window has gained focus.
         * @returns A boolean that states the terminal window has gained focus.
         */
        bool HasFocus() const;

    private:
        /** @brief A boolean that states the terminal window has gained focus. */
        bool m_hasFocus;
    };

    /** @brief Represents a terminal window resize event. */
    class ResizeEvent final
    {
    public:
        /** @brief Creates a new instance of the ResizeEvent class. */
        ResizeEvent();
        /**
         * @brief Gets the terminal window dimensions.
         * @returns The terminal window dimensions.
         */
        Dimensions GetDimensions() const;

    private:
        /** @brief The terminal window dimensions. */
        Dimensions m_dimensions;
    };

    /** @brief Represents a terminal mouse event. */
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
        /** @brief The mouse coordinate. */
        Coordinate m_coordinate;
        /** @brief The mouse button pressed. */
        MouseButton m_button;
        /** @brief A boolean that states the mouse was being dragged. */
        bool m_isDragging;
        /** @brief A boolean that states the Ctrl key was being holded. */
        bool m_hasCtrl;
        /** @brief A boolean that states the Alt key was being holded. */
        bool m_hasAlt;
        /** @brief A boolean that states the Shift key was being holded. */
        bool m_hasShift;
    };

    /** @brief Represents the information of a terminal event. */
    class EventInfo final
    {
    public:
        /**
         * @brief Creates a new instance of the EventInfo class.
         * @param type The event type.
         */
        EventInfo(EventType type);
        /**
         * @brief Creates a new instance of the EventInfo class.
         * @param focusEvent A focus event to be converted.
         */
        EventInfo(FocusEvent focusEvent);
        /**
         * @brief Creates a new instance of the EventInfo class.
         * @param resizeEvent A resize event to be converted.
         */
        EventInfo(ResizeEvent resizeEvent);
        /**
         * @brief Creates a new instance of the EventInfo class.
         * @param mouseEvent A mouse event to be converted.
         */
        EventInfo(MouseEvent mouseEvent);
        /**
         * @brief Gets the event type.
         * @returns The event type.
         */
        EventType GetType() const;
        /**
         * @brief Gets the focus event read.
         * @returns The focus event read.
         * @throws InvalidEventTypeException Gets thrown whenever the event type is not a focus event.
         */
        FocusEvent GetFocusEvent() const;
        /**
         * @brief Gets the resize event read.
         * @returns The resize event read.
         * @throws InvalidEventTypeException Gets thrown whenever the event type is not a resize event.
         */
        ResizeEvent GetResizeEvent() const;
        /**
         * @brief Gets the mouse event read.
         * @returns The mouse event read.
         * @throws InvalidEventTypeException Gets thrown whenever the event type is not a mouse event.
         */
        MouseEvent GetMouseEvent() const;

    private:
        /** @brief The event type. */
        EventType m_type;
        union {
            /** @brief The focus event read. */
            FocusEvent m_focusEvent;
            /** @brief The resize event read. */
            ResizeEvent m_resizeEvent;
            /** @brief The mouse event read. */
            MouseEvent m_mouseEvent;
        };
    };

    /** @brief Represents the terminal. */
    class Terminal final
    {
    public:
#ifdef _WIN32
        /** @brief Represents the terminal encoding converter. It is only available on Windows. */
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

        /** @brief Represents the standard input stream. */
        class Input final
        {
        public:
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

        /** @brief Represents the standard output stream. */
        class Output final
        {
        public:
            /** @brief Flushes the stream buffer. */
            static void Flush();
            /**
             * @brief Formats and writes arguments to the standard output stream with a newline character appended at its end.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param ... The arguments to be formatted.
             * @throws WideCharacterOrientationException Gets thrown whenever the standard output stream is wide character oriented.
             */
            static void WriteLine(std::string format, ...);
            /**
             * @brief Writes a newline character to the standard output stream.
             * @throws WideCharacterOrientationException Gets thrown whenever the standard output stream is wide character oriented.
             */
            static void WriteLine();
            /**
             * @brief Formats and writes arguments to the standard output stream.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param ... The arguments to be formatted.
             * @throws WideCharacterOrientationException Gets thrown whenever the standard output stream is wide character oriented.
             */
            static void Write(std::string format, ...);
            /**
             * @brief Gets the file descriptor associated with the standard output stream.
             * @param The file descriptor associated with the standard output stream.
             */
            static std::FILE* GetFile();
            /**
             * @brief Gets the file descriptor number associated with the standard output stream.
             * @param The file descriptor number associated with the standard output stream.
             */
            static int GetFileNumber();
            /**
             * @brief Checks if the standard output stream is connected to an interactive terminal (TTY).
             * @param A boolean that states the standard output stream is a TTY.
             */
            static bool IsTTY();

        private:
            Output() = delete;
        };

        /** @brief Represents the standard error stream. */
        class Error final
        {
        public:
            /**
             * @brief Formats and writes arguments to the standard error stream with a newline character appended at its end.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param ... The arguments to be formatted.
             * @throws WideCharacterOrientationException Gets thrown whenever the standard error stream is wide character oriented.
             */
            static void WriteLine(std::string format, ...);
            /**
             * @brief Writes a newline character to the standard error stream.
             * @throws WideCharacterOrientationException Gets thrown whenever the standard error stream is wide character oriented.
             */
            static void WriteLine();
            /**
             * @brief Formats and writes arguments to the standard error stream.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param ... The arguments to be formatted.
             * @throws WideCharacterOrientationException Gets thrown whenever the standard error stream is wide character oriented.
             */
            static void Write(std::string format, ...);
            /**
             * @brief Gets the file descriptor associated with the standard error stream.
             * @param The file descriptor associated with the standard error stream.
             */
            static std::FILE* GetFile();
            /**
             * @brief Gets the file descriptor number associated with the standard error stream.
             * @param The file descriptor number associated with the standard error stream.
             */
            static int GetFileNumber();
            /**
             * @brief Checks if the standard error stream is connected to an interactive terminal (TTY).
             * @param A boolean that states the standard error stream is a TTY.
             */
            static bool IsTTY();

        private:
            Error() = delete;
        };

        /** @brief Represents the terminal process. */
        class Process final
        {
        public:
            /**
             * @brief Gets and treats the command line arguments.
             * @param rawTotalArguments The total arguments given as first parameter of the main function.
             * @param rawArguments The arguments given as second parameter of the main function.
             * @returns The treated arguments.
             */
            static Arguments GetArguments(int rawTotalArguments, char** rawArguments);
            /**
             * @brief Exits the process.
             * @param exitCode The exit code to be used.
             */
            static void Exit(int exitCode);

        private:
            Process() = delete;
        };

        /** @brief Represents the terminal window. */
        class Window final
        {
        public:
            /**
             * @brief Gets the terminal window dimensions.
             * @returns The terminal window dimensions.
             * @exception NoValidTTYException Gets thrown whenever the standard output and error streams are being redirected/piped.
             */
            static Dimensions GetDimensions();
            /** @brief Opens the alternate window. */
            static void OpenAlternateWindow();
            /** @brief Closes the alternate window. */
            static void CloseAlternateWindow();
            /**
             * @brief Sets the terminal window title.
             * @param title The title to be set.
             */
            static void SetTitle(std::string title);

        private:
            Window() = delete;
        };

        /** @brief Represents the terminal bell. */
        class Bell final
        {
        public:
            /** @brief Rings the terminal bell. */
            static void Ring();

        private:
            Bell() = delete;
        };

        /** @brief Represents the terminal font. */
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
             * @param effect The bitmask containing the effects. It must be composed by value inside of the Effect enum.
             * @throws OutOfRangeException Gets thrown whenever an invalid effect is used to compose the bitmask.
             */
            static void SetEffect(int effect);
            /**
             * @brief Sets a terminal effect.
             * @param effect The effect to be set.
             */
            static void SetEffect(Effect effect);
            /** @brief Resets the terminal font colors. */
            static void ResetColors();
            /** @brief Resets the terminal font weight. */
            static void ResetWeight();
            /** @brief Resets the terminal effects. */
            static void ResetEffects();

        private:
            Font() = delete;
        };

        /** @brief Represents the terminal cursor. */
        class Cursor final
        {
        public:
            /**
             * @brief Gets the terminal cursor coordinate. On Linux, as it parses a terminal answer given through the standard input stream, its buffer will
             * always be cleared.
             * @returns The terminal cursor coordinate.
             * @throws NoValidTTYException Gets thrown, on Windows, whenever the standard output and error streams are being redirected/piped; and, on Linux,
             * whenever the standard input or the standard output and error streams are being redirected/piped.
             */
            static Coordinate GetCoordinate();
            /**
             * @brief Sets the terminal cursor coordinate.
             * @param column The column component of the coordinate.
             * @param row The row component of the coordinate.
             * @throws OutOfRangeException Gets thrown whenever the coordinate given is outside of the terminal window boundaries.
             */
            static void SetCoordinate(unsigned short column, unsigned short row);
            /**
             * @brief Sets the terminal cursor coordinate.
             * @param coordinate The coordinate to be set.
             * @throws OutOfRangeException Gets thrown whenever the coordinate given is outside of the terminal window boundaries.
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
            /** @brief Resets the terminal cursor shape. */
            static void ResetShape();
            /** @brief Clears the terminal cursor line. */
            static void ClearLine();

        private:
            Cursor() = delete;
        };

    private:
        Terminal() = delete;
    };

    /**
     * @brief Concatenates two effects together in order to create a bitmask.
     * @param effect0 The first effect.
     * @param effect1 The second effect.
     * @returns A bitmask containing both effects.
     */
    int operator|(Effect effect0, Effect effect1);
}
