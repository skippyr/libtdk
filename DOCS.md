<p align="center">
  <img alt="" src="assets/ornament.webp" />
</p>
<h1 align="center">≥v≥v&ensp;libtdk&ensp;≥v≥v</h1>
<p align="center">Terminal Development Kit - Documentation - v13.0.0</p>
<p align="center">
  <img alt="" src="https://img.shields.io/github/license/skippyr/libtdk?style=plastic&label=%E2%89%A5%20license&labelColor=%2324130e&color=%23b8150d" />
  &nbsp;
  <img alt="" src="https://img.shields.io/github/v/tag/skippyr/libtdk?style=plastic&label=%E2%89%A5%20tag&labelColor=%2324130e&color=%23b8150d" />
  &nbsp;
  <img alt="" src="https://img.shields.io/github/commit-activity/t/skippyr/libtdk?style=plastic&label=%E2%89%A5%20commits&labelColor=%2324130e&color=%23b8150d" />
  &nbsp;
  <img alt="" src="https://img.shields.io/github/stars/skippyr/libtdk?style=plastic&label=%E2%89%A5%20stars&labelColor=%2324130e&color=%23b8150d" />
</p>

## ❡ About

This document will give you an overview about certain topics related to terminal manipulation, showing you how to use the library to handle them, and it will also include the documentation of each of its components.

The recommended way of reading it is directly on GitHub. That way as you will be able to use its **Outline** feature available at the top right corner of the website to access each section more easily.

## ❡ Usage Limitation

The library has some limitations over its usage. To avoid conflicts and malfunctions, you must follow these rules:

- It uses byte oriented functions and operator overloadings from the standard library for UTF-8 encoding support. Thus, all standard streams must be kept unoriented or under that same orientation. Do not use wide character functions.

