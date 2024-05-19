#pragma once

#include <cstdarg>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#else
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif

namespace tdk {
/*
 * @brief An enum class containing the available terminal cursor shapes. Apply
 * one by using the setCursorShape function.
 */
enum class Shape {
  /* @brief The default cursor shape, intended for resets. */
  Default,
  /* @brief The blinking variant of the block shape. */
  BlinkingBlock,
  /* @brief The non-blinking variant of the block shape. */
  Block,
  /* @brief The blinking variant of the underline shape. */
  BlinkingUnderline,
  /* @brief The non-blinking variant of the underline shape. */
  Underline,
  /* @brief The blinking variant of the bar shape. */
  BlinkingBar,
  /* @brief The non-blinking variant of the bar shape. */
  Bar
};

/*
 * @brief An enum class containing the available terminal text effects. Apply
 * one by using the left shifting operator (<<) against an instance of the
 * std::ostream class. Remove it by using the right shifting operator (>>)
 * instead.
 */
enum class Effect {
  /*
   * @brief The italic effect makes the text curly. It may require the use of a
   * font with italic weight in order to be seen.
   */
  Italic = 3,
  /* @brief The underline effect draws a horizontal line below the text. */
  Underline,
  /* @brief The blinking effect makes the text blink indefinitely. */
  Blinking,
  /*
   * @brief The reverse-video effect swaps the foreground and background colors.
   */
  ReverseVideo = 7,
  /* @brief The conceal effect makes the text hard to see or invisible. */
  Conceal,
  /*
   * @brief The strikethrough effect draws a horizontal line in the middle of
   * the text.
   */
  Strikethrough
};

/*
 * @brief An enum class containing mostly keyboard keys not represented by UTF-8
 * graphemes. For exceptions, the values here serve the purpose of increase
 * readability. They are used to compose the information retrieved by the
 * readKeyEvent function.
 */
enum class Key {
#ifdef _WIN32
  /* @brief The left arrow key. */
  LeftArrow = -23,
  /* @brief The right arrow key. */
  UpArrow,
  /* @brief The right arrow key. */
  RightArrow,
  /* @brief The down arrow key. */
  DownArrow,
#else
  /* @brief The up arrow key. */
  UpArrow = -23,
  /* @brief The down arrow key. */
  DownArrow,
  /* @brief The right arrow key. */
  RightArrow,
  /* @brief The left arrow key. */
  LeftArrow,
#endif
  /* @brief The F1 key. */
  F1,
  /* @brief The F2 key. */
  F2,
  /* @brief The F3 key. */
  F3,
  /* @brief The F4 key. */
  F4,
  /* @brief The F5 key. */
  F5,
  /* @brief The F6 key. */
  F6,
  /* @brief The F7 key. */
  F7,
  /* @brief The F8 key. */
  F8,
  /* @brief The F9 key. */
  F9,
  /* @brief The F10 key. */
  F10,
  /* @brief The F11 key. */
  F11,
  /* @brief The F12 key. */
  F12,
#ifdef _WIN32
  /* @brief The PageUp key. */
  PageUp,
  /* @brief The PageDown key. */
  PageDown,
  /* @brief The End key. */
  End,
  /* @brief The Home key. */
  Home,
  /* @brief The Insert key. */
  Insert,
  /* @brief The Delete key. */
  Delete,
#else
  /* @brief The Home key. */
  Home,
  /* @brief The Insert key. */
  Insert,
  /* @brief The Delete key. */
  Delete,
  /* @brief The End key. */
  End,
  /* @brief The PageUp key. */
  PageUp,
  /* @brief The PageDown key. */
  PageDown,
#endif
  /* @brief The Tab key. */
  Tab = 9,
#if defined(_WIN32) || defined(__APPLE__)
  /* @brief The Enter key. */
  Enter = 13,
#else
  /* @brief The Enter key. */
  Enter,
#endif
  /* @brief The Escape key. */
  Escape = 27,
  /* @brief The Spacebar key. */
  Spacebar = 32,
  /* @brief The Backspace key. */
  Backspace = 127
};

/*
 * @brief An enum class containing the possible statuses of a terminal key event
 * reading. It is returned by the readKeyEvent function.
 */
enum class EventStatus {
  /* @brief The sucess status states the reading succeeded. */
  Success,
  /*
   * @brief Only happening on Windows, the window resize interrupt status states
   * the key event was interrupt by a window resize event.
   */
  WindowResizeInterrupt = -2,
  /* @brief The failure status states the reading failed. */
  Failure
};

/*
 * @brief An enum class containing the available terminal layers. They are used
 * to create instances of the XColor, HexColor and RGBColor classes.
 */
enum class Layer {
  /* @brief The foreground layer. */
  Foreground = 3,
  /* @brief The background layer. */
  Background
};

/*
 * @brief An enum class containing the standard terminal streams. They are meant
 * to be used by the isTTY function and by internal implementations.
 */
enum class Stream {
  /* @brief The standard input stream (stdin). */
  Input,
  /* @brief The standard output stream (stdout). */
  Output,
  /* @brief The standard error stream (stderr). */
  Error
};

/*
 * @brief An enum class containing the ANSI codes of the first 16 colors of the
 * XTerm palette plus one more color intended for resets. They are intended to
 * be used to create instances of the XColor class.
 */
enum class XColorCode {
  /* @brief The default color, intended for resets. */
  Default = -1,
  /* @brief The regular variant of the black color. */
  Black,
  /* @brief The regular variant of the red color. */
  Red,
  /* @brief The regular variant of the green color. */
  Green,
  /* @brief The regular variant of the yellow color. */
  Yellow,
  /* @brief The regular variant of the blue color. */
  Blue,
  /* @brief The regular variant of the magenta color. */
  Magenta,
  /* @brief The regular variant of the cyan color. */
  Cyan,
  /* @brief The regular variant of the white color. */
  White,
  /* @brief The light variant of the black color. */
  LightBlack,
  /* @brief The light variant of the red color. */
  LightRed,
  /* @brief The light variant of the green color. */
  LightGreen,
  /* @brief The light variant of the yellow color. */
  LightYellow,
  /* @brief The light variant of the blue color. */
  LightBlue,
  /* @brief The light variant of the magenta color. */
  LightMagenta,
  /* @brief The light variant of the cyan color. */
  LightCyan,
  /* @brief The light variant of the white color. */
  LightWhite
};

/*
 * @brief An enum class containing the available terminal weights. Apply one by
 * using the left shifting operator (<<) against an instance of the std::ostream
 * class.
 */
enum class Weight {
  /* @brief The default weight, intended for resets. */
  Default,
  /*
   * @brief The bold weight is usually rendered with bold weight and/or with
   * light colors. It may require the use of a font with bold weight in order to
   * be seen.
   */
  Bold,
  /* @brief The dim weight is usually rendered with faint colors. */
  Dim
};

/*
 * @brief A class that represents a terminal coordinate. It is usually used to
 * refer to the cursor one by using the getCursorCoordinate and
 * setCursorCoordinate functions.
 */
class Coordinate {
public:
  /* @brief Creates an instance of the Coordinate class. */
  Coordinate();
  /*
   * @brief Creates an instance of the Coordinate class.
   * @param column The column component of the coordinate.
   * @param row The row component of the coordinate.
   */
  Coordinate(unsigned short column, unsigned short row);

