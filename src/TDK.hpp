#pragma once

#include <functional>
#include <iostream>

/**
 * @brief A simple C++ terminal manipulation library for Windows and Linux. It can handle terminal properties, colors,
 * effects and events readings with UTF-8 encoding.
 */
namespace TDK
{
    /** @brief Contains the available terminal cursor shapes. */
    enum class CursorShape
    {
        /** @brief Resets the default shape. */
        Default,
        /** @brief The blinking variant of the block shape. */
        BlinkingBlock,
        /** @brief The non-blinking variant of the block shape. */
        NonBlinkingBlock,
        /** @brief The blinking variant of the underline shape. */
        BlinkingUnderline,
        /** @brief The non-blinking variant of the underline shape. */
        NonBlinkingUnderline,
        /** @brief The blinking variant of the bar shape. */
        BlinkingBar,
        /** @brief The non-blinking variant of the bar shape. */
        NonBlinkingBar
    };

    /** @brief Contains the available terminal event types. */
    enum class EventType
    {
        /** @brief No events were available. */
        None,
        /** @brief The event timer ran out. */
        TimeOut,
        /** @brief A window resize event was read. */
        WindowResize,
        /** @brief A windows focus event was read. */
        WindowsFocus,
        /** @brief A key event was read. */
        Key,
        /** @brief A mouse event was read. */
        Mouse
    };

    /** @brief Contains the ANSI code of the available terminal effects. */
    enum class EffectCode
    {
        /** @brief Makes the text curly. */
        Italic = 3,
        /** @brief Draws a horizontal line below the text. */
        Underline,
        /** @brief Makes the text blink indefinitely. */
        Blinking,
        /** @brief Swaps the foreground and background colors. */
        Negative = 7,
        /** @brief Makes the text hard to see or invisible. */
        Hidden,
        /** @brief Draws a horizontal line crossing through the text. */
        CrossedOut
    };

    /** @brief Contains the available terminal layers. */
    enum class Layer
    {
        /** @brief The foreground layer. */
        Foreground = 3,
        /** @brief The background layer. */
        Background
    };

    /** @brief Contains the standard terminal streams. */
    enum class Stream
    {
        /** @brief Receives data and events */
        Input,
        /** @brief Outputs regular tense messages. */
        Output,
        /** @brief Outputs error tense messages. */
        Error
    };

    /**
     * @brief Contains the ANSI code of the first 16 colors of the XTerm palette plus one more color intended for
     * resets.
     */
    enum class XColorCode
    {
        /** @brief Resets the color. */
        Default = -1,
        /** @brief The dark variant of the black color. */
        DarkBlack,
        /** @brief The dark variant of the red color. */
        DarkRed,
        /** @brief The dark variant of the green color. */
        DarkGreen,
        /** @brief The dark variant of the yellow color. */
        DarkYellow,
        /** @brief The dark variant of the blue color. */
        DarkBlue,
        /** @brief The dark variant of the magenta color. */
        DarkMagenta,
        /** @brief The dark variant of the cyan color. */
        DarkCyan,
        /** @brief The dark variant of the white color. */
        DarkWhite,
        /** @brief The light variant of the black color. */
        LightBlack,
        /** @brief The light variant of the red color. */
        LightRed,
        /** @brief The light variant of the green color. */
        LightGreen,
        /** @brief The light variant of the yellow color. */
        LightYellow,
        /** @brief The light variant of the blue color. */
        LightBlue,
        /** @brief the light variant of the magenta color. */
        LightMagenta,
        /** @brief The light variant of the cyan color. */
        LightCyan,
        /** @brief The light variant of the white color. */
        LightWhite
    };

    /** @brief Contains the available terminal weights. */
    enum class Weight
    {
        /** @brief Resets the weight */
        Default,
        /** @brief Usually rendered as bold weight and/or with light colors. */
        Bold,
        /** @brief Usually rendered with faint colors. */
        Dim
    };

    template <class T>
    class Color;
    class Coordinate;
    class Effect;
    class HexColor;
    class Region;
    class RGBColor;
    class XColor;

