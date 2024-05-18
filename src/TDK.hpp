#pragma once

#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#else
#include <unistd.h>
#endif

/*
 * @brief Terminal Development Kit (TDK) is a C++ terminal manipulation library that can be used to create keyboard
 * driven applications and other libraries for more specific use cases for both Windows and Linux.
 */
namespace TDK
{
    /*
     * @brief An enum class containing the available layers where a color can be applied on. They are used as reference
     * to apply the classes XColor, HexColor and RGBColor.
     */
    enum class Layer
    {
        /* @brief The foreground layer refers to the color of letters. */
        Foreground = 3,
        /* @brief The background layer refers to the background of the letters. */
        Background
    };

    /* @brief An enum class containing the available standard terminal data streams. */
    enum class Stream
    {
        /* @brief The input stream receives characters and events from the user or other process. */
        Input,
        /* @brief The output stream serves the purpose of outputting regular tense messages. */
        Output,
        /* @brief The error stream serves the purpose of outputting error tense messages. */
        Error
    };

    /*
     * @brief An enum class containing the ANSI codes of the first 16 colors of the XTerm color palette plus one more
     * color intended for resets. These codes may be used to create instances of the XColor class. They refer colors
     * from the active terminal theme, so they can be used to ensure consistency.
     */
    enum class XColorCode
    {
        /* @brief The default color, intended for resets. */
        Default = -1,
        /* @brief The dark variant of the black color. */
        Black,
        /* @brief The dark variant of the red color. */
        Red,
        /* @brief The dark variant of the green color. */
        Green,
        /* @brief The dark variant of the yellow color. */
        Yellow,
        /* @brief The dark variant of the blue color. */
        Blue,
        /* @brief The dark variant of the magenta color. */
        Magenta,
        /* @brief The dark variant of the cyan color. */
        Cyan,
        /* @brief The dark variant of the white color. */
        White,
        /* @brief The light variant of the black color. */
        LightBlack,
        /* @brief The light variant of the red color. */
        LightRed,
        /* @brief The light variant of the green color. */
        LightGreen,
        /* @brief The light variant of the yellow color. */
        LightYellow,
        /* @brief The light variant of the blue color. */
        LightBlue,
        /* @brief The light variant of the magenta color. */
        LightMagenta,
        /* @brief The light variant of the cyan color. */
        LightCyan,
        /* @brief The light variant of the white color. */
        LightWhite
    };

    /*
     * @brief A class that represents a color from the XTerm palette: a terminal exclusive palette composed by 256
     * colors. Apply it by using the left shifting operator (<<) against an instance of the std::ostream class. Remove
     * it by using the XColorCode::Default targetting the same layer where the color has been applied on.
     */
    class XColor
    {
    public:
        /* @brief The ANSI code of the color. */
        int m_code;
        /* @brief The layer where color should be applied on. */
        Layer m_layer;

        /*
         * @brief Creates an instance of the XColor class.
         * @param code The ANSI code of the color. It must be a value in range from 0 to 255.
         * @param layer The layer where the color should be applied on.
         */
        XColor(unsigned char code, Layer layer);
        /*
         * @brief Creates an instance of the XColor class.
         * @param code The ANSI code of the color.
         * @param layer The layer where the color should be applied on.
         */
        XColor(XColorCode code, Layer layer);
        /* @brief Inverts the layer of the color. */
        XColor Invert();
    };

    class RGBColor
    {
    public:
        unsigned char m_red;
        unsigned char m_green;
        unsigned char m_blue;
        Layer m_layer;

        RGBColor(unsigned char red, unsigned char green, unsigned char blue, Layer layer);
        RGBColor Invert();
    };

    std::ostream& operator<<(std::ostream& stream, XColor color);
    std::ostream& operator<<(std::ostream& stream, RGBColor color);
}
