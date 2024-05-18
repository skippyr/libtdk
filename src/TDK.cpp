#include "TDK.hpp"

static char g_cache = 0;

#define IS_TTY(a_stream) (g_cache & 1 << static_cast<int>(a_stream))
#define CHECK_STREAM_TTY_STATUS()                                                                                      \
    PrepareTTYAndCache();                                                                                              \
    if ((stream.rdbuf() == std::cout.rdbuf() && !IS_TTY(TDK::Stream::Output)) ||                                       \
        (stream.rdbuf() == std::cerr.rdbuf() && !IS_TTY(TDK::Stream::Error)))                                          \
    {                                                                                                                  \
        return stream;                                                                                                 \
    }

#ifdef _WIN32
#define TTY_CACHE(a_stream)                                                                                            \
    (_isatty(_fileno(!static_cast<int>(a_stream) ? stdin : static_cast<int>(a_stream) == 1 ? stdout : stderr)))
#else
#define TTY_CACHE(a_stream)                                                                                            \
    (isatty(fileno(!static_cast<int>(a_stream) ? stdin : static_cast<int>(a_stream) == 1 ? stdout : stderr)))
#endif

static void PrepareTTYAndCache();

static void PrepareTTYAndCache()
{
    if (!(g_cache & 1 << 7))
    {
        g_cache = TTY_CACHE(TDK::Stream::Input) | TTY_CACHE(TDK::Stream::Output) << 1 |
                  TTY_CACHE(TDK::Stream::Error) << 2 | 1 << 7;
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