    /**
     * @brief Represents a generic terminal color.
     * @tparam T The more specific color.
     */
    template <class T>
    class Color
    {
    public:
        /**
         * @brief Inverts the layer the color applies on.
         * @returns The color with its layer inverted.
         */
        virtual T Invert() const = 0;
        /**
         * @brief Gets the layer the color applies on.
         * @returns The layer of the color.
         */
        Layer GetLayer() const;
        /**
         * @brief Sets the layer the color applies on.
         * @param layer The layer to be set.
         */
        void SetLayer(Layer layer);

    protected:
        /** @brief The layer the color applies on. */
        Layer m_layer;

        /** @brief Creates a new instance of the Color class. */
        Color();
        /**
         * @brief Filters a layer.
         * @param layer The layer to be filtered.
         * @returns The layer given if valid or Layer::Foreground otherwise.
         */
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
    /// Represents a group of terminal effects.
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

    /// <summary>
    /// Represents a terminal hex color.
    /// </summary>
    class HexColor final : public Color<HexColor>
    {
    public:
        /// <summary>
        /// Creates an instance of the HexColor class.
        /// </summary>
        /// <param name="code">The hex code of the color.</param>
        /// <param name="layer">The layer the color applies on.</param>
        HexColor(unsigned int code, Layer layer);
        /// <summary>
        /// Creates an instance of the HexColor class.
        /// </summary>
        /// <param name="color">The RGB color to be converted.</param>
        HexColor(RGBColor color);
        /// <summary>
        /// Inverts the layer the color applies on.
        /// </summary>
        /// <returns>The color with its layer inverted.</returns>
        HexColor Invert() const;
        /// <summary>
        /// Gets the hex code of the color.
        /// </summary>
        /// <returns>The hex code of the color.</returns>
        unsigned int GetCode() const;
        /// <summary>
        /// Sets the hex code of the color.
        /// </summary>
        /// <param name="code">The hex code to be set.</param>
        void SetCode(unsigned int code);

    private:
        /// <summary>
        /// The hex code of the color.
        /// </summary>
        unsigned int m_code;

        /// <summary>
        /// Filters a hex code.
        /// </summary>
        /// <param name="code">The hex code to be filtered.</param>
        /// <returns>The hex code filtered within a valid range.</returns>
        static unsigned int FilterCode(unsigned int code);
    };

    /// <summary>
    /// Represents a terminal region.
    /// </summary>
    class Region final
    {
    public:
        /// <summary>
        /// Creates a new instance of the Region class.
        /// </summary>
        Region();
        /// <summary>
        /// Creates a new instance of the Region class.
        /// </summary>
        /// <param name="totalColumns">The total columns of the region.</param>
        /// <param name="totalRows">The total rows of the region.</param>
        Region(unsigned short totalColumns, unsigned short totalRows);
        /// <summary>
        /// Creates a new instance of the Region class.
        /// </summary>
        /// <param name="cornerCoordinate0">A first corner coordinate of the region.</param>
        /// <param name="cornerCoordinate1">A second corner coordinate of the region, opposite to the first one.</param>
        Region(Coordinate cornerCoordinate0, Coordinate cornerCoordinate1);
        /// <summary>
        /// Checks if a coordinate is contained within the region.
        /// </summary>
        /// <param name="column">The column component of the coordinate.</param>
        /// <param name="row">The row component of the coordinate.</param>
        /// <returns>A boolean that states the coordinate is contained within the region.</returns>
        bool Contains(unsigned short column, unsigned short row) const;
        /// <summary>
        /// Checks if a coordinate is contained within the region.
        /// </summary>
        /// <param name="coordinate">The coordinate to be checked.</param>
        /// <returns>A boolean that states the coordinate is contained within the region.</returns>
        bool Contains(Coordinate coordinate) const;
        /// <summary>
        /// Gets the total columns of the region.
        /// </summary>
        /// <returns>The total columns of the region.</returns>
        unsigned short GetTotalColumns() const;
        /// <summary>
        /// Gets the total rows of the region.
        /// </summary>
        /// <returns>The total rows of the region.</returns>
        unsigned short GetTotalRows() const;
        /// <summary>
        /// Gets the area of the region.
        /// </summary>
        /// <returns>The area of the region.</returns>
        unsigned int GetArea() const;
        /// <summary>
        /// Gets the top left coordinate of the region.
        /// </summary>
        /// <returns>The top left coordinate of the region.</returns>
        Coordinate GetTopLeftCoordinate() const;
        /// <summary>
        /// Gets the top right coordinate of the region.
        /// </summary>
        /// <returns>The top right coordinate of the region.</returns>
        Coordinate GetTopRightCoordinate() const;
        /// <summary>
        /// Gets the bottom left coordinate of the region.
        /// </summary>
        /// <returns>The bottom left coordinate of the region.</returns>
        Coordinate GetBottomLeftCoordinate() const;
        /// <summary>
        /// Gets the bottom right coordinate of the region.
        /// </summary>
        /// <returns>The bottom right coordinate of the region.</returns>
        Coordinate GetBottomRightCoordinate() const;

