#pragma once

#include <functional>
#include <iostream>

/**
 * @brief A simple C++ terminal manipulation library for Windows and Linux. It
 * can handle terminal properties, colors, effects and events readings with
 * UTF-8 encoding.
 */
namespace tdk {
/** @brief Contains the available terminal cursor shapes. */
enum class CursorShape {
  /** @brief Resets the default shape. */
  Default,
  /** @brief The blinking variant of the block shape. */
  BlinkingBlock,
  /** @brief The non-blinking variant of the block shape. */
  Block,
  /** @brief The blinking variant of the underline shape. */
  BlinkingUnderline,
  /** @brief The non-blinking variant of the underline shape. */
  Underline,
  /** @brief The blinking variant of the bar shape. */
  BlinkingBar,
  /** @brief The non-blinking variant of the bar shape. */
  Bar
};

/** @brief Contains the available terminal event types. */
enum class EventType {
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
enum class EffectCode {
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
enum class Layer {
  /** @brief The foreground layer. */
  Foreground = 3,
  /** @brief The background layer. */
  Background
};

/** @brief Contains the standard terminal streams. */
enum class Stream {
  /** @brief Receives data and events. */
  Input,
  /** @brief Outputs regular tense messages. */
  Output,
  /** @brief Outputs error tense messages. */
  Error
};

/**
 * @brief Contains the ANSI code of the first 16 colors of the XTerm palette
 * plus one more color intended for resets.
 */
enum class XColorCode {
  /** @brief Resets the color. */
  Default = -1,
  /** @brief The dark variant of the black color. */
  Black,
  /** @brief The dark variant of the red color. */
  Red,
  /** @brief The dark variant of the green color. */
  Green,
  /** @brief The dark variant of the yellow color. */
  Yellow,
  /** @brief The dark variant of the blue color. */
  Blue,
  /** @brief The dark variant of the magenta color. */
  Magenta,
  /** @brief The dark variant of the cyan color. */
  Cyan,
  /** @brief The dark variant of the white color. */
  White,
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
enum class Weight {
  /** @brief Resets the weight */
  Default,
  /** @brief Usually rendered as bold weight and/or with light colors. */
  Bold,
  /** @brief Usually rendered with faint colors. */
  Dim
};

template <class T> class Color;
class Coordinate;
class Dimensions;
class Effects;
class HexColor;
class RGBColor;
class XColor;

/**
 * @brief Represents a generic terminal color.
 * @tparam T A more specific color type.
 */
template <class T> class Color {
public:
  /**
   * @brief Inverts the layer the color applies on.
   * @returns The color with its layer inverted.
   */
  virtual T invert() const = 0;
  /**
   * @brief Gets the layer the color applies on.
   * @returns The layer of the color.
   */
  Layer getLayer() const;
  /**
   * @brief Sets the layer the color applies on.
   * @param layer The layer to be set.
   */
  void setLayer(Layer layer);

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
  static Layer filterLayer(Layer layer);
};

/** @brief Represents a terminal coordinate. */
class Coordinate final {
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
  unsigned short getColumn() const;
  /**
   * @brief Gets the row component of the coordinate.
   * @returns The row component of the coordinate.
   */
  unsigned short getRow() const;
  /**
   * @brief Sets the column component of the coordinate.
   * @param column The column component to be set.
   */
  void setColumn(unsigned short column);
  /**
   * @brief Sets the row component of the coordinate.
   * @param row The row component to be set.
   */
  void setRow(unsigned short row);

private:
  /** @brief The column component of the coordinate. */
  unsigned short m_column;
  /** @brief The row component of the coordinate. */
  unsigned short m_row;
};

/** @brief Represents terminal dimensions. */
class Dimensions final {
public:
  /** @brief Creates an instance of the Dimensions class. */
  Dimensions();
  /**
   * @brief Creates an instance of the Dimensions class.
   * @param totalColumns The total columns in the dimensions.
   * @param totalRows The total rows in the dimensions.
   */
  Dimensions(unsigned short totalColumns, unsigned short totalRows);
  /**
   * @brief Gets the total columns of the dimensions.
   * @returns The total columns of the dimensions.
   */
  unsigned short getTotalColumns() const;
  /**
   * @brief Gets the total rows of the dimensions.
   * @returns The total rows of the dimensions.
   */
  unsigned short getTotalRows() const;

private:
  /** @brief The total columns in the dimensions. */
  unsigned short m_totalColumns;
  /** @brief The total rows in the dimensions. */
  unsigned short m_totalRows;
};

/** @brief Represents a group of terminal effects. */
class Effects final {
public:
  /**
   * @brief Creates a new instance of the Effects class.
   * @param code The ANSI code of the effect being targeted.
   * @param isToEnable A boolean that states the effect targeted should be
   * enabled.
   */
  Effects(EffectCode code, bool isToEnable);
  /**
   * @brief Creates a new instance of the Effects class.
   * @param code A bitmask containing the ANSI codes of the effects being
   * targeted.
   * @param isToEnable A boolean that states the effects targeted should be
   * enabled.
   */
  Effects(int code, bool isToEnable);
  /**
   * @brief Gets a bitmask containing the ANSI codes of the effects being
   * targeted.
   * @returns A bitmask containing the ANSI codes of the effects being targeted.
   */
  int getCode() const;
  /**
   * @brief Gets a boolean that states the effects targeted should be enabled.
   * @returns A boolean that states the effects targeted should be enabled
   */
  bool getIsToEnable() const;

private:
  /** @brief A bitmask containing the ANSI codes of the effects being targeted.
   */
  int m_code;
  /** @brief A boolean that states the effects targeted should be enabled. */
  bool m_isToEnable;

