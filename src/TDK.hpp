#pragma once

#include <cstdarg>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#else
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif

/*
 * @brief Terminal Development Kit (TDK) is a cross-platform C++ terminal manipulation library that provides a
 * human-friendly interface to manipulate ANSI escape sequences and parse terminal events. It can be used to create
 * keyboard driven applications and other terminal libraries for more specific use cases for both Windows and Linux.
 * @brief -
 * @brief Check its full documentation at https://github.com/skippyr/wiki.
 */
namespace TDK
{
    /*
     * @brief An enum class containing the available terminal cursor shapes. To apply one, use the SetCursorShape
     * function.
     */
    enum class CursorShape
    {
        /* @brief The default shape, intended for resets. */
        Default,
        /* @brief The blinking variant of the block shape. */
        BlinkingBlock,
        /* @brief The non-blinking variant of the block shape. */
        Block,
        /* @brief The blinking variant of the underline shape. */
        BlinkingUnderline,
        /* @brief The non-blinking variant of the underline shape. */
        Underline,
        /* @brief The blinking variant of the bar shape. */
        BlinkingBar,
        /* @brief The non-blinking variant of the bar shape. */
        Bar
    };

    /*
     * @brief An enum class containing the available terminal text effects. To apply one, use the left shifting
     * operator (<<) agains an instance of the std::ostream class. To remove it, use the right shifting instead.
     */
    enum class Effect
    {
        /*
         * @brief The italic effect makes the text curly. It may require the use of a font with italic style in order to
         * be seen.
         */
        Italic = 3,
        /* @brief The underline effect draws a horizontal line crossing below the text. */
        Underline,
        /* @brief The blinking effect makes the text blink indefinitely. */
        Blinking,
        /* @brief The reverse-video effect swaps the foreground and background colors. */
        ReverseVideo = 7,
        /* @brief The conceal effect makes the text hard to see or invisible. */
        Conceal,
        /* @brief The strike-through effect draws a horizontal line crossing through the middle of the text. */
        Strikethrough
    };

    /*
     * @brief An enum class containing mostly keyboard keys not represented by UTF-8. For exceptions, the value here
     * serve the purpose of increase readability.
     */
    enum class Key
    {
#ifdef _WIN32
        /* @brief The left arrow key. */
        LeftArrow = -23,
        /* @brief The up arrow key. */
        UpArrow,
        /* @brief The right arrow key. */
        RightArrow,
        /* @brief The down arrow key. */
        DownArrow,
#else
        /* @brief The up arrow key. */
        UpArrow = -23,
        /* @brief The down arrow key. */
        DownArrow,
        /* @brief The right arrow key. */
        RightArrow,
        /* @brief The left arrow key. */
        LeftArrow,
#endif
        /* @brief The F1 function key. */
        F1,
        /* @brief The F2 function key. */
        F2,
        /* @brief The F3 function key. */
        F3,
        /* @brief The F4 function key. */
        F4,
        /* @brief The F5 function key. */
        F5,
        /* @brief The F6 function key. */
        F6,
        /* @brief The F7 function key. */
        F7,
        /* @brief The F8 function key. */
        F8,
        /* @brief The F9 function key. */
        F9,
        /* @brief The F10 function key. */
        F10,
        /* @brief The F11 function key. */
        F11,
        /* @brief The F12 function key. */
        F12,
#ifdef _WIN32
        /* @brief The PageUp navigation key. */
        PageUp,
        /* @brief The PageDown navigation key. */
        PageDown,
        /* @brief The End navigation key. */
        End,
        /* @brief The Home navigation key. */
        Home,
        /* @brief The Insert insertion key. */
        Insert,
        /* @brief The Delete deletion key. */
        Delete,
#else
        /* @brief The Home navigation key. */
        Home,
        /* @brief The Insert insertion key. */
        Insert,
        /* @brief The Delete deletion key. */
        Delete,
        /* @brief The End navigation key. */
        End,
        /* @brief The PageUp navigation key. */
        PageUp,
        /* @brief The PageDown navigation key. */
        PageDown,
#endif
        /* @brief The Tab insertion key. */
        Tab = 9,
#if defined(_WIN32) || defined(__APPLE__)
        /* @brief The Enter insertion key. */
        Enter = 13,
#else
        /* @brief The Enter Insertion key. */
        Enter,
#endif
        /* @brief The Escape navigation key. */
        Escape = 27,
        /* @brief The Spacebar insertion key. */
        Spacebar = 32,
        /* @brief The Backspace deletion key. */
        Backspace = 127
    };

};

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
 * @brief An enum class containing the available terminal text weights. To apply one, use the left shifting operator
 * (<<) agains an instance of the std::ostream class.
 */
enum class Weight
{
    /* @brief The default weight, intended for resets. */
    Default,
    /*
     * @brief The bold weight is usually rendered with bold weight and/or with light colors. It may require the use
     * of a font with bold weight in order to be seen.
     */
    Bold,
    /* @brief The dim weight is usually rendered with faint colors. */
    Dim
};

/*
 * @brief A class that represents a coordinate inside of the terminal window. It is primarily used to refer to the
 * cursor one by using the GetCursorCoordinate and SetCursorCoordinate functions.
 */
class Coordinate
{
public:
    /* @brief Creates an instance of the Coordinate class. */
    Coordinate();
    /*
     * @brief Creates an instance of the Coordinate class.
     * @param column The column component of the coordinate.
     * @param row The row component of the coordinate.
     */
    Coordinate(unsigned short column, unsigned short row);

    /* @brief The column component of the coordinate. */
    unsigned short m_column;
    /* @brief The row component of the coordinate. */
    unsigned short m_row;
};