    private:
        /// <summary>
        /// The total columns of the region.
        /// </summary>
        unsigned short m_totalColumns;
        /// <summary>
        /// The total rows of the region.
        /// </summary>
        unsigned short m_totalRows;
        /// <summary>
        /// The area of the region.
        /// </summary>
        unsigned int m_area;
        /// <summary>
        /// The top left coordinate of the region.
        /// </summary>
        Coordinate m_topLeftCoordinate;
        /// <summary>
        /// The top right coordinate of the region.
        /// </summary>
        Coordinate m_topRightCoordinate;
        /// <summary>
        /// The bottom left coordinate of the region.
        /// </summary>
        Coordinate m_bottomLeftCoordinate;
        /// <summary>
        /// The bottom right coordinate of the region.
        /// </summary>
        Coordinate m_bottomRightCoordinate;
    };

    /// <summary>
    /// Represents a terminal RGB color.
    /// </summary>
    class RGBColor final : public Color<RGBColor>
    {
    public:
        /// <summary>
        /// Creates an instance of the RGB color class.
        /// </summary>
        /// <param name="red">The red component of the color.</param>
        /// <param name="green">The green component of the color.</param>
        /// <param name="blue">The blue component of the color.</param>
        /// <param name="blue">The blue component of the color.</param>
        /// <param name="layer">The layer the color applies on.</param>
        RGBColor(unsigned char red, unsigned char green, unsigned char blue, Layer layer);
        /// <summary>
        /// Creates an instance of the RGB color class.
        /// </summary>
        /// <param name="color">The hex color to be converted.</param>
        RGBColor(HexColor color);
        /// <summary>
        /// Inverts the layer the color applies on.
        /// </summary>
        /// <returns>The color with its layer inverted.</returns>
        RGBColor Invert() const;
        /// <summary>
        /// Gets the red component of the coordinate.
        /// </summary>
        /// <returns>The red component of the coordinate.</returns>
        unsigned char GetRed() const;
        /// <summary>
        /// Gets the green component of the coordinate.
        /// </summary>
        /// <returns>The green component of the coordinate.</returns>
        unsigned char GetGreen() const;
        /// <summary>
        /// Gets the blue component of the coordinate.
        /// </summary>
        /// <returns>The blue component of the coordinate.</returns>
        unsigned char GetBlue() const;
        /// <summary>
        /// Sets the red component of the coordinate.
        /// </summary>
        /// <returns>The red component to be set.</returns>
        void SetRed(unsigned char red);
        /// <summary>
        /// Sets the green component of the coordinate.
        /// </summary>
        /// <returns>The green component to be set.</returns>
        void SetGreen(unsigned char green);
        /// <summary>
        /// Sets the blue component of the coordinate.
        /// </summary>
        /// <returns>The blue component to be set.</returns>
        void SetBlue(unsigned char blue);

