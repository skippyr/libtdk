#pragma once

#include <string>

/**
 * @brief A powerful, basic and open-source C++ terminal manipulation library for Windows and Linux that includes
 * features the standard library lacks to handle terminal properties, colors,effects, arguments and event readings
 * using UTF-8 encoding.
 */
namespace TMK
{
    /** @brief Contains the available t erminal font effects. */
    enum class Effect
    {
        /** @brief Makes the text curly. */
        Italic = 1 << 3,
        /** @brief Draws a horizontal line below the text. */
        Underline = 1 << 4,
        /** @brief Makes the text blink indefinitely. */
        Blinking = 1 << 5,
        /** @brief Swaps the foreground and background colors. */
        Negative = 1 << 7,
        /** @brief Makes the text hard to see or invisible. */
        Hidden = 1 << 8,
        /** @brief Draws a horizontal line crossing through the middle of the text. */
        CrossedOut = 1 << 9
    };

    /** @brief Contains the available layers where a terminal color can be set on the font. */
    enum class Layer
    {
        /** @brief Affects the color of the letters. */
        Foreground = 3,
        /** @brief Affects the background color of the letters. */
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
