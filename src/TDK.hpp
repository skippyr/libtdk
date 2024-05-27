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

    template <typename T>
    class Color
    {
    public:
        virtual T Invert() const = 0;
        Layer GetLayer() const;
        void SetLayer(Layer layer);

    protected:
        Layer m_layer;

        Color();
        static Layer FilterLayer(Layer layer);
    };

    class Coordinate final
    {
    public:
        Coordinate();
        Coordinate(unsigned short column, unsigned short row);
        unsigned short GetColumn() const;
        unsigned short GetRow() const;
        void SetColumn(unsigned short column);
        void SetRow(unsigned short row);

    private:
        unsigned short m_column;
        unsigned short m_row;
    };

    class Effect final
    {
    public:
        Effect(EffectCode code, bool isToEnable);
        Effect(int code, bool isToEnable);

        int GetCode() const;
        bool IsToEnable() const;

    private:
        int m_code;
        bool m_isToEnable;

        static int FilterCode(int code);
    };

    class HexColor final : public Color<HexColor>
    {
    public:
        HexColor(unsigned int code, Layer layer);
        HexColor Invert() const;
        unsigned int GetCode() const;
        void SetCode(unsigned int code);

    private:
        unsigned int m_code;

        static unsigned int FilterCode(unsigned int code);
    };

    class Region final
    {
    public:
        Region();
        Region(unsigned short totalColumns, unsigned short totalRows);
        Region(Coordinate cornerCoordinate0, Coordinate cornerCoordinate1);
        bool Contains(unsigned short column, unsigned short row) const;
        bool Contains(Coordinate coordinate) const;
        unsigned short GetTotalColumns() const;
        unsigned short GetTotalRows() const;
        unsigned int GetArea() const;
        Coordinate GetTopLeftCoordinate() const;
        Coordinate GetTopRightCoordinate() const;
        Coordinate GetBottomLeftCoordinate() const;
        Coordinate GetBottomRightCoordinate() const;

    private:
        unsigned short m_totalColumns;
        unsigned short m_totalRows;
        unsigned int m_area;
        Coordinate m_topLeftCoordinate;
        Coordinate m_topRightCoordinate;
        Coordinate m_bottomLeftCoordinate;
        Coordinate m_bottomRightCoordinate;
    };

    class RGBColor final : public Color<RGBColor>
    {
    public:
        RGBColor(unsigned char red, unsigned char green, unsigned char blue, Layer layer);
        RGBColor(HexColor color);
        RGBColor Invert() const;
        unsigned char GetRed() const;
        unsigned char GetGreen() const;
        unsigned char GetBlue() const;
        void SetRed(unsigned char red);
        void SetGreen(unsigned char green);
        void SetBlue(unsigned char blue);

    private:
        unsigned char m_red;
        unsigned char m_green;
        unsigned char m_blue;
    };

    class XColor final : public Color<XColor>
    {
    public:
        XColor(unsigned char code, Layer layer);
        XColor(XColorCode code, Layer layer);
        XColor Invert() const;
        short GetCode() const;
        void SetCode(unsigned char code);
        void SetCode(XColorCode code);

    private:
        short m_code;

        static short FilterCode(XColorCode code);
    };

    std::ostream& operator<<(std::ostream& stream, Effect effect);
    std::ostream& operator<<(std::ostream& stream, HexColor color);
    std::ostream& operator<<(std::ostream& stream, RGBColor color);
    std::ostream& operator<<(std::ostream& stream, XColor color);
    std::ostream& operator<<(std::ostream& stream, Weight weight);
    bool operator==(XColor color0, XColor color1);
    bool operator==(HexColor color0, HexColor color1);
    bool operator==(RGBColor color0, RGBColor color1);
    bool operator==(Coordinate coordinate0, Coordinate coordinate1);
    bool operator==(Region region0, Region region1);
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
    void SetCursorCoordinate(Coordinate coordinate);
    void SetCursorShape(CursorShape shape);
    void SetCursorVisibility(bool isToShow);
    void SetWindowTitle(std::string title);
}
