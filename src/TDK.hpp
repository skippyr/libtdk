#pragma once

#include <functional>
#include <iostream>

namespace TDK
{
    enum class CursorShape
    {
        Default,
        BlinkingBlock,
        Block,
        BlinkingUnderline,
        Underline,
        BlinkingBar,
        Bar
    };

    enum class EffectCode
    {
        Italic = 3,
        Underline,
        Blinking,
        Negative = 7,
        Hidden,
        Strikethrough
    };

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

    enum class Weight
    {
        Default,
        Bold,
        Dim
    };

    class Coordinate final
    {
    public:
        unsigned short m_column;
        unsigned short m_row;

        Coordinate();
        Coordinate(unsigned short column, unsigned short row);
    };

    class Effect final
    {
    public:
        int m_code;
        bool m_isToEnable;

        Effect(EffectCode code, bool isToEnable);
        Effect(int code, bool isToEnable);
    };

    class HexColor final
    {
    public:
        unsigned int m_code;
        Layer m_layer;

        HexColor(unsigned int code, Layer layer);
        HexColor Invert();
    };

    class Region final
    {
    public:
        unsigned short m_totalColumns;
        unsigned short m_totalRows;
        unsigned int m_area;
        Coordinate m_topLeftCoordinate;
        Coordinate m_topRightCoordinate;
        Coordinate m_bottomLeftCoordinate;
        Coordinate m_bottomRightCoordinate;

        Region();
        Region(unsigned short totalColumns, unsigned short totalRows);
        Region(Coordinate& cornerCoordinate0, Coordinate& cornerCoordinate1);
    };

    class RGBColor final
    {
    public:
        unsigned char m_red;
        unsigned char m_green;
        unsigned char m_blue;
        Layer m_layer;

        RGBColor(unsigned char red, unsigned char green, unsigned char blue, Layer layer);
        RGBColor(HexColor& color);
        RGBColor Invert();
    };

    class XColor final
    {
    public:
        int m_code;
        Layer m_layer;

        XColor(unsigned char code, Layer layer);
        XColor(XColorCode code, Layer layer);
        XColor Invert();
    };

    std::ostream& operator<<(std::ostream& stream, Effect effect);
    std::ostream& operator<<(std::ostream& stream, HexColor& color);
    std::ostream& operator<<(std::ostream& stream, RGBColor& color);
    std::ostream& operator<<(std::ostream& stream, XColor& color);
    std::ostream& operator<<(std::ostream& stream, Weight weight);
    bool operator==(XColor& color0, XColor& color1);
    bool operator==(HexColor& color0, HexColor& color1);
    bool operator==(RGBColor& color0, RGBColor& color1);
    bool operator==(Coordinate& coordinate0, Coordinate& coordinate1);
    bool operator==(Region& region0, Region& region1);
    int operator|(EffectCode code0, EffectCode code1);

    void ClearCursorLine();
    void ClearInputBuffer();
    void ClearWindow();
    void CloseAlternateWindow();
    int GetCursorCoordinate(Coordinate& coordinate);
    int GetWindowRegion(Region& region);
    bool IsTTY(Stream stream);
    void OpenAlternateWindow();
    void RingBell();
    void SetCursorCoordinate(unsigned short column, unsigned short row);
    void SetCursorCoordinate(Coordinate& coordinate);
    void SetCursorShape(CursorShape shape);
    void SetCursorVisibility(bool isToShow);
    void SetWindowTitle(std::string title);
}
