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
    class Effects;
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

    /** @brief Represents a terminal coordinate. */
    class Coordinate final
    {
    public:
        /** @brief Creates a new instance of the Coordinate class. */
        Coordinate();
        /**
         * @brief Creates a new instance of the Coordinate class.
         * @param column The column component of the coordinate.
         * @param row The row component of the coordinate.
         */
        Coordinate(unsigned short column, unsigned short row);
        /**
         * @brief Gets the column component of the coordinate.
         * @returns The column component of the coordinate.
         */
        unsigned short GetColumn() const;
        /**
         * @brief Gets the row component of the coordinate.
         * @returns The row component of the coordinate.
         */
        unsigned short GetRow() const;
        /**
         * @brief Sets the column component of the coordinate.
         * @param column The column component to be set.
         */
        void SetColumn(unsigned short column);
        /**
         * @brief Sets the row component of the coordinate.
         * @param row The row component to be set.
         */
        void SetRow(unsigned short row);

    private:
        /** @brief The column component of the coordinate. */
        unsigned short m_column;
        /** @brief The row component of the coordinate. */
        unsigned short m_row;
    };

    /** @brief Represents a group of terminal effects. */
    class Effects final
    {
    public:
        /**
         * @brief Creates a new instance of the Effects class.
         * @param code The ANSI code of the effect.
         * @param isToEnable A boolean that states the effect targeted should be enabled.
         */
        Effects(EffectCode code, bool isToEnable);
        /**
         * @brief Creates a new instance of the Effects class.
         * @param code A bitmask containing the ANSI code of the effects.
         * @param isToEnable A boolean that states the effects targeted should be enabled.
         */
        Effects(int code, bool isToEnable);

        /**
         * @brief Gets a bitmask containing the ANSI codes of the effects being targeted.
         * @returns A bitmask containing the ANSI codes of the effects being targeted.
         */
        int GetCode() const;
        /**
         * Gets a boolean that states the effects targeted should be enabled.
         * @returns A boolean that states the effects targeted should be enabled
         */
        bool IsToEnable() const;

    private:
        /** @brief A bitmask containing the ANSI codes of the effects being targeted. */
        int m_code;
        /** @brief A boolean that states the effects targeted should be enabled. */
        bool m_isToEnable;

        /**
         * @brief Filters the ANSI code of the effects given in a bitmask.
         * @param code The bitmask to be filtered.
         * @returns A bitmask containing only the valid effects.
         */
        static int FilterCode(int code);
    };

    /** @brief Represents a terminal hex color. */
    class HexColor final : public Color<HexColor>
    {
    public:
        /**
         * @brief Creates an instance of the HexColor class.
         * @param code The hex code of the color.
         * @param layer The layer the color applies on.
         */
        HexColor(unsigned int code, Layer layer);
        /**
         * @brief Creates an instance of the HexColor class.
         * @param color The RGB color to be converted.
         */
        HexColor(RGBColor color);
        /**
         * @brief Inverts the layer the color applies on.
         * @returns The color with its layer inverted.
         */
        HexColor Invert() const;
        /**
         * @brief Gets the hex code of the color.
         * @returns The hex code of the color.
         */
        unsigned int GetCode() const;
        /**
         * @brief Sets the hex code of the color.
         * @param code The hex code to be set.
         */
        void SetCode(unsigned int code);

    private:
        /** @brief The hex code of the color */
        unsigned int m_code;

        /**
         * @brief Filters a hex code.
         * @param code The hex code to be filtered.
         * @returns The hex code filtered within a valid range.
         */
        static unsigned int FilterCode(unsigned int code);
    };

    /** @brief Represents a terminal region. */
    class Region final
    {
    public:
        /** @brief Creates a new instance of the Region class */
        Region();
        /**
         * @brief Creates a new instance of the Region class.
         * @param totalColumns The total columns of the region.
         * @param totalRows The total rows of the region.
         */
        Region(unsigned short totalColumns, unsigned short totalRows);
        /**
         * @brief Creates a new instance of the Region class.
         * @param cornerCoordinate0 A first corner coordinate of the region.
         * @param cornerCoordinate1 A second corner coordinate of the region, opposite to the first one.
         */
        Region(Coordinate cornerCoordinate0, Coordinate cornerCoordinate1);
        /**
         * @brief Checks if a coordinate is contained within the region.
         * @param column The column component of the coordinate.
         * @param row The row component of the coordinate.
         * @returns A boolean that states the coordinate is contained within the region.
         */
        bool Contains(unsigned short column, unsigned short row) const;
        /**
         * @brief Checks if a coordinate is contained within the region.
         * @param coordinate The coordinate to be checked.
         * @returns A boolean that states the coordinate is contained within the region.
         */
        bool Contains(Coordinate coordinate) const;
        /**
         * @brief Gets the total columns of the region.
         * @returns The total columns of the region.
         */
        unsigned short GetTotalColumns() const;
        /**
         * @brief Gets the total rows of the region.
         * @returns The total rows of the region.
         */
        unsigned short GetTotalRows() const;
        /**
         * @brief Gets the area of the region.
         * @returns The area of the region.
         */
        unsigned int GetArea() const;
        /**
         * @brief Gets the top left coordinate of the region.
         * @returns The top left coordinate of the region.
         */
        Coordinate GetTopLeftCoordinate() const;
        /**
         * @brief Gets the top right coordinate of the region.
         * @returns The top right coordinate of the region.
         */
        Coordinate GetTopRightCoordinate() const;
        /**
         * @brief Gets the bottom left coordinate of the region.
         * @returns The bottom left coordinate of the region.
         */
        Coordinate GetBottomLeftCoordinate() const;
        /**
         * @brief Gets the bottom right coordinate of the region.
         * @returns The bottom right coordinate of the region.
         */
        Coordinate GetBottomRightCoordinate() const;

    private:
        /** @brief The total columns of the region. */
        unsigned short m_totalColumns;
        /** @brief The total rows of the region. */
        unsigned short m_totalRows;
        /** @brief The area of the region. */
        unsigned int m_area;
        /** @brief The top left coordinate of the region. */
        Coordinate m_topLeftCoordinate;
        /** @brief The top right coordinate of the region. */
        Coordinate m_topRightCoordinate;
        /** @brief the bottom left coordinate of the region. */
        Coordinate m_bottomLeftCoordinate;
        /** @brief The bottom right coordinate of the region. */
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
    std::ostream& operator<<(std::ostream& stream, Effects effect);
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
