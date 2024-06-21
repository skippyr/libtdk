#ifndef _TMK_H
#define _TMK_H

#include <stdarg.h>
#include <stdbool.h>

#pragma region Enums
/**
 * @brief Contains the terminal streams.
 */
enum tmk_Stream
{
    /**
     * @brief Where events and data is input.
     */
    tmk_Stream_Input,
    /**
     * @brief Where regular tense messages are output.
     */
    tmk_Stream_Output,
    /**
     * @brief Where error tense messages are output.
     */
    tmk_Stream_Error
};

/**
 * @brief Contains the ANSI code of the first 16 colors of the XTerm palette.
 */
enum tmk_XColor
{
    /**
     * @brief The dark variant of the black color.
     */
    tmk_XColor_DarkBlack,
    /**
     * @brief The dark variant of the red color.
     */
    tmk_XColor_DarkRed,
    /**
     * @brief The dark variant of the green color.
     */
    tmk_XColor_DarkGreen,
    /**
     * @brief The dark variant of the yellow color.
     */
    tmk_XColor_DarkYellow,
    /**
     * @brief The dark variant of the blue color.
     */
    tmk_XColor_DarkBlue,
    /**
     * @brief The dark variant of the magenta color.
     */
    tmk_XColor_DarkMagenta,
    /**
     * @brief The dark variant of the cyan color.
     */
    tmk_XColor_DarkCyan,
    /**
     * @brief The dark variant of the white color.
     */
    tmk_XColor_DarkWhite,
    /**
     * @brief The light variant of the black color.
     */
    tmk_XColor_LightBlack,
    /**
     * @brief The light variant of the red color.
     */
    tmk_XColor_LightRed,
    /**
     * @brief The light variant of the green color.
     */
    tmk_XColor_LightGreen,
    /**
     * @brief The light variant of the yellow color.
     */
    tmk_XColor_LightYellow,
    /**
     * @brief The light variant of the blue color.
     */
    tmk_XColor_LightBlue,
    /**
     * @brief The light variant of the magenta color.
     */
    tmk_XColor_LightMagenta,
    /**
     * @brief The light variant of the cyan color.
     */
    tmk_XColor_LightCyan,
    /**
     * @brief The light variant of the white color.
     */
    tmk_XColor_LightWhite,
};

/**
 * @brief Contains the terminal font layers.
 */
enum tmk_FontLayer
{
    /**
     * @brief Refers to the graphemes.
     */
    tmk_FontLayer_Foreground = 3,
    /**
     * @brief Refers to the background of the graphemes.
     */
    tmk_FontLayer_Background
};

/**
 * @brief Contains the terminal font weights.
 */
enum tmk_FontWeight
{
    /**
     * @brief Usually rendered as bold weight and/or with bright colors.
     */
    tmk_FontWeight_Bold = 1,
    /**
     * @brief Usually rendered with faint colors.
     */
    tmk_FontWeight_Light
};

/**
 * @brief Contains the terminal font effects.
 */
enum tmk_FontEffect
{
    /**
     * @brief Makes the text curly.
     */
    tmk_FontEffect_Italic = 1 << 3,
    /**
     * @brief Draws a horizontal line crossing below the text.
     */
    tmk_FontEffect_Underline = 1 << 4,
    /**
     * @brief Makes the text blink indefinitely.
     */
    tmk_FontEffect_Blink = 1 << 5,
    /**
     * @brief Swaps the background and foreground colors.
     */
    tmk_FontEffect_Negative = 1 << 7,
    /**
     * @brief Makes the text hard to see or invisible.
     */
    tmk_FontEffect_Hidden = 1 << 8,
    /**
     * @brief Draws a horizontal line crossing through the middle of the text.
     */
    tmk_FontEffect_Strike = 1 << 9
};

/**
 * @brief Contains the terminal cursor shapes.
 */
enum tmk_CursorShape
{
    /**
     * @brief Fills the whole character cell.
     **/
    tmk_CursorShape_Block = 2,
    /**
     * @brief Fills a region at the bottom of the character cell.
     **/
    tmk_CursorShape_Underline = 4,
    /**
     * @brief Fills a region at the left of the character cell.
     */
    tmk_CursorShape_Bar = 6
};
#pragma endregion

#pragma region Structs
/**
 * @brief Represents an RGB color.
 */
struct tmk_RGBColor
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

/**
 * @brief Represents terminal dimensions.
 */
struct tmk_Dimensions
{
    /**
     * @brief The total columns of the dimensions.
     */
    unsigned short totalColumns;
    /**
     * @brief The total rows of the dimensions.
     */
    unsigned short totalRows;
};

/**
 * @brief Represents a terminal coordinate.
 */
struct tmk_Coordinate
{
    /**
     * @brief The column component of the coordinate.
     */
    unsigned short column;
    /**
     * @brief The row component of the coordinate.
     */
    unsigned short row;
};
#pragma endregion

