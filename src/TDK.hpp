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
enum class CursorShape {
  Default,
  BlinkingBlock,
  Block,
  BlinkingUnderline,
  Underline,
  BlinkingBar,
  Bar
};

enum class Effect {
  Italic = 3,
  Underline,
  Blinking,
  ReverseVideo = 7,
  Conceal,
  Strikethrough
};

enum class Key {
#ifdef _WIN32
  LeftArrow = -23,
  UpArrow,
  RightArrow,
  DownArrow,
#else
  UpArrow = -23,
  DownArrow,
  RightArrow,
  LeftArrow,
#endif
  F1,
  F2,
  F3,
  F4,
  F5,
  F6,
  F7,
  F8,
  F9,
  F10,
  F11,
  F12,
#ifdef _WIN32
  PageUp,
  PageDown,
  End,
  Home,
  Insert,
  Delete,
#else
  Home,
  Insert,
  Delete,
  End,
  PageUp,
  PageDown,
#endif
  Tab = 9,
#if defined(_WIN32) || defined(__APPLE__)
  Enter = 13,
#else
  Enter,
#endif
  Escape = 27,
  Spacebar = 32,
  Backspace = 127
};

enum class KeyEventStatus {
  Success,
  WindowResizeInterrupt = -2,
  Failure
};

enum class Layer {
  Foreground = 3,
  Background
};

enum class Stream {
  Input,
  Output,
  Error
};

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

enum class Weight {
  Default,
  Bold,
  Dim
};

class Coordinate {
public:
  Coordinate();
  Coordinate(unsigned short column, unsigned short row);

  unsigned short m_column;
  unsigned short m_row;
};

class Dimensions {
public:
  Dimensions();
  Dimensions(unsigned short totalColumns, unsigned short totalRows);

  unsigned short m_totalColumns;
  unsigned short m_totalRows;
};

class HexColor {
public:
  HexColor(unsigned int code, Layer layer);
  HexColor m_invert();

  unsigned int m_code;
  Layer m_layer;
};

class KeyEvent {
public:
  KeyEvent();

  int m_key;
  bool m_hasAlt;
  bool m_hasCtrl;
};

class RGBColor {
public:
  RGBColor(unsigned char red, unsigned char green, unsigned char blue,
           Layer layer);
  RGBColor(HexColor color);
  RGBColor m_invert();

  unsigned char m_red;
  unsigned char m_green;
  unsigned char m_blue;
  Layer m_layer;
};

class XColor {
public:
  XColor(unsigned char code, Layer layer);
  XColor(XColorCode code, Layer layer);
  XColor m_invert();

  int m_code;
  Layer m_layer;
};

std::ostream &operator<<(std::ostream &stream, Effect effect);
std::ostream &operator>>(std::ostream &stream, Effect effect);
std::ostream &operator<<(std::ostream &stream, HexColor color);
std::ostream &operator<<(std::ostream &stream, RGBColor color);
std::ostream &operator<<(std::ostream &stream, XColor color);
std::ostream &operator<<(std::ostream &stream, Weight weight);
bool operator==(int code, Key key);
bool operator!=(int code, Key key);

void clearCursorLine();
void clearInputBuffer();
int getCursorCoordinate(Coordinate &coordinate);
int getWindowDimensions(Dimensions &dimensions);
bool isTTY(Stream stream);
KeyEventStatus readKeyEvent(KeyEvent &event);
void ringBell();
void setAlternateWindow(bool isToOpen);
void setCursorCoordinate(unsigned short column, unsigned short row);
void setCursorCoordinate(Coordinate &coordinate);
void setCursorShape(CursorShape shape);
void setCursorVisibility(bool isToShow);
}
