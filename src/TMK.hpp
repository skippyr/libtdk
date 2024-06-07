#pragma once

#include <iostream>

namespace TMK
{
    enum class FontWeight
    {
        Bold = 1,
        Dim
    };

    enum class FontLayer
    {
        Foreground = 3,
        Background
    };

    enum class CursorShape
    {
        Block = 2,
        Underline = 4,
        Bar = 6
    };

    enum class XColor
    {
        Black,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        White,
        LightBlack,
        LightRed,
        LightGreen,
        LightYellow,
        LightBlue,
        LightMagenta,
        LightCyan,
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

    class Terminal
    {
    public:
        class Input
        {
        public:
            static std::FILE* GetFile();
            static int GetFileNumber();
            static bool IsTTY();

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
            static int GetDimensions(Dimensions& dimensions);

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
