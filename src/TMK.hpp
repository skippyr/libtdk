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

    class Arguments
    {
    public:
#ifdef _WIN32
        Arguments(int totalArguments, char** utf8Arguments, wchar_t** utf16Arguments);
        std::wstring GetUTF16ArgumentByOffset(std::size_t offset) const;
#endif
        Arguments(int totalArguments, char** utf8Arguments);
        ~Arguments();
        int GetTotalArguments() const;
        std::string GetUTF8ArgumentByOffset(std::size_t offset) const;

    private:
#ifdef _WIN32
        wchar_t** m_utf16Arguments;
#endif
        char** m_utf8Arguments;
        int m_totalArguments;
    };

    class Dimensions
    {
    public:
        Dimensions();
        Dimensions(unsigned short width, unsigned short height);
        unsigned short GetWidth() const;
        unsigned short GetHeight() const;

    private:
        unsigned short m_width;
        unsigned short m_height;
    };

    class HexColor
    {
    public:
        HexColor(unsigned int code);
        HexColor(RGBColor color);
        unsigned int GetCode() const;

    private:
        unsigned int m_code;
    };

    class RGBColor
    {
    public:
        RGBColor(unsigned char red, unsigned char green, unsigned char blue);
        RGBColor(HexColor color);
        unsigned char GetRed() const;
        unsigned char GetGreen() const;
        unsigned char GetBlue() const;

    private:
        unsigned char m_red;
        unsigned char m_green;
        unsigned char m_blue;
    };

    class NoValidTTYException
    {
    };

    class Terminal
    {
    public:
        class Input
        {
        public:
            static std::FILE* GetFile();
            static int GetFileNumber();
            static bool IsTTY();
            static char ReadByte();

        private:
            Input() = delete;
        };

        class Output
        {
        public:
            static void Flush();
            static int WriteLine(std::string format, ...);
            static int WriteLine();
            static int Write(std::string format, ...);
            static std::FILE* GetFile();
            static int GetFileNumber();
            static bool IsTTY();

        private:
            Output() = delete;
        };

        class Error
        {
        public:
            static int WriteLine(std::string format, ...);
            static int WriteLine();
            static int Write(std::string format, ...);
            static std::FILE* GetFile();
            static int GetFileNumber();
            static bool IsTTY();

        private:
            Error() = delete;
        };

        class Process
        {
        public:
            static Arguments GetArguments(int rawTotalArguments, char** rawArguments);
            static void Exit(int exitCode);

        private:
            Process() = delete;
        };

        class Window
        {
        public:
            /**
             * @brief Gets the terminal window dimensions.
             * @returns The terminal window dimensions.
             * @exception NoValidTTYException If no standard stream connected to a terminal window is found.
             */
            static Dimensions GetDimensions();
            static void OpenAlternateWindow();
            static void CloseAlternateWindow();

        private:
            Window() = delete;
        };

        class Bell
        {
        public:
            static void Ring();

        private:
            Bell() = delete;
        };

        class Font
        {
        public:
            static void SetWeight(FontWeight weight);
            static void SetXColor(unsigned char color, FontLayer layer);
            static void SetXColor(XColor color, FontLayer layer);
            static void SetRGBColor(unsigned char red, unsigned char green, unsigned char blue, FontLayer layer);
            static void SetRGBColor(RGBColor color, FontLayer layer);
            static void SetHexColor(unsigned int hex, FontLayer layer);
            static void SetHexColor(HexColor color, FontLayer layer);
            static void ResetColors();
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
