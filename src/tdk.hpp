#pragma once

#include <functional>
#include <iostream>

namespace tdk {
enum class CursorShape {
  Default,
  BlinkingBlock,
  Block,
  BlinkingUnderline,
  Underline,
  BlinkingBar,
  Bar
};

enum class EffectCode {
  Italic = 3,
  Underline,
  Blinking,
  Negative = 7,
  Hidden,
  Strikethrough
};

enum class Layer { Foreground = 3, Background };

enum class Stream { Input, Output, Error };

enum class XColorCode {
  Default = -1,
  Black,
  Red,
  Green,
  Yellow,
  Blue,
  Magenta,
  Cyan,
  White,
  LightBlack,
  LightRed,
  LightGreen,
  LightYellow,
  LightBlue,
  LightMagenta,
  LightCyan,
  LightWhite
};

enum class Weight { Default, Bold, Dim };

class Coordinate final {
public:
  Coordinate();
  Coordinate(unsigned short column, unsigned short row);

  unsigned short column_m;
  unsigned short row_m;
};

class Effect final {
public:
  int code_m;
  bool isToEnable_m;

  Effect(EffectCode code, bool isToEnable);
  Effect(int code, bool isToEnable);
};

class HexColor final {
public:
  HexColor(unsigned int code, Layer layer);
  HexColor invert_m();

  unsigned int code_m;
  Layer layer_m;
};

class Region final {
public:
  unsigned short totalColumns_m;
  unsigned short totalRows_m;
  unsigned int area_m;
  Coordinate topLeftCoordinate_m;
  Coordinate topRightCoordinate_m;
  Coordinate bottomLeftCoordinate_m;
  Coordinate bottomRightCoordinate_m;

  Region();
  Region(unsigned short totalColumns, unsigned short totalRows);
  Region(Coordinate cornerCoordinate0, Coordinate cornerCoordinate1);
};

class RGBColor final {
public:
  RGBColor(unsigned char red, unsigned char green, unsigned char blue,
           Layer layer);
  RGBColor(HexColor color);
  RGBColor invert_m();

  unsigned char red_m;
  unsigned char green_m;
  unsigned char blue_m;
  Layer layer_m;
};

class XColor final {
public:
  XColor(unsigned char code, Layer layer);
  XColor(XColorCode code, Layer layer);
  XColor invert_m();

  int code_m;
  Layer layer_m;
};

std::ostream &operator<<(std::ostream &stream, Effect effect);
std::ostream &operator<<(std::ostream &stream, HexColor color);
std::ostream &operator<<(std::ostream &stream, RGBColor color);
std::ostream &operator<<(std::ostream &stream, XColor color);
std::ostream &operator<<(std::ostream &stream, Weight weight);
int operator|(EffectCode code0, EffectCode code1);

void clearCursorLine();
void clearInputBuffer();
void clearWindow();
void closeAlternateWindow();
int getCursorCoordinate(Coordinate &coordinate);
int getWindowRegion(Region &region);
bool isTTY(Stream stream);
void openAlternateWindow();
void ringBell();
void setCursorCoordinate(unsigned short column, unsigned short row);
void setCursorCoordinate(Coordinate &coordinate);
void setCursorShape(CursorShape shape);
void setCursorVisibility(bool isToShow);
void setWindowTitle(std::string title);
} // namespace tdk
