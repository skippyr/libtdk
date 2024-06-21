#include "tmk.h"

#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#pragma region Macros
/**
 * @brief A bitmask flag that states the redirection cache of the terminal streams has been filled.
 */
#define _TMK_STREAM_REDIRECTION_HAS_BEEN_FILLED (1 << 7)
/**
 * @brief Creates the redirection cache of a terminal stream in order to create a bitmask.
 * @param stream_a The stream being referenced. It must be a value from the tmk_FontLayer enum.
 * @returns The redirection cache of the stream.
 */
#define _TMK_REDIRECTION_CACHE(stream_a) (!isatty(stream_a) << stream_a)
/**
 * @brief Checks the cache to see if a terminal stream is being redirected.
 * @param stream_a The stream being referenced. It must be a value from the tmk_FontLayer enum.
 * @returns A boolean that states the stream is being redirected.
 */
#define _TMK_IS_STREAM_REDIRECTED(stream_a) (tmk_streamRedirectionCache_g & 1 << stream_a)
#pragma endregion

/**
 * @brief Contains the redirection cache of the terminal streams.
 */
static char tmk_streamRedirectionCache_g = 0;

#pragma region Static Functions
/**
 * @brief Fills the TTY cache.
 */
static void _tmk_fillTTYCache(void)
{
    if (!(tmk_streamRedirectionCache_g & _TMK_STREAM_REDIRECTION_HAS_BEEN_FILLED))
    {
        tmk_streamRedirectionCache_g |= _TMK_STREAM_REDIRECTION_HAS_BEEN_FILLED | _TMK_REDIRECTION_CACHE(tmk_Stream_Input) | _TMK_REDIRECTION_CACHE(tmk_Stream_Output) | _TMK_REDIRECTION_CACHE(tmk_Stream_Error);
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
    vfprintf(!_TMK_IS_STREAM_REDIRECTED(tmk_Stream_Output) ? stdout : stderr, format, arguments);
    va_end(arguments);
}

/**
 * @brief Formats and writes a string to a terminal stream.
 * @param stream The stream to be affected.
 * @param hasNewline A boolean that states a newline grapheme should be appended to the end of the stream.
 * @param format The format to be used. It accepts the same format specifiers as the print function family.
 * @param arguments The arguments to be formatted.
 */
static void _tmk_writeToStream(enum tmk_Stream stream, bool hasNewline, const char* format, va_list arguments)
{
    if (stream == tmk_Stream_Error)
    {
        fflush(stdout);
    }
    FILE* file = stream == tmk_Stream_Output ? stdout : stderr;
    vfprintf(file, format, arguments);
    if (hasNewline)
    {
        fputc('\n', file);
    }
}
#pragma endregion

#pragma region Library Functions
bool tmk_isStreamRedirected(enum tmk_Stream stream)
{
    _tmk_fillTTYCache();
    return _TMK_IS_STREAM_REDIRECTED(stream);
}

int tmk_getWindowDimensions(struct tmk_Dimensions* dimensions)
{
    struct winsize size;
    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &size) && ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) && ioctl(STDERR_FILENO, TIOCGWINSZ, &size))
    {
        return -1;
    }
    dimensions->totalColumns = size.ws_col;
    dimensions->totalRows = size.ws_row;
    return 0;
}

void tmk_setFontXColor(unsigned char color, enum tmk_FontLayer layer)
{
    _tmk_writeANSIEscapeSequence("\x1b[%d8;5;%dm", layer, color);
}

void tmk_setFontRGBColor(struct tmk_RGBColor color, enum tmk_FontLayer layer)
{
    _tmk_writeANSIEscapeSequence("\x1b[%d8;2;%d;%d;%dm", layer, color.red, color.green, color.blue);
}

void tmk_setFontHexColor(unsigned int color, enum tmk_FontLayer layer)
{
    tmk_setFontRGBColor((struct tmk_RGBColor){color >> 16 & 0xff, color >> 8 & 0xff, color & 0xff}, layer);
}

void tmk_setFontWeight(enum tmk_FontWeight weight)
{
    _tmk_writeANSIEscapeSequence("\x1b[22;%dm", weight);
}

void tmk_setCursorShape(enum tmk_CursorShape shape, bool shouldBlink)
{
    _tmk_writeANSIEscapeSequence("\x1b[%d q", shape - shouldBlink);
}

void tmk_setCursorVisibility(bool isVisible)
{
    _tmk_writeANSIEscapeSequence("\x1b[?25%c", isVisible ? 'h' : 'l');
}

void tmk_resetFontColors()
{
    _tmk_writeANSIEscapeSequence("\x1b[39;49m");
}

void tmk_resetFontWeight()
{
    _tmk_writeANSIEscapeSequence("\x1b[22m");
}

void tmk_resetCursorShape()
{
    _tmk_writeANSIEscapeSequence("\x1b[0 q");
}

void tmk_openAlternateWindow(void)
{
    _tmk_writeANSIEscapeSequence("\x1b[?1049h\x1b[2J\x1b[1;1H");
}

void tmk_closeAlternateWindow(void)
{
    _tmk_writeANSIEscapeSequence("\x1b[?1049l");
}

void tmk_writeErrorArguments(const char* format, va_list arguments)
{
    _tmk_writeToStream(tmk_Stream_Output, false, format, arguments);
}

void tmk_writeError(const char* format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    tmk_writeArguments(format, arguments);
    va_end(arguments);
}

void tmk_writeErrorLineArguments(const char* format, va_list arguments)
{
    _tmk_writeToStream(tmk_Stream_Output, true, format, arguments);
}

void tmk_writeErrorLine(const char* format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    tmk_writeLineArguments(format, arguments);
    va_end(arguments);
}

void tmk_writeArguments(const char* format, va_list arguments)
{
    _tmk_writeToStream(tmk_Stream_Output, false, format, arguments);
}

void tmk_write(const char* format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    tmk_writeArguments(format, arguments);
    va_end(arguments);
}

void tmk_writeLineArguments(const char* format, va_list arguments)
{
    _tmk_writeToStream(tmk_Stream_Output, true, format, arguments);
}

void tmk_writeLine(const char* format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    tmk_writeLineArguments(format, arguments);
    va_end(arguments);
}
#pragma endregion