  /* @brief The column component of the coordinate. */
  unsigned short m_column;
  /* @brief The row component of the coordinate. */
  unsigned short m_row;
};

/*
 * @brief A class that represents terminal dimensions. It is usually used to
 * refer to the window one by using the getWindowDimensions function.
 */
class Dimensions {
public:
  /* @brief Creates an instance of the Dimensions class. */
  Dimensions();
  /*
   * @brief Creates an instance of the Dimensions class.
   * @param totalColumns The total columns of the dimensions.
   * @param totalRows The total rows of the dimensions.
   */
  Dimensions(unsigned short totalColumns, unsigned short totalRows);

  /* @brief The total columns of the dimensions. */
  unsigned short m_totalColumns;
  /* @brief the total rows of the dimensions. */
  unsigned short m_totalRows;
};

/* @brief A class that represents a hex color. Apply it by using the left
 * shifting operator (<<) against an instance of the std::ostream class. Remove
 * it by applying an instance of the XColor class with code XColorCode::Default
 * targetting the same layer where it has been applied instead.
 */
class HexColor {
public:
  /*
   * @brief Creates an instance of the HexColor class.
   * @param code The hex code of the color. It must be a value between 0x0 to
   * 0xffffff.
   * @param layer The layer where it should be applied.
   */
  HexColor(unsigned int code, Layer layer);
  /* @brief Inverts the layer where it is being applied. */
  HexColor m_invert();

