#pragma once

#include <functional>
#include <iostream>

/// <summary>
/// A simple C++ terminal manipulation library for Windows and Linux. It can handle terminal properties, colors, effects
/// and window resize and key events readings with UTF-8 encoding.
/// </summary>
namespace TDK
{
    /// <summary>
    /// Contains the available terminal cursor shapes.
    /// </summary>
    enum class CursorShape
    {
        /// <summary>
        /// Resets the default shape.
        /// </summary>
        Default,
        /// <summary>
        /// The blinking variant of the block shape.
        /// </summary>
        BlinkingBlock,
        /// <summary>
        /// The non-blinking variant of the block shape.
        /// </summary>
        Block,
        /// <summary>
        /// The blinking variant of the underline shape.
        /// </summary>
        BlinkingUnderline,
        /// <summary>
        /// The non-blinking variant of the underline shape.
        /// </summary>
        Underline,
        /// <summary>
        /// The blinking variant of the bar shape.
        /// </summary>
        BlinkingBar,
        /// <summary>
        /// The non-blinking variant of the bar shape.
        /// </summary>
        Bar
    };

    /// <summary>
    /// Contains the ANSI code of the available terminal effects.
    /// </summary>
    enum class EffectCode
    {
        /// <summary>
        /// Makes the text curly.
        /// </summary>
        Italic = 3,
        /// <summary>
        /// Draws a horizontal line below the text.
        /// </summary>
        Underline,
        /// <summary>
        /// Makes the text blink indefinetely.
        /// </summary>
        Blinking,
        /// <summary>
        /// Swaps the background and foreground colors.
        /// </summary>
        Negative = 7,
        /// <summary>
        /// Makes the text hard to see or invisible.
        /// </summary>
        Hidden,
        /// <summary>
        /// Draws a horizontal line crossing through the text.
        /// </summary>
        CrossedOut
    };

    /// <summary>
    /// Contains the available terminal layers.
    /// </summary>
    enum class Layer
    {
        /// <summary>
        /// The foreground layer.
        /// </summary>
        Foreground = 3,
        /// <summary>
        /// The background layer.
        /// </summary>
        Background
    };

    /// <summary>
    /// Contains the standard terminal data streams.
    /// </summary>
    enum class Stream
    {
        /// <summary>
        /// Receives data and events.
        /// </summary>
        Input,
        /// <summary>
        /// Outputs regular tense messages.
        /// </summary>
        Output,
        /// <summary>
        /// Outputs error tense messages.
        /// </summary>
        Error
    };

    /// <summary>
    /// Contains the ANSI code of the first 16 colors of the XTerm palette plus one more color intended for resets.
    /// </summary>
    enum class XColorCode
    {
        /// <summary>
        /// Resets the color.
        /// </summary>
        Default = -1,
        /// <summary>
        /// The dark variant of the black color.
        /// </summary>
        DarkBlack,
        /// <summary>
        /// The dark variant of the red color.
        /// </summary>
        DarkRed,
        /// <summary>
        /// The dark variant of the green color.
        /// </summary>
        DarkGreen,
        /// <summary>
        /// The dark variant of the yellow color.
        /// </summary>
        DarkYellow,
        /// <summary>
        /// The dark variant of the blue color.
        /// </summary>
        DarkBlue,
        /// <summary>
        /// The dark variant of the magenta color.
        /// </summary>
        DarkMagenta,
        /// <summary>
        /// The dark variant of the cyan color.
        /// </summary>
        DarkCyan,
        /// <summary>
        /// The dark variant of the white color.
        /// </summary>
        DarkWhite,
        /// <summary>
        /// The light variant of the black color.
        /// </summary>
        LightBlack,
        /// <summary>
        /// The light variant of the red color.
        /// </summary>
        LightRed,
        /// <summary>
        /// The light variant of the green color.
        /// </summary>
        LightGreen,
        /// <summary>
        /// The light variant of the yellow color.
        /// </summary>
        LightYellow,
        /// <summary>
        /// The light variant of the blue color.
        /// </summary>
        LightBlue,
        /// <summary>
        /// The light variant of the magenta color.
        /// </summary>
        LightMagenta,
        /// <summary>
        /// The light variant of the cyan color.
        /// </summary>
        LightCyan,
        /// <summary>
        /// The light variant of the white color.
        /// </summary>
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
        /// <summary>
        /// Creates a new instance of the Coordinate class.
        /// </summary>
        /// <param name="column">The column component of the coordinate.</param>
        /// <param name="row">The row component of the coordinate.</param>
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