  /**
   * @brief Filters the ANSI code of the effects given in a bitmask.
   * @param code The bitmask to be filtered.
   * @returns A bitmask containing only the valid effects.
   */
  static int filterCode(int code);
};

/** @brief Represents a terminal hex color. */
class HexColor final : public Color<HexColor> {
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
  HexColor invert() const;
  /**
   * @brief Converts the hex code of the color into a string.
   * @param isUpper A boolean that states the string is upper case.
   * @param hasPrefix A boolean that states the string has the 0x prefix.
   */
  std::string toString(bool isUpper, bool hasPrefix) const;
  /**
   * @brief Gets the hex code of the color.
   * @returns The hex code of the color.
   */
  unsigned int getCode() const;
  /**
   * @brief Sets the hex code of the color.
   * @param code The hex code to be set.
   */
  void setCode(unsigned int code);

private:
  /** @brief The hex code of the color */
  unsigned int m_code;

  /**
   * @brief Filters a hex code.
   * @param code The hex code to be filtered.
   * @returns The hex code filtered within a valid range.
   */
  static unsigned int filterCode(unsigned int code);
};

/** @brief Represents a terminal RGB color. */
class RGBColor final : public Color<RGBColor> {
public:
  /**
   * @brief Creates an instance of the RGB color class.
   * @param red The red component of the color.
   * @param green The green component of the color.
   * @param blue The blue component of the color.
   * @param layer The layer the color applies on.
   */
  RGBColor(unsigned char red, unsigned char green, unsigned char blue,
           Layer layer);
  /**
   * @brief Creates an instance of the RGB color class.
   * @param color The hex color to be converted.
   */
  RGBColor(HexColor color);
  /**
   * @brief Inverts the layer the color applies on.
   * @returns The color with its layer inverted.
   */
  RGBColor invert() const;
  /**
   * @brief Gets the red component of the color.
   * @returns The red component of the color.
   */
  unsigned char getRed() const;
  /**
   * @brief Gets the green component of the color.
   * @returns The green component of the color.
   */
  unsigned char getGreen() const;
  /**
   * @brief Gets the blue component of the color.
   * @returns The blue component of the color.
   */
  unsigned char getBlue() const;
  /**
   * @brief Sets the red component of the color.
   * @returns The red component to be set.
   */
  void setRed(unsigned char red);
  /**
   * @brief Sets the green component of the color.
   * @returns The green component to be set.
   */
  void setGreen(unsigned char green);
  /**
   * @brief Sets the blue component of the color.
   * @returns The blue component to be set.
   */
  void setBlue(unsigned char blue);

private:
  /** @brief The red component of the color. */
  unsigned char m_red;
  /** @brief The green component of the color. */
  unsigned char m_green;
  /** @brief The blue component of the color. */
  unsigned char m_blue;
};

/** @brief Represents a terminal XTerm color. */
class XColor final : public Color<XColor> {
public:
  /**
   * @brief Creates a new instance of the XColor class.
   * @param code The ANSI code of the color.
   * @param layer The layer the color applies on.
   */
  XColor(unsigned char code, Layer layer);
  /**
   * @brief Creates a new instance of the XColor class.
   * @param code The ANSI code of the color.
   * @param layer The layer the color applies on.
   */
  XColor(XColorCode code, Layer layer);
  /**
   * @brief Inverts the layer the color applies on.
   * @returns The color with its layer inverted.
   */
  XColor invert() const;
  /**
   * @brief Gets the ANSI code of the color.
   * @returns The ANSI code of the color.
   */
  short getCode() const;
  /**
   * @brief Sets the ANSI code of the color.
   * @param code The ANSI code to be set.
   */
  void setCode(unsigned char code);
  /**
   * @brief Sets the ANSI code of the color.
   * @param code The ANSI code to be set.
   */
  void setCode(XColorCode code);

private:
  /** @brief The ANSI code of the color. */
  short m_code;

