#pragma once

#include <functional>
#include <iostream>

namespace TMK
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

    enum class EventType
    {
        Failure,
        None,
        TimeOut,
        Resize,
        Focus,
        Key,
        Mouse
    };

    enum class EffectCode
    {
        Italic = 3,
        Underline,
        Blinking,
        Negative = 7,
        Hidden,
        CrossedOut
    };

    enum class Layer
    {
        Foreground = 3,
        Background
    };

    enum class MouseButton
    {
        None,
        Left,
        Wheel,
        WheelUp,
        WheelDown,
        Right
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

    class RGBColor;

    template <class T>
    class Color
    {
    public:
        virtual T Invert() const = 0;
        Layer GetLayer() const;
        void SetLayer(Layer layer);

    protected:
        Layer m_layer;

        static Layer FilterLayer(Layer layer);
    };

    class HexColor final : public Color<HexColor>
    {
    public:
        HexColor(unsigned int code, Layer layer);
        HexColor(RGBColor color);
        HexColor Invert() const;
        std::string ToString(bool isUpper, bool hasPrefix) const;
        unsigned int GetCode() const;
        void SetCode(unsigned int code);

    private:
        unsigned int m_code;

        static unsigned int FilterCode(unsigned int code);
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

    class Dimensions final
    {
    public:
        Dimensions();
        Dimensions(unsigned short totalColumns, unsigned short totalRows);
        unsigned short GetTotalColumns() const;
        unsigned short GetTotalRows() const;

    private:
        unsigned short m_totalColumns;
        unsigned short m_totalRows;
    };

    class FocusEvent final
    {
    public:
        FocusEvent(bool hasFocus);
        bool HasFocus() const;

    private:
        bool m_hasFocus;
    };

    class ResizeEvent final
    {
    public:
        ResizeEvent();
        Dimensions GetDimensions() const;

    private:
        Dimensions m_dimensions;
    };

    class MouseEvent final
    {
    public:
        MouseEvent(Coordinate coordinate, MouseButton button, bool isDragging, bool hasCtrl, bool hasAlt,
                   bool hasShift);
        Coordinate GetCoordinate() const;
        MouseButton GetButton() const;
        bool IsDragging() const;
        bool HasCtrl() const;
        bool HasAlt() const;
        bool HasShift() const;

    private:
        Coordinate m_coordinate;
        MouseButton m_button;
        bool m_isDragging;
        bool m_hasCtrl;
        bool m_hasAlt;
        bool m_hasShift;
    };

    class EventInfo final
    {
    public:
        EventInfo(EventType type);
        EventInfo(FocusEvent focusEvent);
        EventInfo(ResizeEvent resizeEvent);
        EventInfo(MouseEvent mouseEvent);
        EventType GetType() const;
        FocusEvent GetFocusEvent() const;
        ResizeEvent GetResizeEvent() const;
        MouseEvent GetMouseEvent() const;

    private:
        EventType m_type;
        union {
            FocusEvent m_focusEvent;
            ResizeEvent m_resizeEvent;
            MouseEvent m_mouseEvent;
        };
    };

    class Effects final
    {
    public:
        Effects(EffectCode code, bool isToEnable);
        Effects(int code, bool isToEnable);
        int GetCode() const;
        bool IsToEnable() const;

    private:
        int m_code;
        bool m_isToEnable;

        static int FilterCode(int code);
    };

    std::ostream& operator<<(std::ostream& stream, Effects effects);
    std::ostream& operator<<(std::ostream& stream, HexColor color);
    std::ostream& operator<<(std::ostream& stream, RGBColor color);
    std::ostream& operator<<(std::ostream& stream, XColor color);
    std::ostream& operator<<(std::ostream& stream, Weight weight);
    bool operator==(XColor color0, XColor color1);
    bool operator==(HexColor color0, HexColor color1);
    bool operator==(RGBColor color0, RGBColor color1);
    bool operator==(HexColor hexColor, RGBColor rgbColor);
    bool operator==(RGBColor rgbColor, HexColor hexColor);
    bool operator==(Coordinate coordinate0, Coordinate coordinate1);
    int operator|(EffectCode code0, EffectCode code1);

    void ClearCursorLine();
    void ClearInputBuffer();
    void ClearWindow();
    void CloseAlternateWindow();
    int GetCursorCoordinate(Coordinate& coordinate);
    int GetWindowDimensions(Dimensions& dimensions);
    bool IsTTY(Stream stream);
    void OpenAlternateWindow();
    void RingBell();
    EventInfo ReadEvent(bool allowMouseCapture);
    EventInfo ReadEvent(bool allowMouseCapture, unsigned short waitInMilliseconds);
    EventInfo ReadEvent(bool allowMouseCapture, unsigned short waitInMilliseconds,
                        std::function<bool(EventInfo&)> filter);
    void SetCursorCoordinate(unsigned short column, unsigned short row);
    void SetCursorCoordinate(Coordinate coordinate);
    void SetCursorShape(CursorShape shape);
    void SetCursorVisibility(bool isVisible);
    void SetWindowTitle(std::string title);
}
