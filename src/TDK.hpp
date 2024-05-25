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

    enum class Key
    {

#ifdef _WIN32
        LeftArrow = -23,
        UpArrow,
        RightArrow,
        DownArrow,
#else
        UpArrow = -23,
        DownArrow,
        RightArrow,
        LeftArrow,
#endif
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
#ifdef _WIN32
        PageUp,
        PageDown,
        End,
        Home,
        Insert,
        Delete,
#else
        Home,
        Insert,
        Delete,
        End,
        PageUp,
        PageDown,
#endif
        Tab = 9,
#if defined(_WIN32) || defined(__APPLE__)
        Enter = 13,
#else
        Enter,
#endif
        Escape = 27,
        Spacebar = 32,
        Backspace = 127
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

    class KeyEvent
    {
    public:
        int m_key;
        bool m_hasAlt;
        bool m_hasCtrl;

        KeyEvent();
    };

    class WindowResizeEvent
    {
    public:
        Dimensions m_dimensions;

        WindowResizeEvent();
    };

    class EventInfo
    {
    public:
        EventType m_type;
        union {
            KeyEvent m_keyEvent;
            WindowResizeEvent m_windowResizeEvent;
        };

        EventInfo(EventType type);
        EventInfo(KeyEvent keyEvent);
        EventInfo(WindowResizeEvent windowResizeEvent);
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
    bool operator==(int code, Key key);
    bool operator!=(int code, Key key);
    bool operator>(int code, Key key);
    bool operator>=(int code, Key key);
    bool operator<(int code, Key key);
    bool operator<=(int code, Key key);

    void ClearCursorLine();
    void ClearInputBuffer();
    void ClearWindow();
    void CloseAlternateWindow();
    int GetCursorCoordinate(Coordinate& coordinate);
    int GetWindowDimensions(Dimensions& dimensions);
    bool IsTTY(Stream stream);
    void OpenAlternateWindow();
    EventInfo ReadEvent();
    EventInfo ReadEvent(int waitInMilliseconds);
    void RingBell();
    void SetCursorCoordinate(unsigned short column, unsigned short row);
    void SetCursorCoordinate(Coordinate& coordinate);
    void SetCursorShape(CursorShape shape);
    void SetCursorVisibility(bool isToShow);
    void SetWindowTitle(std::string title);
}