/*
 * @brief A class that represents dimensions inside of the terminal window. It is primarily used to refer to the
 * window dimensions by using the GetWindowDimensions function.
 */
class Dimensions
{
public:
    /* @brief Creates an instance of the Dimensions class. */
    Dimensions();
    /*
     * @brief Creates an instance of the Dimensions class.
     * @param totalColumns The total columns of the dimensions.
     * @param totalRows The total rows of the dimensions.
     */
    Dimensions(unsigned short totalColumns, unsigned short totalRows);

    /* @brief The total columns of the dimensions. */
    unsigned short m_totalColumns;
    /* @brief The total rows of the dimensions. */
    unsigned short m_totalRows;
};

/*
 * @brief A class that represents a hexadecimal color. Apply it by using the left shifting operator (<<)
 * against an instance of the std::ostream class. Remove it by using the XColor class with XColorCode::Default
 * targetting the same layer where the color has been applied on.
 */
class HexColor
{
public:
    /*
     * @brief Creates an instance of the HexColor class.
     * @param code The hexadecimal code of the color.
     * @param layer The layer where the color should be applied on.
     */
    HexColor(unsigned int code, Layer layer);
    /* @brief Inverts the layer of the color. */
    HexColor Invert();

    /* @brief The hexadecimal code of the color. */
    unsigned int m_code;
    /* @brief The layer where the color should be applied on. */
    Layer m_layer;
};

/*
 * @brief A class that represents an RGB color. Apply it by using the left shifting operator (<<) against an
 * instance of the std::ostream class. Remove it by using the XColor class with XColorCode::Default targetting the
 * same layer where the color has been applied on.
 */
class RGBColor
{
public:
    /*
     * @brief Creates an instance of the RGBColor class.
     * @param red The red component the color.
     * @param green The green component of the color.
     * @param blue The blue component of the color.
     * @param layer The layer where the color should be applied on.
     */
    RGBColor(unsigned char red, unsigned char green, unsigned char blue, Layer layer);
    /*
     * @brief Creates an instance of the RGBColor class.
     * @param color An hexadecimal color to be converted.
     */
    RGBColor(HexColor color);
    /* @brief Invers the layer of the color. */
    RGBColor Invert();

    /* @brief The red component of the color. */
    unsigned char m_red;
    /* @brief The green component of the color. */
    unsigned char m_green;
    /* @brief The blue component of the color. */
    unsigned char m_blue;
    /* @brief The layer where the color should be applied on. */
    Layer m_layer;
};

/*
 * @brief A class that represents a color from the XTerm palette: a terminal exclusive palette composed by 256
 * colors. Apply it by using the left shifting operator (<<) against an instance of the std::ostream class. Remove
 * it by using the XColorCode::Default targetting the same layer where the color has been applied on.
 */
class XColor
{
public:
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

    /* @brief The ANSI code of the color. */
    int m_code;
    /* @brief The layer where color should be applied on. */
    Layer m_layer;
};

std::ostream& operator<<(std::ostream& stream, Effect effect);
std::ostream& operator>>(std::ostream& stream, Effect effect);
std::ostream& operator<<(std::ostream& stream, HexColor color);
std::ostream& operator<<(std::ostream& stream, RGBColor color);
std::ostream& operator<<(std::ostream& stream, XColor color);
std::ostream& operator<<(std::ostream& stream, Weight weight);

/* @brief Clears the contents present in the terminal cursor line. */
void ClearCursorLine();
/*
 * @brief Clears the contents present in the terminal input buffer, allowing new readings to be performed without
 * conflicts.
 */
void ClearInputBuffer();
/*
 * @brief Gets the terminal cursor coordinate. On Linux, as it parses a terminal answer given through the standard
 * input buffer, it will always clear the input buffer.
 * @param coordinate The address where the coordinate info will be put into.
 * @returns 0 if successful and -1 otherwise.
 */
int GetCursorCoordinate(Coordinate& coordinate);
/*
 * @brief Gets the dimensions of the terminal window.
 * @param dimensions The address where the dimensions info will be put into.
 * @returns 0 if successful and -1 otherwise.
 */
int GetWindowDimensions(Dimensions& dimensions);
/*
 * @brief Looks into the library cache to checks if a standard terminal stream is connected to an interactive
 * terminal (TTY).
 * @param stream The stream to be checked.
 * @returns The check result.
 */
bool IsTTY(Stream stream);
/*
 * @brief Rings the terminal bell, possibly emitting a symbol in the terminal title bar, visual flash, a system
 * notification or a beep from the motherboard speaker.
 */
void RingBell();
/*
 * @brief Sets the state of the alternate window buffer.
 * @param isToOpen A boolean that states the alternate window should be opened or not.
 */
void SetAlternateWindow(bool isToOpen);
/*
 * @brief Sets the terminal cursor coordinate. Both component, will always be fit within the terminal window
 * boundaries. Use GetWindowDimensions to treat possible exceptions.
 * @param column The column component of the coordinate.
 * @param row The row component of the coordinate.
 */
void SetCursorCoordinate(unsigned short column, unsigned short row);
/*
 * @brief Sets the terminal cursor coordinate.
 * @param coordinate The coordinate to be set. It will always be fit within the terminal window boundaries. Use
 * GetWindowDimensions to treat possible exceptions.
 */
void SetCursorCoordinate(Coordinate& coordinate);
/*
 * @brief Sets the terminal cursor shape.
 * @param shape The shape to be set.
 */
void SetCursorShape(CursorShape shape);
/*
 * @brief Sets the terminal cursor visibility.
 * @param isToShow A boolean that states the cursor should be visible or not.
 */
void SetCursorVisibility(bool isToShow);
}
