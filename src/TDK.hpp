#pragma once

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

    enum class EventType
    {
        WindowResize,
        Key,
        None,
        TimeOut,
        Invalid
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

    class Coordinate
    {
    public:
        Coordinate();
        Coordinate(unsigned short column, unsigned short row);

        unsigned short m_column;
        unsigned short m_row;
    };

    class Dimensions
    {
    public:
        Dimensions();
        Dimensions(unsigned short totalColumns, unsigned short totalRows);

        unsigned short m_totalColumns;
        unsigned short m_totalRows;
    };

    class Effect
    {
    public:
        int m_code;
        bool m_isToEnable;

        Effect(EffectCode code, bool isToEnable);
        Effect(int code, bool isToEnable);
    };

    class Event
    {
    public:
        EventType m_type;
        typedef union {
            Dimensions dimensions;
        } m_data;

        Event(EventType type);
    };

    class HexColor
    {
    public:
        HexColor(unsigned int code, Layer layer);
        HexColor Invert();

        unsigned int m_code;
        Layer m_layer;
    };

    class RGBColor
    {
    public:
        RGBColor(unsigned char red, unsigned char green, unsigned char blue, Layer layer);
        RGBColor(HexColor color);
        RGBColor Invert();

        unsigned char m_red;
        unsigned char m_green;
        unsigned char m_blue;
        Layer m_layer;
    };

    class XColor
    {
    public:
        XColor(unsigned char code, Layer layer);
        XColor(XColorCode code, Layer layer);
        XColor Invert();

        int m_code;
        Layer m_layer;
    };

    std::ostream& operator<<(std::ostream& stream, Effect effect);
    std::ostream& operator<<(std::ostream& stream, HexColor color);
    std::ostream& operator<<(std::ostream& stream, RGBColor color);
    std::ostream& operator<<(std::ostream& stream, XColor color);
    std::ostream& operator<<(std::ostream& stream, Weight weight);
    int operator|(EffectCode code0, EffectCode code1);
    bool operator==(Event event0, Event event1);

    void ClearCursorLine();
    void ClearInputBuffer();
    void ClearWindow();
    void CloseAlternateWindow();
    int GetCursorCoordinate(Coordinate& coordinate);
    int GetWindowDimensions(Dimensions& dimensions);
    bool IsTTY(Stream stream);
    void OpenAlternateWindow();
    Event ReadEvent();
    Event ReadEvent(int waitInMilliseconds);
    void RingBell();
    void SetCursorCoordinate(unsigned short column, unsigned short row);
    void SetCursorCoordinate(Coordinate& coordinate);
    void SetCursorShape(CursorShape shape);
    void SetCursorVisibility(bool isToShow);
    void SetWindowTitle(std::string title);
}
