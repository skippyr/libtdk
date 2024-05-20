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

This document will give you an overview about certain topics related to terminal manipulation, showing you how to use this library to handle them, and it will also include the documentation of each of its components.

The recommended way of reading it is directly on GitHub. That way as you will be able to use its **Outline** feature available at the top right corner of the website to access each section more easily.

## ❡ Usage Limitation

This library has some limitations over its usage. To avoid conflicts and malfunctions, you must follow these rules:

- It uses byte oriented functions and operator overloadings from the standard library for UTF-8 encoding support. Thus, all standard streams must be kept unoriented or under that same orientation. Do not use wide character functions.

- It does not provide full resets of terminal properties. Do not manipulate them manually.

## ❡ Namespace

This library reserves names under the `tdk` namespace. To avoid naming conflicts, it is highly recommend to not short them by using the `using namespace` keywords.

## ❡ Colors

This library allows you to set terminals colors into layers by using different color formats. Available layers are contained in the [`Layer`](#layer-enum-class) enum class. Each color format accepted is a class:

- [`HexColor`](#hexcolor-class): refers to a hexadecimal color.
- [`RGBColor`](#rgbcolor-class): refers to an RGB color.
- [`XColor`](#xcolor-class): refers to a color from the XTerm palette: a terminal exclusive palette containing 256 colors. The ANSI code of the first 16 colors of this palette plus one more color intended for resets are contained in the [`XColorCode`](#xcolorcode-enum-class) enum class. These codes map to colors of the active terminal theme and can be used to ensure consistency.

A color can be applied by using the left shifting operator (`<<`) with an instance of a color class against an instance of the [`std::ostream`](https://cplusplus.com/reference/ostream/ostream) class. To be removed, you must apply an instance of the [`XColor`](#xcolor-class) class constructed with the ANSI code [`XColorCode::Default`](#xcolorcode-enum-class) in the layer where the color has been applied.

Colors are not applied if the standard output and error streams are being redirected or piped. Using a hex or RGB colors require the terminal to have truecolor support.

The following example shows an example that outputs the demo of three colors using different formats. Some colors are part of the [Flamerial](https://github.com/skippyr/flamerial) theme.

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

Check the specific documentation for the enum classes and classes used in this section for more details.

## ❡ Weights

## ❡ Effects

## ❡ Standard Streams

## ❡ Window

## ❡ Cursor

## ❡ Key Events

## ❡ Responsive Layouts

## ❡ Enum Classes

### Layer Enum Class

#### Brief

An enum class containing the available terminal layers. It is used to create instances of the [`HexColor`](#hexcolor-class), [`RGBColor`](#rgbcolor-class) and [`XColor`](#xcolor-class) classes.

#### Enumerators

- `Foreground`: the foreground layer.
- `Background`: the background layer.

### XColorCode Enum Class

#### Brief

An enum class containing the ANSI codes of the first 16 colors of the XTerm palette plus one more color intended for resets. It is used to create instances of the [`XColor`](#xcolor-class) class. These codes map to colors of the active terminal theme and can be used to ensure consistency.

#### Enumerators

- `Default`: the default color, intended for resets.
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

## ❡ Functions

## ❡ Classes

### XColor Class

#### Brief

A class that represents a color from the XTerm palette: a terminal exclusive palette containing 256 colors. Apply it by using the left shifting operator (`<<`) against an instance of the [`std::ostream`](https://cplusplus.com/reference/ostream/ostream) class. Remove it by using the ANSI code [`XColorCode::Default`](#xcolorcode-enum-class) in the layer where the color has been applied.

The color is not applied if the standard output and error streams are being redirected or piped.

#### Constructors

Creates an instance of the [`XColor`](#xcolor-class) class.

```cpp
XColor(unsigned char code, Layer layer);
```

- `code`: the ANSI code of the color.
- `layer`: the layer where the color should be applied on.

```cpp
XColor(XColorCode code, Layer layer);
```

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

Returns a color with the inverted layer.

### HexColor Class

#### Brief

A class that represents a hex color. Apply it by using the left shifting operator (`<<`) against an instance of the [`std::ostream`](https://cplusplus.com/reference/ostream/ostream) class. Remove it, by applying an instance of the [`XColor`](#xcolor-class) class constructed with the ANSI code [`XColorCode::Default`](#xcolorcode-enum-class) in the layer where the color has been applied.

It requires the terminal to have truecolor support. The color is not applied if the standard output and error streams are being redirected or piped.

#### Constructors

Creates an instance of the [`HexColor`](#hexcolor-class) class.

```cpp
HexColor(unsigned int code, Layer layer);
```

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

Returns a color with the inverted layer.

### RGBColor Class

#### Brief

A class that represents an RGB color. Apply it by using the left shifting operator (`<<`) against an instance of the [`std::ostream`](https://cplusplus.com/reference/ostream/ostream) class. Remove it, by applying an instance of the [`XColor`](#xcolor-class) class constructed with the ANSI code [`XColorCode::Default`](#xcolorcode-enum-class) in the layer where the color has been applied.

It requires the terminal to have truecolor support. The color is not applied if the standard output and error streams are being redirected or piped.

#### Constructors

Creates an instance of the [`RGBColor`](#rgbcolor-class) class.

```cpp
RGBColor(unsigned char red, unsigned char green, unsigned char blue,
         Layer layer);
```

- `red`: the red component of the color.
- `green`: the green component of the color.
- `blue`: the blue component of the color.
- `layer`: the layer where the color should be applied.

```cpp
RGBColor(HexColor color);
```

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

Returns a color with the inverted layer.

## ❡ Operator Overloadings

### std::ostream &operator<<(std::ostream &stream, HexColor color);

#### Brief

Applies a hex color.

#### Declaration

```cpp
std::ostream &operator<<(std::ostream &stream, Color color);
```

#### Parameters

- `stream`: the terminal stream to be affected.
- `color`: the color to be applied.

#### Remarks

It requires the terminal to have truecolor support. The color is not applied if the standard output and error streams are being redirected or piped.

### std::ostream &operator<<(std::ostream &stream, RGBColor color);

#### Brief

Applies an RGB color.

#### Declaration

```cpp
std::ostream &operator<<(std::ostream &stream, RGBColor color);
```

#### Parameters

- `stream`: the terminal stream to be affected.
- `color`: the color to be applied.

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

#### Remarks

The color is not applied if the stream is being redirected or piped.

&ensp;
<p align="center"><sup><strong>≥v≥v&ensp;Here Be Dragons!&ensp;≥v≥</strong><br />Made with love by skippyr <3</sup></p>
