#pragma once

#include <string>

/**
 * @brief A powerful, basic and open-source C++ terminal manipulation library for Windows and Linux that includes
 * features the standard library lacks to handle terminal properties, colors,effects, arguments and event readings
 * using UTF-8 encoding. It is heavily inspired by Java and the standard C# console library, thus having an object
 * oriented philosophy for those who like a touch of complexity.
 */
namespace TMK
{
    /** @brief Contains the available terminal font effects. */
    enum class Effect
    {
        /**
         * @brief Makes the text curly. In order to be seen, the terminal might require the use of a font with italic
         * style.
         */
        Italic = 1 << 3,
        /** @brief Draws a horizontal line crossing below the text. */
        Underlined = 1 << 4,
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

    /** @brief Contains the available terminal font weights. */
    enum class Weight
    {
        /** @brief Sets the weight to default. */
        Default,
        /** @brief Usually rendered as bold and/or with bright color. */
        Bold,
        /** @brief Usually rendered with faint color. */
        Dim
    };

    /** @brief Contains the ANSI codes of the first 16 colors of the XTerm palette plus a default color. */
    enum class XColor
    {
        /** @brief Sets the color to default. */
        Default = -1,
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

    /** @brief Represents the standard terminal input stream. */
    class Input final
    {
    public:
        Input() = delete;
        /**
         * @brief Checks if the input stream is connected to an interactive terminal (TTY).
         * @returns A boolean that states the stream is a TTY.
         */
        static bool IsTTY();
        /** @brief Clears the input buffer. */
        static void ClearBuffer();
        /**
         * @brief Gets the file descriptor associated with the input stream.
         * @returns The file descriptor associated with the stream.
         */
        static std::FILE* GetFile();
        /**
         * @brief Gets the file descriptor number associated with the input stream.
         * @returns The file descriptor number associated with the stream.
         */
        static int GetFileNo();
    };

    /** @brief Represents the standard terminal output stream. */
    class Output final
    {
    public:
        Output() = delete;
        /**
         * @brief Writes a formatted string to the output stream with a newline character appended at its end.
         * @param format The format to be used. It uses the same specifiers as the printf function family.
         * @param ... The arguments to be formatted.
         * @returns 0 if successful or -1 otherwise.
         */
        static int WriteLine(std::string format, ...);
        /**
         * @brief Writes a newline character to the output stream.
         * @returns 0 if successful or -1 otherwise.
         */
        static int WriteLine();
        /**
         * @brief Writes a formatted string to the output stream.
         * @param format The format to be used. It uses the same specifiers as the printf function family.
         * @param ... The arguments to be formatted.
         * @returns 0 if successful or -1 otherwise.
         */
        static int Write(std::string format, ...);
        /**
         * @brief Checks if the output stream is connected to an interactive terminal (TTY).
         * @returns A boolean that states the stream is a TTY.
         */
        static bool IsTTY();
        /**
         * @brief Gets the file descriptor associated with the output stream.
         * @returns The file descriptor associated with the stream.
         */
        static std::FILE* GetFile();
        /**
         * @brief Gets the file descriptor number associated with the output stream.
         * @returns The file descriptor number associated with the stream.
         */
        static int GetFileNo();
    };

    /** @brief Represents the standard terminal error stream. */
    class Error final
    {
    public:
        Error() = delete;
        /**
         * @brief Writes a formatted string to the error stream with a newline character appended at its end.
         * @param format The format to be used. It uses the same specifiers as the printf function family.
         * @param ... The arguments to be formatted.
         * @returns 0 if successful or -1 otherwise.
         */
        static int WriteLine(std::string format, ...);
        /**
         * @brief Writes a newline character to the error stream.
         * @returns 0 if successful or -1 otherwise.
         */
        static int WriteLine();
        /**
         * @brief Writes a formatted string to the error stream.
         * @param format The format to be used. It uses the same specifiers as the printf function family.
         * @param ... The arguments to be formatted.
         * @returns 0 if successful or -1 otherwise.
         */
        static int Write(std::string format, ...);
        /**
         * @brief Checks if the error stream is connected to an interactive terminal (TTY).
         * @returns A boolean that states the stream is a TTY.
         */
        static bool IsTTY();
        /**
         * @brief Gets the file descriptor associated with the error stream.
         * @returns The file descriptor associated with the stream.
         */
        static std::FILE* GetFile();
        /**
         * @brief Gets the file descriptor number associated with the error stream.
         * @returns The file descriptor number associated with the stream.
         */
        static int GetFileNo();
    };

    /** @brief Represents the terminal command-line arguments. */
    class Arguments final
    {
    public:
        /**
         * @brief Creates an instance of the Arguments class.
         * @param The total of arguments.
         * @param The arguments.
         */
        Arguments(int totalArguments, char** arguments);
        ~Arguments();
        /**
         * @brief Gets the total arguments.
         * @returns The total arguments.
         */
        int GetTotalArguments() const;
        /**
         * @brief Gets an argument by its offset.
         * @param The offset of the argument.
         * @returns The argument if successful or nullptr otherwise.
         */
        const char* GetArgumentByOffset(std::size_t offset) const;

    private:
        /** @brief The total arguments. */
        int m_totalArguments;
        /** @brief The arguments. */
        char** m_arguments;
    };

    /** @brief Represents dimensions inside of the terminal window. */
    class Dimensions final
    {
    public:
        /** @brief Creates a new instance of the Dimensions class. */
        Dimensions();
        /**
         * @brief Creates a new instance of the Dimensions class.
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
