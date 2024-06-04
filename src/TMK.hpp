#pragma once

#include <cstdio>

/// <summary>
/// A powerful, basic and open-source C++ terminal manipulation for Windows and Linux that includes features to handle
/// terminal properties, colors, effects, arguments and event readings with UTF-8 encoding.
/// </summary>
namespace TMK
{
    enum class Effect
    {
        Italic = 1 << 3,
        Underline = 1 << 4,
        Blinking = 1 << 5,
        Negative = 1 << 7,
        Hidden = 1 << 8,
        CrossedOut = 1 << 9
    };

    enum class Layer
    {
        Foreground = 3,
        Background
    };

    enum class Weight
    {
        Default,
        Bold,
        Dim
    };

    enum class XColor
    {
        Default = -1,
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

    class Input final
    {
    public:
        Input() = delete;
        static bool IsTTY();
        static void ClearBuffer();
        static std::FILE* GetFile();
        static int GetFileNo();
    };

    class Output final
    {
    public:
        Output() = delete;
        static int WriteLine(const char* format, ...);
        static int WriteLine();
        static int Write(const char* format, ...);
        static bool IsTTY();
        static std::FILE* GetFile();
        static int GetFileNo();
    };

    class Error final
    {
    public:
        Error() = delete;
        static int WriteLine(const char* format, ...);
        static int WriteLine();
        static int Write(const char* format, ...);
        static bool IsTTY();
        static std::FILE* GetFile();
        static int GetFileNo();
    };

    class Arguments final
    {
    public:
        Arguments(int totalArguments, char** arguments);
        ~Arguments();
        int GetTotalArguments() const;
        const char* GetArgumentByOffset(std::size_t offset) const;

    private:
        int m_totalArguments;
        char** m_arguments;
    };

    class Dimensions final
    {
    public:
        Dimensions();
        Dimensions(unsigned short totalColumns, unsigned short totalRows);
        unsigned short GetTotalColumns() const;
        unsigned short GetTotalRows() const;

    private:
        unsigned short m_totalColumns;
        unsigned short m_totalRows;
    };

    class Environment final
    {
    public:
        Environment() = delete;
        static Arguments GetArguments(int totalArguments, char** rawArguments);
    };

    class Font final
    {
    public:
        Font() = delete;
        static void SetWeight(Weight weight);
        static void SetEffect(Effect effect, bool isToEnable);
        static void SetEffect(int effect, bool isToEnable);
        static void SetXColor(XColor color, Layer layer);
        static void SetXColor(int color, Layer layer);
        static void SetHexColor(int hex, Layer layer);
        static void SetRGBColor(int red, int green, int blue, Layer layer);
    };

    class Window final
    {
    public:
        Window() = delete;
        static int GetDimensions(Dimensions& dimensions);
        static void SetTitle(const char* title);
        static void OpenAlternateBuffer();
        static void CloseAlternateBuffer();
        static void Clear();
    };

    int operator|(Effect effect0, Effect effect1);
}