  /* @brief The hex code of the color. It must be a value between 0x0 to
   * 0xffffff. */
  unsigned int m_code;
  /* @brief The layer where it should be applied. */
  Layer m_layer;
};

/*
 * @brief A class that represents a terminal key event. It is used by the
 * readKeyEvent function.
 */
class KeyEvent {
public:
  /* @brief Creates an instance of the KeyEvent class. */
  KeyEvent();

  /*
   * @brief The key pressed during the event. It may be an UTF-8 grapheme or a
   * value from the Key enum.
   */
  int m_key;
  /* @brief A boolean that states the Alt key was being hold during the event.
   */
  bool m_hasAlt;
  /* @brief A boolean that states the Ctrl key was being hold during the event.
   */
  bool m_hasCtrl;
};

/*
 * @brief A class that represents an RGB color. Apply it by using the left
 * shifting operator (<<) against an instance of the std::ostream class. Remove
 * it by applying an instance of the XColor class with code XColorCode::Default
 * targetting the same layer where it has been applied instead.
 */
class RGBColor {
public:
  /*
   * @brief Creates an instance of the RGBColor class.
   * @param red The red component of the color.
   * @param green The green component of the color.
   * @param blue The blue component of the color.
   */
  RGBColor(unsigned char red, unsigned char green, unsigned char blue,
           Layer layer);
  /*
   * @brief Creates an instance of the RGBColor class.
   * @param color A hex color to be converted.
   */
  RGBColor(HexColor color);
  /* @brief Inverts the layer where it is being applied. */
  RGBColor m_invert();

  /* @brief The red component of the color. */
  unsigned char m_red;
  /* @brief The green component of the color. */
  unsigned char m_green;
  /* @brief The blue component of the color. */
  unsigned char m_blue;
  /* @brief The layer where the color should be applied. */
  Layer m_layer;
};

/*
 * @brief A class that represents a color from the XTerm palette: a terminal
 * exclusive palette that contains 256 colors.
 */
class XColor {
public:
  /*
   * @brief Creates an instance of the XColor class.
   * @param code The ANSI code of the color.
   * @param layer The layer where the color should be applied on.
   */
  XColor(unsigned char code, Layer layer);
  /*
   * @brief Creates an instance of the XColor class.
   * @param code The ANSI code of the color.
   * @param layer The layer where the color should be applied on.
   */
  XColor(XColorCode code, Layer layer);
  XColor m_invert();

  /* @brief The ANSI code of the color. */
  int m_code;
  /* @brief The layer where the color should be applied on. */
  Layer m_layer;
};

/*
 * @brief Applies a terminal text effect.
 * @param stream The stream to be affected.
 * @param effect The effect to be applied.
 */
std::ostream &operator<<(std::ostream &stream, Effect effect);
/*
 * @brief Removes a terminal text effect.
 * @param stream The stream to be affected.
 * @param effect The effect to be removed.
 */
std::ostream &operator>>(std::ostream &stream, Effect effect);
/*
 * @brief Applies a hex color.
 * @param stream The stream to be affected.
 * @param color The color to be applied.
 */
std::ostream &operator<<(std::ostream &stream, HexColor color);
/*
 * @brief Applies an RGB color.
 * @param stream The stream to be affected.
 * @param color The RGB color to be applied.
 */
std::ostream &operator<<(std::ostream &stream, RGBColor color);
/*
 * @brief Applies an XTerm color.
 * @param stream The stream to be affected.
 * @param color The XTerm color to be applied.
 */
std::ostream &operator<<(std::ostream &stream, XColor color);
/*
 * @brief Applies a terminal weight.
 * @param stream The stream to be affected.
 * @param weight The weight to be applied.
 */
std::ostream &operator<<(std::ostream &stream, Weight weight);
/*
 * @brief Checks if a key is of the same code.
 * @param code The code to be used as reference.
 * @param key The key to be checked.
 */
bool operator==(int code, Key key);
/*
 * @brief Checks if a key is not of the same code.
 * @param code The code to be used as reference.
 * @param key The key to be checked.
 */
bool operator!=(int code, Key key);

void clearCursorLine();
void clearInputBuffer();
int getCursorCoordinate(Coordinate &coordinate);
int getWindowDimensions(Dimensions &dimensions);
bool isTTY(Stream stream);
EventStatus readKeyEvent(KeyEvent &event);
void ringBell();
void setAlternateWindow(bool isToOpen);
void setCursorCoordinate(unsigned short column, unsigned short row);
void setCursorCoordinate(Coordinate &coordinate);
void setCursorShape(Shape shape);
void setCursorVisibility(bool isToShow);
}
