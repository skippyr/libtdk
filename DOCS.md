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

- [`XColor`](#xcolor-class): refers to a color from the XTerm palette. The ANSI code of the first 16 colors of this palette plus one more color intended for resets are contained in the [`XColorCode`](#xcolorcode-enum-class) enum class.
- [`HexColor`](#hexcolor-class): refers to a hexadecimal color.
- [`RGBColor`](#rgbcolor-class): refers to an RGB color.

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

### XColorCode Enum Class

## ❡ Functions

## ❡ Classes

### XColor Class

### HexColor Class

### RGBColor Class

## ❡ Operator Overloadings
