#include "tmk.h"

#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

#pragma region Macros
/**
 * @brief A bitmask flag that states the TTY cache has been filled.
 */
#define _TMK_TTY_CACHE_HAS_BEEN_FILLED_FLAG (1 << 7)
/**
 * @brief Creates the TTY cache of a terminal stream in order to create a bitmask.
 * @param stream_a The stream being referenced. It must be a value from the tmk_Layer enum.
 * @returns The TTY cache of the stream.
 */
#define _TMK_TTY_CACHE(stream_a) (isatty(stream_a) << stream_a)
/**
 * @brief Checks the cache to see if a terminal stream is a TTY.
 * @param stream_a The stream being referenced. It must be a value from the tmk_Layer enum.
 * @returns A boolean that states the stream is a TTY.
 */
#define _TMK_IS_TTY(stream_a) (tmk_ttyCache_g & 1 << stream_a)
#pragma endregion

/**
 * @brief Contains the TTY statuses of the terminal streams.
 */
static char tmk_ttyCache_g = 0;

#pragma region Static Functions
/**
 * @brief Fills the TTY cache.
 */
static void _tmk_fillTTYCache(void)
{
    if (!(tmk_ttyCache_g & _TMK_TTY_CACHE_HAS_BEEN_FILLED_FLAG))
    {
        tmk_ttyCache_g |= _TMK_TTY_CACHE_HAS_BEEN_FILLED_FLAG | _TMK_TTY_CACHE(tmk_Stream_Input) | _TMK_TTY_CACHE(tmk_Stream_Output) | _TMK_TTY_CACHE(tmk_Stream_Error);
    }
}

/**
 * @brief Writes an ANSI escape sequence to the terminal output or error streams.
 * @param format The format to be used. It accepts the same format specifiers as the printf function family.
 * @param ... The arguments to be formatted.
 */
static void _tmk_writeANSIEscapeSequence(const char* format, ...)
{
    _tmk_fillTTYCache();
    va_list arguments;
    va_start(arguments, format);
    vfprintf(_TMK_IS_TTY(tmk_Stream_Output) ? stdout : stderr, format, arguments);
    va_end(arguments);
}
#pragma endregion

#pragma region Library Functions
void tmk_setXColor(unsigned char color, tmk_Layer_t layer)
{
    _tmk_writeANSIEscapeSequence("\x1b[%d8;5;%dm", layer, color);
}

void tmk_setRGBColor(tmk_RGBColor_t color, tmk_Layer_t layer)
{
    _tmk_writeANSIEscapeSequence("\x1b[%d8;2;%d;%d;%dm", layer, color.red, color.green, color.blue);
}

void tmk_setHexColor(tmk_HexColor_t color, tmk_Layer_t layer)
{
    tmk_setRGBColor((tmk_RGBColor_t){color >> 16 & 0xff, color >> 8 & 0xff, color & 0xff}, layer);
}

void tmk_resetColors()
{
    _tmk_writeANSIEscapeSequence("\x1b[39;49m");
}
#pragma endregion
