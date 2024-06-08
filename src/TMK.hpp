#pragma once

#include <iostream>

/**
 * @brief A powerful and open-source C++ terminal manipulation library for Windows and Linux. It is capable of handling
 * terminal attributes, colors, effects, arguments and event readings targetting UTF-8 and UTF-16 encodings.
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

    class RGBColor;

    /** @brief Represents an exception thrown whenever a group of streams are wide character oriented. */
    class WideCharacterOrientationException
    {
    };

    /** @brief Represents an exception thrown whenever a group of streams do not match a TTY criteria. */
    class NoValidTTYException
    {
    };

    /** @brief Represents an exception thrown whenever a value is out of range. */
    class OutOfRangeException
    {
    };

    /** @brief Represents the command line arguments in both UTF-8 and UTF-16 encodings. */
    class Arguments
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
    class Dimensions
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
    class HexColor
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
    class RGBColor
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

    /** @brief Represents the terminal. */
    class Terminal
    {
    public:
        /** @brief Represents the standard input stream. */
        class Input
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

        private:
            Input() = delete;
        };

        /** @brief Represents the standard output stream. */
        class Output
        {
        public:
            /** @brief Flushes the stream buffer. */
            static void Flush();
            /**
             * @brief Formats and writes arguments to the standard output stream with a newline character appended at
             * its end.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param ... The arguments to be formatted.
             * @throws WideCharacterOrientationException Gets thrown whenever the standard output stream is wide
             * character oriented.
             */
            static void WriteLine(std::string format, ...);
            /**
             * @brief Writes a newline character to the standard output stream.
             * @throws WideCharacterOrientationException Gets thrown whenever the standard output stream is wide
             * character oriented.
             */
            static void WriteLine();
            /**
             * @brief Formats and writes arguments to the standard output stream.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param ... The arguments to be formatted.
             * @throws WideCharacterOrientationException Gets thrown whenever the standard output stream is wide
             * character oriented.
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
        class Error
        {
        public:
            /**
             * @brief Formats and writes arguments to the standard error stream with a newline character appended at its
             * end.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param ... The arguments to be formatted.
             * @throws WideCharacterOrientationException Gets thrown whenever the standard error stream is wide
             * character oriented.
             */
            static void WriteLine(std::string format, ...);
            /**
             * @brief Writes a newline character to the standard error stream.
             * @throws WideCharacterOrientationException Gets thrown whenever the standard error stream is wide
             * character oriented.
             */
            static void WriteLine();
            /**
             * @brief Formats and writes arguments to the standard error stream.
             * @param format The format to be used. It accepts the same specifiers as the printf function family.
             * @param ... The arguments to be formatted.
             * @throws WideCharacterOrientationException Gets thrown whenever the standard error stream is wide
             * character oriented.
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
        class Process
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
        class Window
        {
        public:
            /**
             * @brief Gets the terminal window dimensions.
             * @returns The terminal window dimensions.
             * @exception NoValidTTYException Gets thrown whenever no valid standard stream is a TTY.
             */
            static Dimensions GetDimensions();
            /** @brief Opens the alternate window. */
            static void OpenAlternateWindow();
            /** @brief Closes the alternate window. */
            static void CloseAlternateWindow();

        private:
            Window() = delete;
        };

        /** @brief Represents the terminal bell. */
        class Bell
        {
        public:
            /** @brief Rings the terminal bell. */
            static void Ring();

        private:
            Bell() = delete;
        };

        /** @brief Represents the terminal font. */
        class Font
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
            /** @brief Resets the terminal font colors. */
            static void ResetColors();
            /** @brief Resets the terminal font weight. */
            static void ResetWeight();

        private:
            Font() = delete;
        };

        class Cursor
        {
        public:
            static void SetShape(CursorShape shape, bool isBlinking);
            static void ResetShape();

        private:
            Cursor() = delete;
        };

    private:
        Terminal() = delete;
    };
}