  /**
   * @brief Filters an ANSI code.
   * @param code The ANSI code to be filtered.
   * @returns The ANSI code filtered within a valid range.
   */
  static short filterCode(XColorCode code);
};

/**
 * @brief Applies a group of terminal effects.
 * @param stream The stream to be affected.
 * @param effects The group of terminal effects to be applied.
 * @returns The stream affected.
 */
std::ostream &operator<<(std::ostream &stream, Effects effects);
/**
 * @brief Applies a terminal hex color.
 * @param stream The stream to be affected.
 * @param color The color to be applied.
 * @returns The stream affected.
 */
std::ostream &operator<<(std::ostream &stream, HexColor color);
/**
 * @brief Applies a terminal RGB color.
 * @param stream The stream to be affected.
 * @param color The color to be applied.
 * @returns The stream affected.
 */
std::ostream &operator<<(std::ostream &stream, RGBColor color);
/**
 * @brief Applies a terminal XTerm color.
 * @param stream The stream to be affected.
 * @param color The color to be applied.
 * @returns The stream affected.
 */
std::ostream &operator<<(std::ostream &stream, XColor color);
/**
 * @brief Applies a terminal weight.
 * @param stream The stream to be affected.
 * @param color The color to be applied.
 * @returns The stream affected.
 */
std::ostream &operator<<(std::ostream &stream, Weight weight);
/**
 * @brief Checks if two XTerm colors are equal.
 * @param color0 The first color.
 * @param color1 The second color.
 * @returns A boolean that states the two colors are equal.
 */
bool operator==(XColor color0, XColor color1);
/**
 * @brief Checks if two hex color are equal.
 * @param color0 The first color.
 * @param color1 The second color.
 * @returns A boolean that states the two colors are equal.
 */
bool operator==(HexColor color0, HexColor color1);
/**
 * @brief Checks if two RGB color are equal.
 * @param color0 The first color.
 * @param color1 The second color.
 * @returns A boolean that states the two colors are equal.
 */
bool operator==(RGBColor color0, RGBColor color1);
/**
 * @brief Checks if a hex color is equal to an RGB color.
 * @param hexColor The hex color.
 * @param rgbColor The RGB color.
 */
bool operator==(HexColor hexColor, RGBColor rgbColor);
/**
 * @brief Checks if an RGB color is equal to a hex color.
 * @param rgbColor The RGB color.
 * @param hexColor The hex color.
 */
bool operator==(RGBColor rgbColor, HexColor hexColor);
/**
 * @brief Checks if two coordinates are equal.
 * @param coordinate0 The first coordinate.
 * @param coordinate1 The second coordinate.
 * @returns A boolean that states the two coordinates are equal.
 */
bool operator==(Coordinate coordinate0, Coordinate coordinate1);
/**
 * @brief Concatenates two ANSI effect codes in order to make a bitmask.
 * @param code0 The first ANSI effect code.
 * @param code1 The second ANSI effect code.
 * @returns A bitmask containing both effects.
 */
int operator|(EffectCode code0, EffectCode code1);

/** @brief Clears the terminal cursor line. */
void clearCursorLine();
/** @brief Clears the terminal input buffer. */
void clearInputBuffer();
/** @brief Clears the terminal window. */
void clearWindow();
/** @brief Closes the terminal alternate window. */
void closeAlternateWindow();
/**
 * @brief Gets the terminal cursor coordinate.
 * @param coordinate The address where the coordinate information will be put
 * into.
 * @returns 0 if successful or -1 otherwise.
 */
int getCursorCoordinate(Coordinate &coordinate);
/**
 * @brief Gets the terminal window dimensions.
 * @param dimensions The address where the dimensions information will be put
 * into.
 * @returns 0 if successful or -1 otherwise.
 */
int getWindowDimensions(Dimensions &dimensions);
/**
 * @brief Checks if a standard terminal stream is connected to an interactive
 * terminal (TTY).
 * @param stream The stream to be checked.
 * @returns A boolean that states the stream is a TTY.
 */
bool isTTY(Stream stream);
/** @brief Opens the terminal alternate window. */
void openAlternateWindow();
/** @brief Rings the terminal bell. */
void ringBell();
/**
 * @brief Sets the terminal cursor coordinate.
 * @param column The column component of the coordinate.
 * @param row The row component of the coordinate.
 */
void setCursorCoordinate(unsigned short column, unsigned short row);
/**
 * @brief Sets the terminal cursor coordinate.
 * @param coordinate The coordinate to be set.
 */
void setCursorCoordinate(Coordinate coordinate);
/**
 * @brief Sets the terminal cursor shape.
 * @param shape The shape to be set.
 */
void setCursorShape(CursorShape shape);
/**
 * @brief Sets the terminal cursor visibility.
 * @param isToShow A boolean that states the cursor should be visible.
 */
void setCursorVisibility(bool isToShow);
/**
 * @brief Sets the terminal window title.
 * @param title The title to be set.
 */
void setWindowTitle(std::string title);
} // namespace tdk
