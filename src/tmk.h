#ifndef _TMK_H
#define _TMK_H

#include <stdbool.h>

#pragma region Enums
/**
 * @brief Contains the terminal streams.
 */
typedef enum tmk_Stream
{
    /**
     * @brief Where events and data is input.
     */
    tmk_Stream_Input,
    /**
     * @brief Where regular tense messages are output.
     */
    tmk_Stream_Output,
    /**
     * @brief Where error tense messages are output.
     */
    tmk_Stream_Error
} tmk_Stream_t;

/**
 * @brief Contains the ANSI code of the first 16 colors of the XTerm palette.
 */
typedef enum tmk_XColor
{
    /**
     * @brief The dark variant of the black color.
     */
    tmk_XColor_DarkBlack,
    /**
     * @brief The dark variant of the red color.
     */
    tmk_XColor_DarkRed,
    /**
     * @brief The dark variant of the green color.
     */
    tmk_XColor_DarkGreen,
    /**
     * @brief The dark variant of the yellow color.
     */
    tmk_XColor_DarkYellow,
    /**
     * @brief The dark variant of the blue color.
     */
    tmk_XColor_DarkBlue,
    /**
     * @brief The dark variant of the magenta color.
     */
    tmk_XColor_DarkMagenta,
    /**
     * @brief The dark variant of the cyan color.
     */
    tmk_XColor_DarkCyan,
    /**
     * @brief The dark variant of the white color.
     */
    tmk_XColor_DarkWhite,
    /**
     * @brief The light variant of the black color.
     */
    tmk_XColor_LightBlack,
    /**
     * @brief The light variant of the red color.
     */
    tmk_XColor_LightRed,
    /**
     * @brief The light variant of the green color.
     */
    tmk_XColor_LightGreen,
    /**
     * @brief The light variant of the yellow color.
     */
    tmk_XColor_LightYellow,
    /**
     * @brief The light variant of the blue color.
     */
    tmk_XColor_LightBlue,
    /**
     * @brief The light variant of the magenta color.
     */
    tmk_XColor_LightMagenta,
    /**
     * @brief The light variant of the cyan color.
     */
    tmk_XColor_LightCyan,
    /**
     * @brief The light variant of the white color.
     */
    tmk_XColor_LightWhite,
} tmk_XColor_t;

/**
 * @brief Contains the available terminal layers.
 */
typedef enum tmk_Layer
{
    /**
     * @brief Refers to the graphemes.
     */
    tmk_Layer_Foreground = 3,
    /**
     * @brief Refers to the background of the graphemes.
     */
    tmk_Layer_Background
} tmk_Layer_t;
#pragma endregion

#pragma region Structs
typedef struct tmk_RGBColor
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} tmk_RGBColor_t;
#pragma endregion

#pragma region Types
/**
 * @brief Represents a hex color. It must be a value in range from 0x0 to 0xffffff.
 */
typedef unsigned int tmk_HexColor_t;
#pragma endregion

#ifdef __cplusplus
extern "C"
{
#endif
#pragma region Functions
    /**
     * @brief Sets a XTerm color into a terminal layer.
     * @param color The color to be set. It must be a value in range from 0 to 255 or an enumerator from the tmk_XColor enum.
     * @param layer The layer to be affected.
     */
    void tmk_setXColor(unsigned char color, tmk_Layer_t layer);
    /**
     * @brief Sets an RGB color into a terminal layer.
     * @param color The color to be set.
     * @param layer The layer to be affected.
     */
    void tmk_setRGBColor(tmk_RGBColor_t color, tmk_Layer_t layer);
    /**
     * @brief Sets a hex color into a terminal layer.
     * @param color The color to be set.
     * @param layer The layer to be affected.
     */
    void tmk_setHexColor(tmk_HexColor_t color, tmk_Layer_t layer);
    /**
     * @brief Resets the terminal colors.
     */
    void tmk_resetColors(void);
#pragma endregion
#ifdef __cplusplus
}
#endif
#endif
