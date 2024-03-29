#ifndef _tdk_H
#define _tdk_H
enum tdk_Color
{
    tdk_Color_Default = -1,
    tdk_Color_Black,
    tdk_Color_Red,
    tdk_Color_Green,
    tdk_Color_Yellow,
    tdk_Color_Blue,
    tdk_Color_Magenta,
    tdk_Color_Cyan,
    tdk_Color_White,
    tdk_Color_LightBlack,
    tdk_Color_LightRed,
    tdk_Color_LightGreen,
    tdk_Color_LightYellow,
    tdk_Color_LightBlue,
    tdk_Color_LightMagenta,
    tdk_Color_LightCyan,
    tdk_Color_LightWhite
};

enum tdk_CursorShape
{
    tdk_CursorShape_Default,
    tdk_CursorShape_BlinkingBlock,
    tdk_CursorShape_Block,
    tdk_CursorShape_BlinkingUnderline,
    tdk_CursorShape_Underline,
    tdk_CursorShape_BlinkingBar,
    tdk_CursorShape_Bar
};

enum tdk_Effect
{
    tdk_Effect_Italic = 1 << 3,
    tdk_Effect_Underline = 1 << 4,
    tdk_Effect_Blinking = 1 << 5,
    tdk_Effect_ReverseVideo = 1 << 7,
    tdk_Effect_Conceal = 1 << 8,
    tdk_Effect_Strikethrough = 1 << 9
};

enum tdk_Key
{
#ifdef _WIN32
    tdk_Key_LeftArrow = -23,
    tdk_Key_UpArrow,
    tdk_Key_RightArrow,
    tdk_Key_DownArrow,
#else
    tdk_Key_UpArrow = -23,
    tdk_Key_DownArrow,
    tdk_Key_RightArrow,
    tdk_Key_LeftArrow,
#endif
    tdk_Key_F1,
    tdk_Key_F2,
    tdk_Key_F3,
    tdk_Key_F4,
    tdk_Key_F5,
    tdk_Key_F6,
    tdk_Key_F7,
    tdk_Key_F8,
    tdk_Key_F9,
    tdk_Key_F10,
    tdk_Key_F11,
    tdk_Key_F12,
#ifdef _WIN32
    tdk_Key_PageUp,
    tdk_Key_PageDown,
    tdk_Key_End,
    tdk_Key_Home,
    tdk_Key_Insert,
    tdk_Key_Delete,
#else
    tdk_Key_Home,
    tdk_Key_Insert,
    tdk_Key_Delete,
    tdk_Key_End,
    tdk_Key_PageUp,
    tdk_Key_PageDown,
#endif
    tdk_Key_Tab = 9,
#if defined(_WIN32) || defined(__APPLE__)
    tdk_Key_Enter = 13,
#else
    tdk_Key_Enter,
#endif
    tdk_Key_Escape = 27,
    tdk_Key_Space = 32,
    tdk_Key_Backspace = 127
};

enum tdk_Layer
{
    tdk_Layer_Foreground = 3,
    tdk_Layer_Background
};

enum tdk_ModifierKey
{
    tdk_ModifierKey_Alt = 1,
    tdk_ModifierKey_Ctrl = 1 << 1
};

enum tdk_Stream
{
    tdk_Stream_Input,
    tdk_Stream_Output,
    tdk_Stream_Error
};

enum tdk_Weight
{
    tdk_Weight_Default,
    tdk_Weight_Bold,
    tdk_Weight_Dim
};

struct tdk_Coordinate
{
    unsigned short column;
    unsigned short row;
};

struct tdk_Dimensions
{
    unsigned short totalOfColumns;
    unsigned short totalOfRows;
};

struct tdk_KeyEvent
{
    int key;
    char modifiers;
};

struct tdk_RGB
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

#ifdef __cplusplus
extern "C"
{
#endif
    void tdk_clearCursorLine(void);
    void tdk_clearInputBuffer(void);
    int tdk_isTTY(int stream);
    int tdk_getCursorCoordinate(struct tdk_Coordinate *coordinate);
    int tdk_getWindowDimensions(struct tdk_Dimensions *dimensions);
    int tdk_readKeyEvent(struct tdk_KeyEvent *event);
    void tdk_ringBell(void);
    void tdk_set256Color(int color, int layer);
    void tdk_setAlternateWindow(int isToEnable);
    int tdk_setCursorCoordinate(struct tdk_Coordinate coordinate);
    void tdk_setCursorShape(int shape);
    void tdk_setCursorVisibility(int isToShow);
    void tdk_setEffect(int effect, int isToEnable);
    void tdk_setHEXColor(int color, int layer);
    void tdk_setRGBColor(struct tdk_RGB color, int layer);
    void tdk_setWeight(int weight);
    int tdk_write(const char *format, ...);
    int tdk_writeError(const char *format, ...);
    int tdk_writeErrorLine(const char *format, ...);
    int tdk_writeLine(const char *format, ...);
#ifdef __cplusplus
}
#endif
#endif
