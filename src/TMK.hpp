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

    /** @brief Contains the ANSI codes of the first 16 colors of the XTerm palette. */
    enum class XColor
    {
        /** @brief The regular variant of the black color. */
        Black,
        /** @brief The regular variant of the red color. */
        Red,
        /** @brief The regular variant of the green color. */
        Green,
        /** @brief The regular variant of the yellow color. */
        Yellow,
        /** @brief The regular variant of the blue color. */
        Blue,
        /** @brief The regular variant of the magenta color. */
        Magenta,
        /** @brief The regular variant of the cyan color. */
        Cyan,
        /** @brief The regular variant of the white color. */
        White,
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

    private:
        Input() = delete;
    };

    /** @brief Represents the standard terminal output stream. */
    class Output final
    {
    public:
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

    private:
        Output() = delete;
    };

    /** @brief Represents the standard terminal error stream. */
    class Error final
    {
    public:
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

    private:
        Error() = delete;
    };

    /** @brief Represents the terminal command-line arguments. */
    class Arguments final
    {
    public:
        /**
         * @brief Creates an instance of the Arguments class.
         * @param totalArguments The total of arguments.
         * @param arguments The arguments.
         */
        Arguments(int totalArguments, char** arguments);
        /** @brief Destroys an instance of the Arguments class. */
        ~Arguments();
        /**
         * @brief Gets the total arguments.
         * @returns The total arguments.
         */
        int GetTotalArguments() const;
        /**
         * @brief Gets an argument by its offset.
         * @param offset The offset of the argument.
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

    /** @brief Represents the environment the terminal is running on. */
    class Environment final
    {
    public:
        /**
         * @brief Gets and treats the command-line arguments associated with the terminal process.
         * @param rawTotalArguments The total arguments given as parameter of the main function.
         * @param rawArguments The arguments given as parameter of the main function.
         * @returns The treated arguments associated with the terminal process.
         */
        static Arguments GetArguments(int rawTotalArguments, char** rawArguments);

    private:
        Environment() = delete;
    };

    /** @brief Represents the terminal font. */
    class Font final
    {
    public:
        /**
         * @brief Sets the font weight.
         * @param weight The weight to be set.
         */
        static void SetWeight(Weight weight);
        /**
         * @brief Sets a font effect.
         * @param effect The effect to be set.
         * @param isToEnable A boolean that state the effects should be enabled or disabled.
         */
        static void SetEffect(Effect effect, bool isToEnable);
        /**
         * @brief Sets one or multiple font effects.
         * @param effect A bitmask containing the effects to be set. It must be composed by enumerators from the Effect
         * enum class.
         * @param isToEnable A boolean that state the effects should be enabled or disabled.
         */
        static void SetEffect(int effect, bool isToEnable);
        /**
         * @brief Sets an XTerm font color.
         * @param color The color to be set.
         * @param layer The layer where the color should be applied on.
         */
        static void SetXColor(XColor color, Layer layer);
        /**
         * @brief Sets an XTerm font color.
         * @param color The color to be set. It must be a value in range from 0 to 255.
         * @param layer The layer where the color should be applied on.
         */
        static void SetXColor(int color, Layer layer);
        /**
         * @brief Sets a hex font color.
         * @param hex The color to be set. It must be a value in range from 0 to 0xffffff.
         * @param layer The layer where the color should be applied on.
         */
        static void SetHexColor(int hex, Layer layer);
        /**
         * @brief Sets an RGB font color.
         * @param red The red component of the color. It must be a value in range from 0 to 255.
         * @param green The green component of the color. It must be a value in range from 0 to 255.
         * @param blue The blue component of the color. It must be a value in range from 0 to 255.
         * @param layer The layer where the color should be applied on.
         */
        static void SetRGBColor(int red, int green, int blue, Layer layer);
        static void ResetColors();

    private:
        Font() = delete;
    };

    /** @brief Represents the terminal window. */
    class Window final
    {
    public:
        /**
         * @brief Gets the terminal window dimensions.
         * @param dimensions The address where the dimensions information will be put into.
         */
        static int GetDimensions(Dimensions& dimensions);
        /**
         * @brief Sets the terminal window title.
         * @param title The title to be set.
         */
        static void SetTitle(std::string title);
        /** @brief Opens the terminal alternate window buffer. */
        static void OpenAlternateBuffer();
        /** @brief Closes the terminal alternate window buffer. */
        static void CloseAlternateBuffer();
        /** @brief Clears the terminal window. */
        static void Clear();

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

    /**
     * @brief Concatenates two font effects together in order to create a bitmask containing both.
     * @param effect0 The first effect.
     * @param effect1 The second effect.
     * @returns A bitmask containing both effects.
     */
    int operator|(Effect effect0, Effect effect1);
}