    private:
        /// <summary>
        /// The red component of the coordinate.
        /// </summary>
        unsigned char m_red;
        /// <summary>
        /// The green component of the coordinate.
        /// </summary>
        unsigned char m_green;
        /// <summary>
        /// The blue component of the coordinate.
        /// </summary>
        unsigned char m_blue;
    };

    /// <summary>
    /// Represents a terminal XTerm color.
    /// </summary>
    class XColor final : public Color<XColor>
    {
    public:
        /// <summary>
        /// Creates a new instance of the XColor class.
        /// </summary>
        /// <param name="code">The ANSI code of the color.</param>
        /// <param name="layer">The layer the color applies on.</param>
        XColor(unsigned char code, Layer layer);
        /// <summary>
        /// Creates a new instance of the XColor class.
        /// </summary>
        /// <param name="code">The ANSI code of the color.</param>
        /// <param name="layer">The layer the color applies on.</param>
        XColor(XColorCode code, Layer layer);
        /// <summary>
        /// Inverts the layer the color applies on.
        /// </summary>
        /// <returns>The color with its layer inverted.</returns>
        XColor Invert() const;
        /// <summary>
        /// Gets the ANSI code of the color.
        /// </summary>
        /// <returns>The ANSI code of the color.</returns>
        short GetCode() const;
        /// <summary>
        /// Sets the ANSI code of the color.
        /// </summary>
        /// <param name="code">The ANSI code to be set.</param>
        void SetCode(unsigned char code);
        /// <summary>
        /// Sets the ANSI code of the color.
        /// </summary>
        /// <param name="code">The ANSI code to be set.</param>
        void SetCode(XColorCode code);

    private:
        /// <summary>
        /// The ANSI code of the color.
        /// </summary>
        short m_code;

        /// <summary>
        /// Filters an ANSI code.
        /// </summary>
        /// <param name="code">The ANSI code to be filtered.</param>
        /// <returns>The ANSI code filtered within a valid range.</returns>
        static short FilterCode(XColorCode code);
    };

    /// <summary>
    /// Applies a group of terminal effects.
    /// </summary>
    /// <param name="stream">The stream to be affected.</param>
    /// <param name="effect">The group of terminal effects to be applied.</param>
    /// <returns>The stream affected.</returns>
    std::ostream& operator<<(std::ostream& stream, Effect effect);
    /// <summary>
    /// Applies a terminal hex color.
    /// </summary>
    /// <param name="stream">The stream to be affected.</param>
    /// <param name="color">The hex color to be applied.</param>
    /// <returns>The stream affected.</returns>
    std::ostream& operator<<(std::ostream& stream, HexColor color);
    /// <summary>
    /// Applies a terminal RGB color.
    /// </summary>
    /// <param name="stream">The stream to be affected.</param>
    /// <param name="color">The RGB color to be applied.</param>
    /// <returns>The stream affected.</returns>
    std::ostream& operator<<(std::ostream& stream, RGBColor color);
    /// <summary>
    /// Applies a terminal XTerm color.
    /// </summary>
    /// <param name="stream">The stream to be affected.</param>
    /// <param name="color">The XTerm color to be applied.</param>
    /// <returns>The stream affected.</returns>
    std::ostream& operator<<(std::ostream& stream, XColor color);
    /// <summary>
    /// Applies a terminal weight color.
    /// </summary>
    /// <param name="stream">The stream to be affected.</param>
    /// <param name="color">The weight to be applied.</param>
    /// <returns>The stream affected.</returns>
    std::ostream& operator<<(std::ostream& stream, Weight weight);
    /// <summary>
    /// Checks if two XTerm colors are equal.
    /// </summary>
    /// <param name="color0">The first color.</param>
    /// <param name="color1">The second color.</param>
    /// <returns>A boolean that states the two colors are equal.</returns>
    bool operator==(XColor color0, XColor color1);
    /// <summary>
    /// Checks if two hex colors are equal.
    /// </summary>
    /// <param name="color0">The first color.</param>
    /// <param name="color1">The second color.</param>
    /// <returns>A boolean that states the two colors are equal.</returns>
    bool operator==(HexColor color0, HexColor color1);
    /// <summary>
    /// Checks if two RGB colors are equal.
    /// </summary>
    /// <param name="color0">The first color.</param>
    /// <param name="color1">The second color.</param>
    /// <returns>A boolean that states the two colors are equal.</returns>
    bool operator==(RGBColor color0, RGBColor color1);
    /// <summary>
    /// Checks if two coordinates are equal.
    /// </summary>
    /// <param name="color0">The first coordinate.</param>
    /// <param name="color1">The second coordinate.</param>
    /// <returns>A boolean that states the two coordinates are equal.</returns>
    bool operator==(Coordinate coordinate0, Coordinate coordinate1);
    /// <summary>
    /// Checks if two region are equal.
    /// </summary>
    /// <param name="color0">The first region.</param>
    /// <param name="color1">The second region.</param>
    /// <returns>A boolean that states the two regions are equal.</returns>
    bool operator==(Region region0, Region region1);
    /// <summary>
    /// Concatenates two ANSI effect codes in order to make a bitmask.
    /// </summary>
    /// <param name="color0">The first ANSI effect code.</param>
    /// <param name="color1">The second ANSI effect code.</param>
    /// <returns>A bitmask containing both effects.</returns>
    int operator|(EffectCode code0, EffectCode code1);

