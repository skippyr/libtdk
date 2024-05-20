<p align="center">
  <img alt="" src="assets/ornament.webp" />
</p>
<h1 align="center">≥v≥v&ensp;libtdk&ensp;≥v≥v</h1>
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

This documentation will abroad:

- An explanation over its usage, limitations and namespace.
- An overview of certain topics related to terminal manipulation and show how to use the library components to handle them. Some examples will be given so you can test them in your development environment.
- A more specific explain over each component of the library, providing examples and remarks over their usages.

To access each section more easily, remember to use the GitHub table of contents feature available at the top right corner of the website.

## ❡ Usage Limitation

This library has some limitations over its usage. To avoid conflicts and malfunctions, you must follow these rules:

- It uses byte oriented functions and operator overloadings from the standard library for UTF-8 encoding support. Thus, all standard streams must be kept unoriented or under that same orientation. Do not use wide character functions.

- It does not provide full resets of terminal properties. Do not manipulate them manually.

## ❡ Namespace

This library reserves names under the `tdk` namespace. It is highly recommend to not short them by using the `using namespace` keywords.

## ❡ Included Headers

When included, the library will always put the following headers in the scope of your program:

```cpp
/* Standard Headers */
#include <cstdarg>
#include <iostream>

#ifdef _WIN32
/* Windows Headers */
#include <Windows.h>
#include <io.h>
#else
/* Linux Headers */
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif
```

## ❡ Colors

This library allows you to set terminals colors in terminal layers using different color formats. Available terminal layers are contained in the `Layer` enum class. Each color format accepted is a class:

- `XColor`: refers to a color from the XTerm palette. The ANSI code of the first 16 colors of this palette plus one more color intended for resets are contained in the `XColorCode` enum class.
- `HexColor`: refers to a hexadecimal color.
- `RGBColor`: refers to an RGB color.

A color can be applied by using the left shifting operator (`<<`) with an instance of a color class against an instance of the `std::ostream` class. To be removed, you must apply an instance of the `XColor` class constructed with the ANSI code `XColorCode::Default` in the layer where the color has been applied.

The following example shows an usage example of both background and foreground colors. In it, the terminal will output the demo of three colors, each in using a different supported color format:

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
            << std::right << " "
            << "Background" << std::endl
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
## ❡ Functions
## ❡ Classes
## ❡ Operator Overloadings
