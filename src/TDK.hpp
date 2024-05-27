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

    /// <summary>
    /// Contains the available terminal weights.
    /// </summary>
    enum class Weight
    {
        /// <summary>
        /// Resets the weight.
        /// </summary>
        Default,
        /// <summary>
        /// Usually rendered as bold weight and/or with light colors.
        /// </summary>
        Bold,
        /// <summary>
        /// Usually rendered with faint colors.
        /// </summary>
        Dim
    };

    /// <summary>
    /// Represents a generic terminal color.
    /// </summary>
    /// <typeparam name="T">Represents the more specific color.</typeparam>
    template <class T>
    class Color
    {
    public:
        /// <summary>
        /// Inverts the layer the color applies on.
        /// </summary>
        /// <returns>The color with its layer inverted.</returns>
        virtual T Invert() const = 0;
        /// <summary>
        /// Gets the layer the color applies on.
        /// </summary>
        /// <returns>The layer of the color.</returns>
        Layer GetLayer() const;
        /// <summary>
        /// Sets the layer the color applies on.
        /// </summary>
        /// <param name="layer">The layer to be set.</param>
        void SetLayer(Layer layer);

    protected:
        /// <summary>
        /// The layer the color applies on.
        /// </summary>
        Layer m_layer;

        /// <summary>
        /// Creates a new instance of the Color class.
        /// </summary>
        Color();
        /// <summary>
        /// Filters a layer.
        /// </summary>
        /// <param name="layer">The layer to be filtered.</param>
        /// <returns>If valid, the layer given, otherwise, Layer::Foreground.</returns>
        static Layer FilterLayer(Layer layer);
    };

    /// <summary>
    /// Represents a terminal coordinate.
    /// </summary>
    class Coordinate final
    {
    public:
        /// <summary>
        /// Creates a new instance of the Coordinate class.
        /// </summary>
        Coordinate();
        /// <summary>
        /// Creates a new instance of the Coordinate class.
        /// </summary>
        /// <param name="column">The column component of the coordinate.</param>
        /// <param name="row">The row component of the coordinate.</param>
        Coordinate(unsigned short column, unsigned short row);
        /// <summary>
        /// Gets the column component of the coordinate.
        /// </summary>
        /// <returns>The column component of the coordinate.</returns>
        unsigned short GetColumn() const;
        /// <summary>
        /// Gets the row component of the coordinate.
        /// </summary>
        /// <returns>The row component of the coordinate.</returns>
        unsigned short GetRow() const;
        /// <summary>
        /// Sets the column component of the coordinate.
        /// </summary>
        /// <param name="column">The column component to be set.</param>
        void SetColumn(unsigned short column);
        /// <summary>
        /// Sets the row component of the coordinate.
        /// </summary>
        /// <param name="column">The row component to be set.</param>
        void SetRow(unsigned short row);

    private:
        /// <summary>
        /// The column component of the coordinate.
        /// </summary>
        unsigned short m_column;
        /// <summary>
        /// The row component of the coordinate.
        /// </summary>
        unsigned short m_row;
    };

    /// <summary>
    /// Represents a terminal effect.
    /// </summary>
    class Effect final
    {
    public:
        /// <summary>
        /// Creates a new instance of the Effect class.
        /// </summary>
        /// <param name="code">The ANSI code of the effect.</param>
        /// <param name="isToEnable">A boolean that states the effect targeted should be enabled.</param>
        Effect(EffectCode code, bool isToEnable);
        /// <summary>
        /// Creates a new instance of the Effect class.
        /// </summary>
        /// <param name="code">A bitmask containing the ANSI code of the effects.</param>
        /// <param name="isToEnable">A boolean that states the effects targeted should be enabled.</param>
        Effect(int code, bool isToEnable);

        /// <summary>
        /// Gets a bitmask containg the ANSI codes of the effects being targeted.
        /// </summary>
        /// <returns>A bitmask containg the ANSI codes of the effects being targeted</returns>
        int GetCode() const;
        /// <summary>
        /// Gets a boolean the states the effects targeted should be enabled.
        /// </summary>
        /// <returns>A boolean that states the effects targeted should be enabled.</returns>
        bool IsToEnable() const;

    private:
        /// <summary>
        /// A bitmask containg the ANSI codes of the effects being targeted.
        /// </summary>
        int m_code;
        /// <summary>
        /// A boolean that states the effects targeted should be enabled.
        /// </summary>
        bool m_isToEnable;

        /// <summary>
        /// Filters the ANSI code of effects given in a bitmask.
        /// </summary>
        /// <param name="code">The bitmask to be filtered.</param>
        /// <returns>A bitmask containing only the valid effects.</returns>
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
