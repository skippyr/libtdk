#pragma once

#include <string>

/// <summary>
/// A powerful, basic and open-source C++ terminal manipulation for Windows and Linux that includes features missing
/// from the standard library to handle terminal properties, colors, effects, arguments and event readings with UTF-8
/// encoding.
/// </summary>
namespace TMK
{
    /// <summary>Contains the available terminal font effects.</summary>
    enum class Effect
    {
        /// <summary>Makes the text curly.</summary>
        Italic = 1 << 3,
        /// <summary>Draws a horizontal line below the text.</summary>
        Underline = 1 << 4,
        /// <summary>Makes the text blink indefinitely.</summary>
        Blinking = 1 << 5,
        /// <summary>Swaps the foreground and background colors.</summary>
        Negative = 1 << 7,
        /// <summary>Makes the text hard to see or invisible.</summary>
        Hidden = 1 << 8,
        /// <summary>Draws a horizontal line crossing through the middle of the text.</summary>
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
        static int WriteLine(std::string format, ...);
        static int WriteLine();
        static int Write(std::string format, ...);
        static bool IsTTY();
        static std::FILE* GetFile();
        static int GetFileNo();
    };

    class Error final
    {
    public:
        Error() = delete;
        static int WriteLine(std::string format, ...);
        static int WriteLine();
        static int Write(std::string format, ...);
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
        static void SetTitle(std::string title);
        static void OpenAlternateBuffer();
        static void CloseAlternateBuffer();
        static void Clear();
    };

    int operator|(Effect effect0, Effect effect1);
}
