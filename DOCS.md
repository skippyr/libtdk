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

This document will give you an overview about certain topics related to terminal manipulation, showing you how to use The library to handle them, and it will also include the documentation of each of its components.

The recommended way of reading it is directly on GitHub. That way as you will be able to use its **Outline** feature available at the top right corner of the website to access each section more easily.

## ❡ Usage Limitation

The library has some limitations over its usage. To avoid conflicts and malfunctions, you must follow these rules:

- It uses byte oriented functions and operator overloadings from the standard library for UTF-8 encoding support. Thus, all standard streams must be kept unoriented or under that same orientation. Do not use wide character functions.

- It does not provide full resets of terminal properties. Do not manipulate them manually.

## ❡ Namespace

The library reserves names under the `tdk` namespace. To avoid naming conflicts, it is highly recommend to not short them by using the `using namespace` keywords.

## ❡ Colors

The library allows you to set terminals colors into layers by using different color formats. Available layers are contained in the [`Layer`](#layer-enum-class) enum class. Each color format accepted is a class:

- [`HexColor`](#hexcolor-class): refers to a hexadecimal color.
- [`RGBColor`](#rgbcolor-class): refers to an RGB color.
- [`XColor`](#xcolor-class): refers to a color from the XTerm palette: a terminal exclusive palette containing 256 colors. The ANSI code of the first 16 colors of this palette plus one more color used for resets are contained in the [`XColorCode`](#xcolorcode-enum-class) enum class. These codes map to colors of the active terminal theme and can be used to ensure consistency.

A color can be applied by using the left shifting operator (`<<`) with an instance of a color class against an instance of the [`std::ostream`](https://cplusplus.com/reference/ostream/ostream) class. To be removed, you must apply an instance of the [`XColor`](#xcolor-class) class constructed with the ANSI code [`XColorCode::Default`](#xcolorcode-enum-class) in the layer where the color has been applied.

Colors are not applied if the standard output and error streams are being redirected or piped. Using a hex or RGB colors require the terminal to have truecolor support.

The following example outputs the demo of three colors using different formats. Some colors are part of the [Flamerial](https://github.com/skippyr/flamerial) theme:

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
  tdk::XColor red = tdk::XColor(tdk::XColorCode::Red, tdk::Layer::Foreground);
  tdk::HexColor flamerialYellow =
      tdk::HexColor(0xe0a100, tdk::Layer::Foreground);
  tdk::RGBColor flamerialGreen =
      tdk::RGBColor(tdk::HexColor(0xa16205, tdk::Layer::Foreground));
  std::cout << std::left << std::setw(g_message.length()) << "Foreground"
            << std::right << " Background" << std::endl
            << std::string(g_message.length(), '-') << " "
            << std::string(g_message.length(), '-') << std::endl;
  writeColorDemo(red);
  writeColorDemo(flamerialYellow);
  writeColorDemo(flamerialGreen);
  return 0;
}

```

## ❡ Weights

The library allows you to set terminal text weights, also refered as "foreground brightness". Availabe weight are contained inside of the [`Weight`](#weight-enum-class) enum class. Apply one by using the left shifting operator (`<<`) against an instance of the [`std::ostream`](https://cplusplus.com/reference/ostream/ostream) class.

Weights are not applied if the standard output and error streams are being redirected or piped.

The following example is a demo for the weights:

```cpp
#include <iomanip>

#include <tdk.hpp>

int main() {
  std::string message = "Here Be Dragons!";
  int padding = 9;
  std::cout >> tdk::Effect::Strikethrough;
  std::cout << std::left << std::setw(padding) << "Bold" << tdk::Weight::Bold
            << message << tdk::Weight::Default << std::endl
            << std::setw(padding) << "Dim" << tdk::Weight::Dim << message
            << tdk::Weight::Default << std::endl
            << std::setw(padding) << "Default" << std::right << message
            << std::endl;
  return 0;
}
```

## ❡ Effects

The library allows you to set terminal text effect. Available effects are contained inside of the [`Effect`](#effect-enum-class) enum class. Apply one by using the left shifting operator (`<<`) against an instance of the [`std::ostream`](https://cplusplus.com/reference/ostream/ostream) class. Remove it by using the right shifting operator (`>>`) instead.

Effects are not applied if the standard output and error streams are being redirected or piped.

The following example is a demo for effects:

```cpp
#include <iomanip>

#include <tdk.hpp>

int main() {
  std::string message = "Here Be Dragons!";
  int padding = 15;
  std::cout << std::left << std::setw(padding) << "Italic"
            << tdk::Effect::Italic << message >>
      tdk::Effect::Italic << std::endl
                          << std::setw(padding) << "Underline"
                          << tdk::Effect::Underline << message >>
      tdk::Effect::Underline << std::endl
                             << std::setw(padding) << "Blinking"
                             << tdk::Effect::Blinking << message >>
      tdk::Effect::Blinking << std::endl
                            << std::setw(padding) << "Negative"
                            << tdk::Effect::Negative << message >>
      tdk::Effect::Negative << std::endl
                            << std::setw(padding) << "Hidden"
                            << tdk::Effect::Hidden << message >>
      tdk::Effect::Hidden << std::endl
                          << std::setw(padding) << "Strikethrough" << std::right
                          << tdk::Effect::Strikethrough << message >>
      tdk::Effect::Strikethrough << std::endl;
  return 0;
}
```

## ❡ Standard Streams

The terminal streams are contained in the [`Stream`](#streams-enum-class) enum class. You can check if they are connected to an interactive terminal by using the [`isTTY`](#istty-function) function.

The following example shows how to use them to output the TTY statuses of each stream:

```cpp
#include <tdk.hpp>

#define BOOLEAN(a_value)                                                       \
  tdk::XColor(a_value ? tdk::XColorCode::Green : tdk::XColorCode::Red,         \
              tdk::Layer::Foreground)                                          \
      << (a_value ? "True" : "False")                                          \
      << tdk::XColor(tdk::XColorCode::Default, tdk::Layer::Foreground)

int main() {
  std::cout << "TTY Statuses" << std::endl
            << "------------" << std::endl
            << "Input : " << BOOLEAN(tdk::isTTY(tdk::Stream::Input))
            << std::endl
            << "Output: " << BOOLEAN(tdk::isTTY(tdk::Stream::Output))
            << std::endl
            << "Error : " << BOOLEAN(tdk::isTTY(tdk::Stream::Error))
            << std::endl;
  return 0;
}
```

## ❡ Window

## ❡ Cursor

## ❡ Key Events

## ❡ Responsive Layouts

## ❡ Enum Classes

### Effect Enum Class

#### Brief

An enum class containing the available terminal effects. Apply one by using the left shifting operator (`<<`) against an instance of the [`std::ostream`](https://cplusplus.com/reference/ostream/ostream) class. Remove it by using the right shifting operator (`>>`) instead.

Effects are not applied if the standard output and error streams are being redirected or piped.

#### Enumerators

- `Italic`: the italic effect makes the text curly. The terminal might require the use of a font with italic style in order to make it visible.
- `Underline`: the underline effect draws a horizontal line below the text.
- `Blinking`: the blinking effect makes the text blink indefinitely.
- `Negative`: the negative effect swaps the foreground and background colors.
- `Hidden`: the hidden effect makes the text hard to see or invisible.
- `Strikethrough`: the strike-through effect draws a horizontal line in the middle of the text.

### Layer Enum Class

#### Brief

An enum class containing the available terminal layers. It is used to create instances of the [`HexColor`](#hexcolor-class), [`RGBColor`](#rgbcolor-class) and [`XColor`](#xcolor-class) classes.

#### Enumerators

- `Foreground`: the foreground layer.
- `Background`: the background layer.

### Stream Enum Class

#### Brief

An enum class containing the standard terminal streams. You can check if they are connected to an interactive terminal by using the [`isTTY`](#istty-function) function.

#### Enumerators

- `Input`: the standard input stream (`stdin`).
- `Output`: the standard output stream (`stdout`).
- `Error`: the standard error stream (`stderr`).

#### Enumerators

### XColorCode Enum Class

#### Brief

An enum class containing the ANSI codes of the first 16 colors of the XTerm palette plus one more color used for resets. It is used to create instances of the [`XColor`](#xcolor-class) class. These codes map to colors of the active terminal theme and can be used to ensure consistency.

#### Enumerators

- `Default`: the default color is used for resets.
- `Black`: the regular variant of the black color.
- `Red`: the regular variant of the red color.
- `Green`: the regular variant of the green color.
- `Yellow`: the regular variant of the yellow color.
- `Blue`: the regular variant of the blue color.
- `Magenta`: the regular variant of the magenta color.
- `Cyan`: the regular variant of the cyan color.
- `White`: the regular variant of the white color.
- `LightBlack`: the light variant of the black color.
- `LightRed`: the light variant of the red color.
- `LightGreen`: the light variant of the green color.
- `LightYellow`: the light variant of the yellow color.
- `LightBlue`: the light variant of the blue color.
- `LightMagenta`: the light variant of the magenta color.
- `LightCyan`: the light variant of the cyan color.
- `LightWhite`: the light variant of the white color.

### Weight Enum Class

#### Brief

An enum containing the available terminal text weights. Apply one by using the left shifting operator (`<<`) against an instance of the [`std::ostream`](https://cplusplus.com/reference/ostream/ostream) class.

Weights are not applied if the standard output and error streams are being redirected or piped.

#### Enumerators

- `Default`: the default weight is used for resets.
- `Bold`: the bold weight is usually rendered as bold and/or with bright colors. The terminal might require the use a font with bold weight in order to make it visible.
- `Dim`: the dim weight is usually rendered with faint colors.

## ❡ Functions

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

## ❡ Classes

### HexColor Class

#### Brief

A class that represents a hex color. Apply it by using the left shifting operator (`<<`) against an instance of the [`std::ostream`](https://cplusplus.com/reference/ostream/ostream) class. Remove it, by applying an instance of the [`XColor`](#xcolor-class) class constructed with the ANSI code [`XColorCode::Default`](#xcolorcode-enum-class) in the layer where the color has been applied.

It requires the terminal to have truecolor support. The color is not applied if the standard output and error streams are being redirected or piped.

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

It requires the terminal to have truecolor support. The color is not applied if the standard output and error streams are being redirected or piped.

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

The color is not applied if the standard output and error streams are being redirected or piped.

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

#### Remarks

It requires the terminal to have truecolor support. The color is not applied if the standard output and error streams are being redirected or piped.

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

#### Remarks

It requires the terminal to have truecolor support. The color is not applied if the standard output and error streams are being redirected or piped.

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

#### Remarks

The color is not applied if the stream is being redirected or piped.

&ensp;
<p align="center"><sup><strong>≥v≥v&ensp;Here Be Dragons!&ensp;≥v≥</strong><br />Made with love by skippyr <3</sup></p>
