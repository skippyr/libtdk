#pragma once

#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#else
#include <unistd.h>
#endif

namespace TDK
{
    enum class Layer
    {
        Foreground = 3,
        Background
    };

    enum class Stream
    {
        Input,
        Output,
        Error
    };

    enum class XColorCode
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

    class XColor
    {
    public:
        int m_code;
        Layer m_layer;

        XColor(int code, Layer layer);
        XColor(XColorCode code, Layer layer);
    };

    std::ostream& operator<<(std::ostream& stream, XColor color);
}