    /// <summary>
    /// Clears the terminal cursor line.
    /// </summary>
    void ClearCursorLine();
    /// <summary>
    /// Clears the terminal input buffer.
    /// </summary>
    void ClearInputBuffer();
    /// <summary>
    /// Clears the terminal window.
    /// </summary>
    void ClearWindow();
    /// <summary>
    /// Closes the terminal alternate window.
    /// </summary>
    void CloseAlternateWindow();
    /// <summary>
    /// Gets the terminal cursor coordinate.
    /// </summary>
    /// <param name="coordinate">The address where the coordinate information will be put into.</param>
    /// <returns>0 if successful or -1 otherwise.</returns>
    int GetCursorCoordinate(Coordinate& coordinate);
    /// <summary>
    /// Gets the terminal window region.
    /// </summary>
    /// <param name="region">The address where the region information will be put into.</param>
    /// <returns>0 if successful or -1 otherwise.</returns>
    int GetWindowRegion(Region& region);
    /// <summary>
    /// Checks if a standard terminal stream is connected to an interactive terminal (TTY).
    /// </summary>
    /// <param name="stream">The stream to be checked.</param>
    /// <returns>A boolean that states the stream is a TTY.</returns>
    bool IsTTY(Stream stream);
    /// <summary>
    /// Opens the terminal alternate window.
    /// </summary>
    void OpenAlternateWindow();
    /// <summary>
    /// Rings the terminal bell.
    /// </summary>
    void RingBell();
    /// <summary>
    /// Sets the terminal cursor coordinate.
    /// </summary>
    /// <param name="column">The column component of the coordinate.</param>
    /// <param name="row">The row component of the coordinate.</param>
    void SetCursorCoordinate(unsigned short column, unsigned short row);
    /// <summary>
    /// Sets the terminal cursor coordinate.
    /// </summary>
    /// <param name="coordinate">The coordinate to be set.</param>
    void SetCursorCoordinate(Coordinate coordinate);
    /// <summary>
    /// Sets the terminal cursor shape.
    /// </summary>
    /// <param name="shape">The shape to be set.</param>
    void SetCursorShape(CursorShape shape);
    /// <summary>
    /// Sets the terminal cursor visibility.
    /// </summary>
    /// <param name="isToShow">A boolean that states the cursor should be visible.</param>
    void SetCursorVisibility(bool isToShow);
    /// <summary>
    /// Sets the terminal window title.
    /// </summary>
    /// <param name="title">The title to be set.</param>
    void SetWindowTitle(std::string title);
}