#ifdef __cplusplus
extern "C"
{
#endif
#pragma region Functions
    /**
     * @brief Checks if a terminal stream is redirected.
     * @param stream The stream being referenced.
     * @returns A boolean that states the terminal stream is redirected.
     */
    bool tmk_isStreamRedirected(enum tmk_Stream stream);
    /**
     * @brief Gets the terminal window dimensions.
     * @param dimensions The address where the information about the dimensions should be put into.
     * @returns 0 if successful or -1 otherwise.
     */
    int tmk_getWindowDimensions(struct tmk_Dimensions* dimensions);
    /**
     * @brief Sets a XTerm color into a terminal font layer.
     * @param color The color to be set. It must be a value in range from 0 to 255 or an enumerator from the tmk_XColor enum.
     * @param layer The layer to be affected.
     */
    void tmk_setFontXColor(unsigned char color, enum tmk_FontLayer layer);
    /**
     * @brief Sets an RGB color into a terminal font layer.
     * @param color The color to be set.
     * @param layer The layer to be affected.
     */
    void tmk_setFontRGBColor(struct tmk_RGBColor color, enum tmk_FontLayer layer);
    /**
     * @brief Sets a hex color into a terminal font layer.
     * @param color The color to be set. It must be a value in range from 0x0 to 0xffffff.
     * @param layer The layer to be affected.
     */
    void tmk_setFontHexColor(unsigned int color, enum tmk_FontLayer layer);
    /**
     * @brief Sets the terminal font weight.
     * @param weight The weight to be set.
     */
    void tmk_setFontWeight(enum tmk_FontWeight weight);
    /**
     * @brief Sets terminal font effects.
     * @param effectsMask A bitmask containing the effects to be set.
     */
    void tmk_setFontEffects(int effectsMask);
    /**
     * @brief Sets the terminal cursor shape.
     * @param shape The shape to be set.
     * @param shouldBlink A boolean that states the cursor should blink.
     */
    void tmk_setCursorShape(enum tmk_CursorShape shape, bool shouldBlink);
    /**
     * @brief Sets the terminal cursor visibility.
     * @param isVisible A boolean that states the terminal cursor should be visible.
     */
    void tmk_setCursorVisibility(bool isVisible);
    /**
     * @brief Sets the terminal cursor coordinate.
     * @param coordinate The coordinate to be set.
     */
    void tmk_setCursorCoordinate(struct tmk_Coordinate coordinate);
    /**
     * @brief Resets the terminal font colors.
     */
    void tmk_resetFontColors(void);
    /**
     * @brief Resets the terminal font weight.
     */
    void tmk_resetFontWeight(void);
    /**
     * @brief Resets the terminal font effects.
     */
    void tmk_resetFontEffects(void);
    /**
     * @brief Resets the terminal cursor shape.
     */
    void tmk_resetCursorShape(void);
    /**
     * @brief Opens the alternate window buffer.
     */
    void tmk_openAlternateWindow(void);
    /**
     * @brief Closes the alternate window buffer.
     */
    void tmk_closeAlternateWindow(void);
    /**
     * @brief Clears the terminal cursor line.
     */
    void tmk_clearCursorLine(void);
    /**
     * @brief Clears the terminal window.
     */
    void tmk_clearWindow(void);
    /**
     * @brief Rings the terminal bell.
     */
    void tmk_ringBell(void);
    /**
     * @brief Flushes the terminal output buffer.
     */
    void tmk_flushOutputBuffer(void);
    /**
     * @brief Clears the terminal input buffer.
     */
    void tmk_clearInputBuffer(void);
    /**
     * @brief Formats and writes a string to the terminal error stream.
     * @param format The format to be used. It accepts the same format specifiers as the printf function family.
     * @param arguments The arguments to be formatted.
     */
    void tmk_writeErrorArguments(const char* format, va_list arguments);
    /**
     * @brief Formats and writes a string to the terminal error stream.
     * @param format The format to be used. It accepts the same format specifiers as the printf function family.
     * @param ... The arguments to be formatted.
     */
    void tmk_writeError(const char* format, ...);
    /**
     * @brief Formats and writes a string to the terminal error stream with a newline grapheme appended to its end.
     * @param format The format to be used. It accepts the same format specifiers as the printf function family.
     * @param arguments The arguments to be formatted.
     */
    void tmk_writeErrorLineArguments(const char* format, va_list arguments);
    /**
     * @brief Formats and writes a string to the terminal error stream with a newline grapheme appended to its end.
     * @param format The format to be used. It accepts the same format specifiers as the printf function family.
     * @param ... The arguments to be formatted.
     */
    void tmk_writeErrorLine(const char* format, ...);
    /**
     * @brief Formats and writes a string to the terminal output stream.
     * @param format The format to be used. It accepts the same format specifiers as the printf function family.
     * @param arguments The arguments to be formatted.
     */
    void tmk_writeArguments(const char* format, va_list arguments);
    /**
     * @brief Formats and writes a string to the terminal output stream.
     * @param format The format to be used. It accepts the same format specifiers as the printf function family.
     * @param ... The arguments to be formatted.
     */
    void tmk_write(const char* format, ...);
    /**
     * @brief Formats and writes a string to the terminal output stream with a newline grapheme appended to its end.
     * @param format The format to be used. It accepts the same format specifiers as the printf function family.
     * @param arguments The arguments to be formatted.
     */
    void tmk_writeLineArguments(const char* format, va_list arguments);
    /**
     * @brief Formats and writes a string to the terminal output stream with a newline grapheme appended to its end.
     * @param format The format to be used. It accepts the same format specifiers as the printf function family.
     * @param ... The arguments to be formatted.
     */
    void tmk_writeLine(const char* format, ...);
#pragma endregion
#ifdef __cplusplus
}
#endif
#endif