- It does not changes the Window encoding. You need to perform that change manually by using the [`SetConsoleOutputCP`](https://learn.microsoft.com/en-us/windows/console/setconsoleoutputcp) function with code page [`CP_UTF8`](https://learn.microsoft.com/en-us/windows/win32/intl/code-page-identifiers) at the start of your program in order to write UTF-8 graphemes.

- It does not provide full resets of terminal properties. Do not manipulate them manually, unless you know what you are doing.


## ❡ Namespace

The library reserves names under the `tdk` namespace. To avoid naming conflicts, it is highly recommend to not short them by using the `using namespace` keywords.

## ❡ Colors

You can set terminals colors into layers using different color formats. Available layers are contained in the [`Layer`](#layer-enum-class) enum class and each color  format accepted is a class:

- [`HexColor`](#hexcolor-class): refers to a hexadecimal color.
- [`RGBColor`](#rgbcolor-class): refers to an RGB color.
- [`XColor`](#xcolor-class): refers to a color from the XTerm palette: a terminal exclusive palette containing 256 colors. The ANSI code of the first 16 colors of this palette plus one more color used for resets are contained in the [`XColorCode`](#xcolorcode-enum-class) enum class. These codes map to colors of the active terminal theme and can be used to ensure consistency.

Apply one by using the left shifting operator (`<<`) with an instance of a color class against an instance of the [`std::ostream`](https://cplusplus.com/reference/ostream/ostream) class. Remove it by applying an instance of the [`XColor`](#xcolor-class) class constructed with the ANSI code [`XColorCode::Default`](#xcolorcode-enum-class) in the layer where the color has been applied.

The following example demonstrates how to use colors:

```cpp
#include <iomanip>

#include <tdk.hpp>

std::string g_message = "Here Be Dragons!";

template <typename T> static void writeColorDemo(T foregroundColor) {
  std::cout << foregroundColor << g_message
            << tdk::XColor(tdk::XColorCode::Default, tdk::Layer::Foreground)
            << " " << foregroundColor.m_invert() << g_message
            << tdk::XColor(tdk::XColorCode::Default, tdk::Layer::Background)
            << std::endl;
}

int main() {
  std::cout << std::left << std::setw(g_message.length()) << "Foreground"
            << std::right << " Background" << std::endl
            << std::string(g_message.length(), '-') << " "
            << std::string(g_message.length(), '-') << std::endl;
  writeColorDemo(tdk::XColor(tdk::XColorCode::Red, tdk::Layer::Foreground));
  writeColorDemo(tdk::HexColor(0xe0a100, tdk::Layer::Foreground));
  writeColorDemo(
      tdk::RGBColor(tdk::HexColor(0xa16205, tdk::Layer::Foreground)));
  return 0;
}
```

## ❡ Weights

Availabe text weights are contained inside of the [`Weight`](#weight-enum-class) enum class. Apply one by using the left shifting operator (`<<`) against an instance of the [`std::ostream`](https://cplusplus.com/reference/ostream/ostream) class.

The following example demonstrates how to use weights:

```cpp
#include <array>
#include <iomanip>

#include <tdk.hpp>

int main() {
  std::string message = "Here Be Dragons!";
  std::array<std::string, 3> labels = {"Default", "Bold", "Dim"};
  std::array<tdk::Weight, 3> weights = {tdk::Weight::Default, tdk::Weight::Bold,
                                        tdk::Weight::Dim};
  int padding = 7;
  std::cout << std::left << std::setw(padding) << "Weight" << std::right
            << " Preview" << std::endl
            << std::string(padding, '-') << " "
            << std::string(message.length(), '-') << std::endl;
  for (int offset = 0; offset < labels.size(); ++offset) {
    std::cout << std::left << std::setw(padding) << labels[offset] << std::right
              << " " << weights[offset] << message << tdk::Weight::Default
              << std::endl;
  }
  return 0;
}
```

## ❡ Effects

Available text effects are contained inside of the [`Effect`](#effect-enum-class) enum class. Apply one by using the left shifting operator (`<<`) against an instance of the [`std::ostream`](https://cplusplus.com/reference/ostream/ostream) class. Remove it by using the right shifting operator (`>>`) instead.

The following example demonstrates how to use effects:

```cpp
#include <array>
#include <iomanip>

#include <tdk.hpp>

int main() {
  std::string message = "Here Be Dragons!";
  std::array<std::string, 6> labels = {
      "Italic", "Underline", "Blinking", "Negative", "Hidden", "Strikethrough"};
  std::array<tdk::Effect, 6> effects = {
      tdk::Effect::Italic,   tdk::Effect::Underline,
      tdk::Effect::Blinking, tdk::Effect::Negative,
      tdk::Effect::Hidden,   tdk::Effect::Strikethrough};
  int padding = 13;
  std::cout << std::left << std::setw(padding) << "Effect" << std::right
            << " Preview" << std::endl
            << std::string(padding, '-') << " "
            << std::string(message.length(), '-') << std::endl;
  for (int offset = 0; offset < labels.size(); ++offset) {
    std::cout << std::left << std::setw(padding) << labels[offset] << std::right
              << " " << effects[offset] << message >>
        effects[offset] << std::endl;
  }
  return 0;
}
```

## ❡ Standard Streams

The terminal streams are contained in the [`Stream`](#streams-enum-class) enum class. You can check if they are connected to an interactive terminal by using the [`isTTY`](#istty-function) function.

The standard input buffer can be cleared by using the [`clearInputBuffer`](#clearinputbuffer-function).

The following example demonstrates how to check their TTY statuses:

```cpp
#include <iomanip>

#include <tdk.hpp>

#define STATUS(a_stream)                                                       \
  std::left << std::setw(6) << #a_stream << std::right << " "                  \
            << tdk::XColor(status ? tdk::XColorCode::Green                     \
                                  : tdk::XColorCode::Red,                      \
                           tdk::Layer::Foreground)                             \
            << ((status = tdk::isTTY(tdk::Stream::a_stream))                   \
                    ? "Connected to TTY"                                       \
                    : "Redirected or Piped")                                   \
            << tdk::XColor(tdk::XColorCode::Default, tdk::Layer::Foreground)   \
            << std::endl

int main() {
  bool status;
  std::cout << std::left << std::setw(6) << "Stream" << std::right << " Status"
            << std::endl
            << std::string(6, '-') << " " << std::string(19, '-') << std::endl
            << STATUS(Input) << STATUS(Output) << STATUS(Error);
  return 0;
}
```

## ❡ Key Events

You can use the [`readkeyEvent`](#readkeyevent-function) function to read key events from the standard input buffer. Exclusively on Windows, it can be interrupted by a window resize event that might happen in the same event loop. It holds the thread until a valid key event is pulled or it is interrupted.

The key read may be an UTF-8 grapheme or a key value from the [`Key`](#key-enum-class) enum class static casted to an `int` type. Values from the [`Key`](#key-enum-class) enum can be compared directly against the key by using comparasion operators such as `==`, `>`, `>=`, `<` and `<=`.

As a limitation of this function: modifier keys are only allowed to be detected in keys of the English alphabet (lowercase (`a-z`) and uppercase (`A-Z`) letters), as those are the ranges in which terminals offer the greatest support. The Shift modifier key is intended to be detected by checking the key received: For example, lowercase letters become uppercase when it is used.

There may be some key sequences in which the function will be unable to distinguish the keys used. For example: the sequences `Ctrl + i` and `Ctrl + j` are the same as keys `Key::Backspace` and `Key::Enter` respectively because they have the same underlying code sent by the terminal. For the same reason, keys with `Ctrl` can not have the `Shift` key detected. Some other key sequences may be reserved by the terminal.

The following example shows demonstrates how to read and parse a key event and catch the window resize interrupt:

```cpp
#include <tdk.hpp>

static void writeListItem(std::string item) {
  std::cout << tdk::Weight::Bold
            << tdk::XColor(tdk::XColorCode::Red, tdk::Layer::Foreground)
            << "  - " << tdk::Weight::Default
            << tdk::XColor(tdk::XColorCode::Default, tdk::Layer::Foreground)
            << item << "." << std::endl;
}

static void throwError(std::string message) {
  std::cout << tdk::XColor(tdk::XColorCode::Red, tdk::Layer::Foreground)
            << tdk::Weight::Bold << "[ERROR] " << tdk::Weight::Default
            << tdk::XColor(tdk::XColorCode::Default, tdk::Layer::Foreground)
            << message << std::endl;
  std::exit(1);
}

int main() {
  tdk::KeyEvent keyEvent;
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
#endif
  std::cout << "Enter one of the following keyboard key sequences or grapheme:"
            << std::endl;
  writeListItem("Ctrl + A");
  writeListItem("Alt + B");
  writeListItem("Shift + C");
  writeListItem("Alt + Shift + D");
  writeListItem("Any function key (F1 to F12)");
  writeListItem("Up arrow key");
  writeListItem("Dragon emoji 🐉");
  std::cout << std::endl;
  tdk::clearInputBuffer();
  if (tdk::readKeyEvent(keyEvent) == tdk::EventStatus::WindowResizeInterrupt) {
    throwError("The key reading was interrupted by a window resize event.");
  }
  std::cout << "You entered: ";
  if (keyEvent.m_key == 'a' && keyEvent.m_hasCtrl && !keyEvent.m_hasAlt) {
    std::cout << "Ctrl + A.";
  } else if (keyEvent.m_key == 'b' && !keyEvent.m_hasCtrl &&
             keyEvent.m_hasAlt) {
    std::cout << "Alt + B.";
  } else if (keyEvent.m_key == 'C' && !keyEvent.m_hasCtrl &&
             !keyEvent.m_hasAlt) {
    std::cout << "Shift + C.";
  } else if (keyEvent.m_key == 'D' && !keyEvent.m_hasCtrl &&
             keyEvent.m_hasAlt) {
    std::cout << "Alt + Shift + D.";
  } else if (keyEvent.m_key >= tdk::Key::F1 &&
             keyEvent.m_key <= tdk::Key::F12) {
    std::cout << "a function key.";
  } else if (keyEvent.m_key == tdk::Key::UpArrow) {
    std::cout << "the up arrow key.";
  } else if (keyEvent.m_key == *reinterpret_cast<const int *>("🐉")) {
    std::cout << "the dragon emoji.";
  } else {
    std::cout << "a non-programmed key sequence or grapheme.";
  }
  std::cout << std::endl;
  return 0;
}
```

## ❡ Cursor

The terminal cursor coordinate can be get and set by using the [`getCursorCoordinate`](#getcursorcoordinate-function) and [`setCursorCoordinate`](#setcursorcoordinate-function) functions respectively.

Its shape and visibility can be set by using the [`setCursorShape`](#setcursorshape-function) and [`setCursorVisibility`](#setcursorvisibility-function) functions respectively. Available shapes are contained in the [`Shape`](#shape-enum-class) enum class.

The line the cursor is in can be cleared by using the [`clearCursorLine`](#clearcursorline-function) function.

The following example demonstrates how to use them to build a menu to showcase the cursor shapes:

```cpp
#include <array>

#include <tdk.hpp>

int main() {
  tdk::KeyEvent keyEvent;
  tdk::Coordinate coordinate;
  std::array<std::string, 7> names = {
      "Default",   "Blinking Block", "Block", "Blinking Underline",
      "Underline", "Blinking Bar",   "Bar"};
  std::array<std::string, 7> descriptions = {
      "The default shape intended for resets.",
      "The blinking variant of the block shape.",
      "The non-blinking variant of the block shape.",
      "The blinking variant of the underline shape.",
      "The non-blinking variant of the underline shape.",
      "The blinking variant of the bar shape.",
      "The non-blinking variant of the bar shape."};
  std::array<tdk::Shape, 7> shapes = {
      tdk::Shape::Default,   tdk::Shape::BlinkingBlock,
      tdk::Shape::Block,     tdk::Shape::BlinkingUnderline,
      tdk::Shape::Underline, tdk::Shape::BlinkingBar,
      tdk::Shape::Bar};
  tdk::setCursorVisibility(false);
  std::cout << "The following example will demonstrate the available terminal "
               "cursor shapes."
            << std::endl
            << tdk::XColor(tdk::XColorCode::Blue, tdk::Layer::Foreground)
            << "[?]"
            << tdk::XColor(tdk::XColorCode::Default, tdk::Layer::Foreground)
            << " Use the "
            << tdk::XColor(tdk::XColorCode::Yellow, tdk::Layer::Foreground)
            << tdk::Weight::Bold << "Arrows/Enter" << tdk::Weight::Default
            << tdk::XColor(tdk::XColorCode::Default, tdk::Layer::Foreground)
            << " keys to move and exit." << std::endl
            << std::endl
            << std::endl;
  tdk::getCursorCoordinate(coordinate);
  tdk::setCursorVisibility(true);
  tdk::setCursorCoordinate(0, coordinate.m_row - 1);
  int offset = 0;
  do {
    tdk::clearCursorLine();
    std::cout << "  (" << offset + 1 << "/" << names.size() << ") "
              << names[offset] << std::endl;
    tdk::clearCursorLine();
    std::cout << tdk::XColor(tdk::XColorCode::LightBlack,
                             tdk::Layer::Foreground)
              << "        " << descriptions[offset]
              << tdk::XColor(tdk::XColorCode::Default, tdk::Layer::Foreground);
    tdk::setCursorCoordinate(9 + names[offset].length(), coordinate.m_row - 1);
    tdk::setCursorShape(shapes[offset]);
    tdk::clearInputBuffer();
    tdk::readKeyEvent(keyEvent);
    if ((keyEvent.m_key == tdk::Key::LeftArrow ||
         keyEvent.m_key == tdk::Key::DownArrow) &&
        offset) {
      --offset;
    } else if ((keyEvent.m_key == tdk::Key::RightArrow ||
                keyEvent.m_key == tdk::Key::UpArrow) &&
               offset != names.size() - 1) {
      ++offset;
    }
  } while (keyEvent.m_key != tdk::Key::Enter);
  tdk::setCursorShape(tdk::Shape::Default);
  std::cout << std::endl << std::endl;
  return 0;
}
```

## ❡ Window

The terminal window dimensions can be get by using the [`getWindowDimensions`](#getwindowdimensions-function).

The alternate window is an alternate buffer that creates the feeling of that a terminal applications is running in a separate environment. It can be opened and closed by using the [`setAlternateScreen`](#setalternatewindow-function) function.

```cpp
#include <iomanip>

#include <tdk.hpp>

int main() {
  tdk::KeyEvent keyEvent;
  tdk::Dimensions windowDimensions;
  tdk::getWindowDimensions(windowDimensions);
  tdk::setAlternateWindow(true);
  tdk::setCursorVisibility(false);
  std::cout << "Total columns: " << windowDimensions.m_totalColumns << "."
            << std::endl
            << std::setw(13) << std::left << "Total rows"
            << ": " << std::right << windowDimensions.m_totalRows << "."
            << std::endl
            << std::endl
            << "Press any key to continue...";
  tdk::clearInputBuffer();
  tdk::readKeyEvent(keyEvent);
  tdk::setAlternateWindow(false);
  tdk::setCursorVisibility(true);
  return 0;
}
```

## ❡ Bell

You can ring the terminal bell by using the [`ringBell`](#ringbell-function) function, possibly emitting a symbol in terminal tab bar, visual flash, a system notification or a beep from the motherboard speaker. Terminals might have this feature disabled by default.

## ❡ Responsive Layouts

## ❡ Enum Classes

### Effect Enum Class

#### Brief

Contains the available terminal effects. Apply one by using the left shifting operator (`<<`) against an instance of the [`std::ostream`](https://cplusplus.com/reference/ostream/ostream) class. Remove it by using the right shifting operator (`>>`) instead.

#### Enumerators

- `Italic`: the italic effect makes the text curly. The terminal might require the use of a font with italic style in order to make it visible.
- `Underline`: the underline effect draws a horizontal line below the text.
- `Blinking`: the blinking effect makes the text blink indefinitely.
- `Negative`: the negative effect swaps the foreground and background colors.
- `Hidden`: the hidden effect makes the text hard to see or invisible.
- `Strikethrough`: the strike-through effect draws a horizontal line in the middle of the text.

### EventStatus Enum Class

#### Brief

Contains the status of a key event reading.

#### Enumerators

- `Success`: states the reading was sucessful.
- `WindowResizeInterrupt`: exclusively happening on Windows, states the key reading was interrupted by a window resize event.
- `Failure`: states the reading failed.

### Key Enum Class

#### Brief

Contains keys mostly not represented by UTF-8 graphemes. For exceptions, the values here may be used to improve readability. They are intended to be used to compose the information retrieved by the [`readKeyEvent`](#readkeyevent-function) function.

#### Enumerators

- `LeftArrow`, `UpArrow`, `RightArrow` and `DownArrow`: the arrow keys.
- `F1` to `F12`: the function keys.
- `PageUp`, `PageDown`, `Escape` `End` and `Home`: the navigation keys.
- `Insert`, `Delete`, `Tab`, `Enter`, `Spacebar` and `Backspace`: the insertion/deletion keys.

### Layer Enum Class

#### Brief

Contains the available terminal layers. It is used to create instances of the [`HexColor`](#hexcolor-class), [`RGBColor`](#rgbcolor-class) and [`XColor`](#xcolor-class) classes.

#### Enumerators

- `Foreground`: the foreground layer.
- `Background`: the background layer.

### Shape Enum Class

#### Brief

Contains the available terminal cursor shapes.

#### Parameters

- `Default`: the default shape is intended to be used for resets.
- `Block`, `BlinkingBlock`: the regular and blinking variants of the block shape.
- `Underline`, `BlinkingUnderline`: the regular and blinking variants of the underline shape.
- `Bar`, `BlinkingBar`: the regular and blinking variants of the bar shape.

### Stream Enum Class

#### Brief

Contains the standard terminal streams. You can check if they are connected to an interactive terminal by using the [`isTTY`](#istty-function) function.

#### Enumerators

- `Input`: the standard input stream (`stdin`).
- `Output`: the standard output stream (`stdout`).
- `Error`: the standard error stream (`stderr`).

### Weight Enum Class

#### Brief

Contains the available terminal text weights. Apply one by using the left shifting operator (`<<`) against an instance of the [`std::ostream`](https://cplusplus.com/reference/ostream/ostream) class.

#### Enumerators

- `Default`: the default weight is used for resets.
- `Bold`: the bold weight is usually rendered as bold and/or with bright colors. The terminal might require the use a font with bold weight in order to make it visible.
- `Dim`: the dim weight is usually rendered with faint colors.

### XColorCode Enum Class

#### Brief

Contains the ANSI codes of the first 16 colors of the XTerm palette plus one more color used for resets. It is used to create instances of the [`XColor`](#xcolor-class) class. These codes map to colors of the active terminal theme and can be used to ensure consistency.

#### Enumerators

- `Default`: the default color is used for resets.
- `Black`, `LightBlack`: the regular and light variants of the black color.
- `Red`, `LightRed`: the regular and light variants of the red color.
- `Green`, `LightGreen`: the regular and light variants of the green color.
- `Yellow`, `LightYellow`: the regular and light variants of the yellow color.
- `Blue`, `LightBlue`: the regular and light variants of the blue color.
- `Magenta`, `LightMagenta`: the regular and light variants of the magenta color.
- `Cyan`, `LightCyan`: the regular and light variants of the cyan color.
- `White`, `LightWhite`: the regular and light variants of the white color.

## ❡ Classes

### Coordinate Class

A class that represents a terminal coordinate.

#### Contructors

##### Brief

Creates an instance of the [`Coordinate`](#coordinate-class) class.

##### Declaration 1

```cpp
Coordinate();
```

##### Declaration 2

```cpp
Coordinate(unsigned short column, unsigned short row);
```

##### Parameters 2

- `column`: the column component of the coordinate.
- `row`: the row component of the coordinate.

#### Public Member Variables

- (`unsigned short`) `m_column`: the column component of the coordinate.
- (`unsigned short`) `m_row`: the row component of the coordinate.

### Dimensions Class

#### Brief

A class that represents terminal dimensions.

#### Constructors

##### Brief

Creates an instance of the [`Dimensions`](#dimensions-class) class.

##### Declaration 1

```cpp
Dimensions();
```

##### Declaration 2

```cpp
Dimensions(unsigned short totalColumns, unsigned short totalRows);
```

##### Parameters 2

- `totalColumns`: the total columns of the dimensions.
- `totalRows`: the total rows of the dimensions.

#### Public Member Variables

- (`unsigned short`) `m_totalColumns`: the total columns of the dimensions.
- (`unsigned short`) `m_totalRows`: the total rows of the dimensions.

### HexColor Class

#### Brief

A class that represents a hex color. Apply it by using the left shifting operator (`<<`) against an instance of the [`std::ostream`](https://cplusplus.com/reference/ostream/ostream) class. Remove it, by applying an instance of the [`XColor`](#xcolor-class) class constructed with the ANSI code [`XColorCode::Default`](#xcolorcode-enum-class) in the layer where the color has been applied.

#### Constructors

##### Brief

Creates an instance of the [`HexColor`](#hexcolor-class) class.

##### Declaration

```cpp
HexColor(unsigned int code, Layer layer);
```

##### Parameters

- `code`: the hex code of the color. It must be a value in range from `0x0` to `0xffffff`.
- `layer`: the layer where the color should be applied on.

#### Public Member Variables

- (`unsigned int`) `m_code`: the hex code of the color. It is a value in range from `0x0` to `0xffffff`.
- ([`Layer`](#layer-enum-class)) `m_layer`: the layer where the color should be applied on.

#### Public Member Functions

##### m_invert Member Function

###### Declaration

```cpp
HexColor m_invert();
```

###### Brief

Inverts the layer the color applies on.

###### Return Value

A color with the inverted layer.

### RGBColor Class

#### Brief

A class that represents an RGB color. Apply it by using the left shifting operator (`<<`) against an instance of the [`std::ostream`](https://cplusplus.com/reference/ostream/ostream) class. Remove it, by applying an instance of the [`XColor`](#xcolor-class) class constructed with the ANSI code [`XColorCode::Default`](#xcolorcode-enum-class) in the layer where the color has been applied.

#### Constructors

##### Brief

Creates an instance of the [`RGBColor`](#rgbcolor-class) class.

##### Declaration 1

```cpp
RGBColor(unsigned char red, unsigned char green, unsigned char blue,
         Layer layer);
```

##### Parameters 1

- `red`: the red component of the color.
- `green`: the green component of the color.
- `blue`: the blue component of the color.
- `layer`: the layer where the color should be applied.

##### Declaration 2

```cpp
RGBColor(HexColor color);
```

##### Parameters 2

- `color`: a hex color to be converted.

#### Public Member Variables

- (`unsigned char`) `m_red`: the red component of the color.
- (`unsigned char`) `m_green`: the green component of the color.
- (`unsigned char`) `m_blue`: the blue component of the color.
- ([`Layer`](#layer-enum-class)) `m_layer`: the layer where the color should be applied on.

#### Public Member Functions

##### m_invert Member Function

###### Declaration

```cpp
RGBColor m_invert();
```

###### Brief

Inverts the layer the color applies on.

###### Return Value

A color with the inverted layer.

### XColor Class

#### Brief

A class that represents a color from the XTerm palette: a terminal exclusive palette containing 256 colors. Apply it by using the left shifting operator (`<<`) against an instance of the [`std::ostream`](https://cplusplus.com/reference/ostream/ostream) class. Remove it by using the ANSI code [`XColorCode::Default`](#xcolorcode-enum-class) in the layer where the color has been applied.

#### Constructors

##### Brief

Creates an instance of the [`XColor`](#xcolor-class) class.

##### Declaration 1

```cpp
XColor(unsigned char code, Layer layer);
```

##### Parameters 1

- `code`: the ANSI code of the color.
- `layer`: the layer where the color should be applied on.

##### Declaration 2

```cpp
XColor(XColorCode code, Layer layer);
```

##### Parameters 2

- `code`: the ANSI code of the color.
- `layer`: the layer where the color should be applied on.

#### Public Member Variables

- (`int`) `m_code`: the ANSI code of the color.
- ([`Layer`](#layer-enum-class)) `m_layer`: the layer where the color should be applied on.

#### Public Member Functions

##### m_invert Member Function

###### Declaration

```cpp
XColor m_invert();
```

###### Brief

Inverts the layer the color applies on.

###### Return Value

A color with the inverted layer.

## ❡ Operator Overloadings

### operator<<(std::ostream &stream, HexColor color);

#### Brief

Applies a hex color.

#### Declaration

```cpp
std::ostream &operator<<(std::ostream &stream, Color color);
```

#### Parameters

- `stream`: the terminal stream to be affected.
- `color`: the color to be applied.

#### Return Value

The stream being affected.

### operator<<(std::ostream &stream, RGBColor color);

#### Brief

Applies an RGB color.

#### Declaration

```cpp
std::ostream &operator<<(std::ostream &stream, RGBColor color);
```

#### Parameters

- `stream`: the terminal stream to be affected.
- `color`: the color to be applied.

#### Return Value

The stream being affected.

### std::ostream &operator<<(std::ostream &stream, XColor color);

#### Brief

Applies a color from the XTerm palette.

#### Declaration

```cpp
std::ostream &operator<<(std::ostream &stream, XColor color);
```

#### Parameters

- `stream`: the terminal stream to be affected.
- `color`: the color to be applied.

#### Return Value

The stream being affected.

## ❡ Functions

### clearCursorLine Function

#### Brief

Clears the contents in the terminal cursor line.

#### Declaration

```cpp
void clearCursorLine();
```

### clearInputBuffer Function

#### Brief

Clears the standard input buffer.

#### Declaration

```cpp
void clearInputBuffer();
```

### getCursorCoordinate Function

### getWindowDimensions Function

#### Brief

Gets the terminal window dimensions if successful.

#### Declaration

```cpp
int getWindowDimensions(Dimensions &dimensions);
```

#### Parameters

- `dimensions`: the address where the dimensions information will be put into.

#### Return Value

0 if successful and -1 otherwise.

### isTTY Function

#### Brief

Checks if a standard stream is connected to an interactive terminal (TTY).

#### Declaration

```cpp
bool isTTY(Stream stream);
```

#### Parameters

- `stream`: the stream to be checked.

#### Return Value

A boolean that states the check result.

### readKeyEvent Function

#### Brief

Reads a key event from the standard input buffer. Exclusively on Windows, it can be interrupted by a window resize event that might happen in the same event loop. It holds the thread until a valid key event is pulled or it is interrupted.

The key read may be an UTF-8 grapheme or a key value from the [`Key`](#key-enum-class) enum class static casted to an `int` type. Values from the [`Key`](#key-enum-class) enum can be compared directly against the key by using comparasion operators such as `==`, `>`, `>=`, `<` and `<=`.

As a limitation of this function: modifier keys are only allowed to be detected in keys of the English alphabet (lowercase (`a-z`) and uppercase (`A-Z`) letters), as those are the ranges in which terminals offer the greatest support. The Shift modifier key is intended to be detected by checking the key received: For example, lowercase letters become uppercase when it is used.

There may be some key sequences in which it may be unable to distinguish the keys used. For example: the sequences `Ctrl + i` and `Ctrl + j` are the same as keys `Key::Backspace` and `Key::Enter` respectively because they have the same underlying code sent by the terminal. For the same reason, keys with `Ctrl` can not have the `Shift` key detected. Some other key sequences may be reserved by the terminal.

#### Declaration

```cpp
EventStatus readKeyEvent(KeyEvent &event);
```

#### Parameters

- `event`: the address where the event information will be put into.

#### Return Value

The status of the event reading.

#### Parameters

### ringBell Function

#### Brief

Rings the terminal bell, possibly emitting a symbol in terminal tab bar, visual flash, a system notification or a beep from the motherboard speaker. Terminals might have this feature disabled by default.

#### Declaration

```cpp
void ringBell();
```

### setAlternateWindow Function

#### Brief

Opens/closes the alternate window: an alternate buffer that creates the feeling of that a terminal applications is running in a separate environment.

#### Declaration

```cpp
void setAlternateWindow(bool isToOpen);
```

#### Parameters

- `isToOpen`: a boolean that states the alternate window should be opened or closed

### setCursorCoordinate Function

#### Brief

Sets the terminal cursor coordinate. The coordinate will always be fit within the window boundaries, use the [`getWindowDimensions`](#getwindowdimensions-function) function to treat possible exceptions.

#### Declaration 1

```cpp
void setCursorCoordinate(unsigned short column, unsigned short row);
```

#### Parameters 1

- `column`: the column component of the coordinate.
- `row`: the row component of the coordinate.

#### Declaration 2

```cpp
void setCursorCoordinate(Coordinate &coordinate);
```

#### Parameters 2

- `coordinate`: the coordinate to be set.

### setCursorShape Function

#### Brief

Sets the terminal cursor shape.

#### Declaration

```cpp
void setCursorShape(Shape shape);
```

#### Parameters

- `shape`: the shape to be set.

### setCursorVisibility Function

#### Brief

Sets the terminal cursor visibility.

#### Declaration

```cpp
void setCursorVisibility(bool isToShow);
```

#### Parameters

- `isToShow`: a boolean that states the cursor should be visible or not.

&ensp;
<p align="center"><sup><strong>≥v≥v&ensp;Here Be Dragons!&ensp;≥v≥</strong><br />Made with love by skippyr <3</sup></p>
