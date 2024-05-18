#include "TDK.hpp"

/* @brief A cache that contains the TTY statuses of the standard terminal streams. */
static char g_cache = 0;

/*
 * @brief Looks into cache to check if a standard terminal stream is a TTY.
 * @param a_stream The stream to be checked. It must be a value from the TDK::Stream enum class.
 * @returns A boolean with the result.
 */
#define IS_TTY(a_stream) static_cast<bool>(g_cache & 1 << static_cast<int>(a_stream))
/* @brief Causes an early return if the standard terminal stream being targeted is not a TTY. */
#define CHECK_STREAM_TTY_STATUS()                                                                                      \
    PrepareTTYAndCache();                                                                                              \
    if ((stream.rdbuf() == std::cout.rdbuf() && !IS_TTY(TDK::Stream::Output)) ||                                       \
        (stream.rdbuf() == std::cerr.rdbuf() && !IS_TTY(TDK::Stream::Error)))                                          \
    {                                                                                                                  \
        return stream;                                                                                                 \
    }
/* @brief A bit that states the cache has already been filled in. */
#define HAS_CACHED_TTY_BIT (1 << 7)
#ifdef _WIN32
/*
 * @brief Creates the TTY status cache of a standard terminal stream.
 * @param a_stream The stream being checked.
 */
#define TTY_CACHE(a_stream)                                                                                            \
    (!!(_isatty(_fileno(!static_cast<int>(a_stream)       ? stdin                                                      \
                        : static_cast<int>(a_stream) == 1 ? stdout                                                     \
                                                          : stderr)))                                                  \
     << static_cast<int>(a_stream))
#else
/*
 * @brief Creates the TTY status cache of a standard terminal stream.
 * @param a_stream The stream being checked.
 */
#define TTY_CACHE(a_stream)                                                                                            \
    (isatty(fileno(!static_cast<int>(a_stream)       ? stdin                                                           \
                   : static_cast<int>(a_stream) == 1 ? stdout                                                          \
                                                     : stderr))                                                        \
     << static_cast<int>(a_stream))
#endif

/*
 * @brief Creates a cache containing info about the TTY statuses of the standard terminal streams and, on Windows, set
 * the ENABLE_VIRTUAL_TERMINAL_PROCESSING bit in order to the terminal to start parsing ANSI escape sequences.
 */
static void PrepareTTYAndCache();

static void PrepareTTYAndCache()
{
    if (!(g_cache & HAS_CACHED_TTY_BIT))
    {
        g_cache = TTY_CACHE(TDK::Stream::Input) | TTY_CACHE(TDK::Stream::Output) | TTY_CACHE(TDK::Stream::Error) |
                  HAS_CACHED_TTY_BIT;
    }
#ifdef _WIN32
    if (!IS_TTY(TDK::Stream::Input) && !IS_TTY(TDK::Stream::Error))
    {
        return;
    }
    HANDLE handle = GetStdHandle(IS_TTY(TDK::Stream::Output) ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE);
    DWORD mode;
    GetConsoleMode(handle, &mode);
    SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

TDK::XColor::XColor(int code, Layer layer) : m_code(code), m_layer(layer)
{
}

TDK::XColor::XColor(XColorCode code, Layer layer) : m_code(static_cast<int>(code)), m_layer(layer)
{
}

std::ostream& TDK::operator<<(std::ostream& stream, XColor color)
{
    CHECK_STREAM_TTY_STATUS();
    return color.m_code == static_cast<int>(TDK::XColorCode::Default)
               ? stream << "\x1b[" << static_cast<int>(color.m_layer) << "9m"
               : stream << "\x1b[" << static_cast<int>(color.m_layer) << "8;5;" << color.m_code << "m";
}
