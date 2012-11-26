/*------------------------------------------------------------------------------
 *                  __                           ___
 *                 || |             __          //  |
 *       _______   || |   _______  || |__      //   |    _____  ___
 *      ||  ___ \  || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
 *      || |  || | || | || |  || | || |      // /|| |   || |\\  /\\ \
 *      || |__|| | || | || |__|| | || |     // /_|| |_  || | || | || |
 *      ||  ____/  || |  \\____  | || |__  //_____   _| || | || | || |
 *      || |       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
 *      || |
 *      ||_|
 *
 * Copyright (c) 2012 Ben Minerd. All rights reserved.
 *
 * GNU Lesser General Public License Usage
 * This file may be used under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation and
 * appearing in the file LICENSE.LGPL included in the packaging of this file.
 * Please review the following information to ensure the GNU Lesser General
 * Public License version 2.1 requirements will be met:
 * http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *----------------------------------------------------------------------------*/

/**
 * @file display_driver_osd.c
 * @author Ben Minerd
 * @date 8/9/2012
 * @brief Display driver layer for the OSD OLED display.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <display_driver_osd.h>
#include <display_interface.h>

#include <system_interface.h>
#include <gpio_interface.h>
#include <font_arial.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * @brief Display command flag.
 */
#define DISPLAY_FLAG_COMMAND    0x80

/**
 * @brief Display data flag.
 */
#define DISPLAY_FLAG_DATA       0x40

/**
 * @brief Display data offset.
 */
#define DISPLAY_OFFSET_DATA     1

/**
 * @brief Display height in pixels.
 */
#define DISPLAY_HEIGHT_PIXELS    16

/**
 * @brief Display width in pixels.
 */
#define DISPLAY_WIDTH_PIXELS     96

/**
 * @brief Display height in bytes.
 */
#define DISPLAY_HEIGHT_BYTES     (DISPLAY_HEIGHT_PIXELS / 8)

/**
 * @brief Display width in bytes.
 */
#define DISPLAY_WIDTH_BYTES      (DISPLAY_WIDTH_PIXELS)

/**
 * @brief Display horizontal pixel center.
 */
#define DISPLAY_CENTER_HORIZ    (DISPLAY_WIDTH_PIXELS / 2)

/**
 * @brief Display vertical pixel center.
 */
#define DISPLAY_CENTER_VERT     (DISPLAY_HEIGHT_PIXELS / 2)

/**
 * @brief Display size in bytes.
 */
#define DISPLAY_SIZE_BYTES ((DISPLAY_HEIGHT_BYTES * DISPLAY_WIDTH_BYTES))

#define SECTION_CENTER_X                (16)
#define SECTION_CENTER_Y                (0)
#define SECTION_CENTER_WIDTH            (64)
#define SECTION_CENTER_HEIGHT           (16)

#define SECTION_CENTER_TOP_X            (16)
#define SECTION_CENTER_TOP_Y            (0)
#define SECTION_CENTER_TOP_WIDTH        (64)
#define SECTION_CENTER_TOP_HEIGHT       (8)

#define SECTION_CENTER_BOTTOM_X         (16)
#define SECTION_CENTER_BOTTOM_Y         (8)
#define SECTION_CENTER_BOTTOM_WIDTH     (64)
#define SECTION_CENTER_BOTTOM_HEIGHT    (8)

#define SECTION_LEFT_TOP_X              (0)
#define SECTION_LEFT_TOP_Y              (0)
#define SECTION_LEFT_TOP_WIDTH          (16)
#define SECTION_LEFT_TOP_HEIGHT         (8)

#define SECTION_LEFT_BOTTOM_X           (0)
#define SECTION_LEFT_BOTTOM_Y           (8)
#define SECTION_LEFT_BOTTOM_WIDTH       (16)
#define SECTION_LEFT_BOTTOM_HEIGHT      (8)

#define SECTION_RIGHT_TOP_X             (80)
#define SECTION_RIGHT_TOP_Y             (0)
#define SECTION_RIGHT_TOP_WIDTH         (16)
#define SECTION_RIGHT_TOP_HEIGHT        (8)

#define SECTION_RIGHT_BOTTOM_X          (80)
#define SECTION_RIGHT_BOTTOM_Y          (8)
#define SECTION_RIGHT_BOTTOM_WIDTH      (16)
#define SECTION_RIGHT_BOTTOM_HEIGHT     (8)

#define TRANSITION_HORIZ_INCREMENT      (16)
#define TRANSITION_VERT_INCREMENT       (4)

//#define TRANSITION_INCREMENT_DELAY      (40)
#define TRANSITION_INCREMENT_DELAY      (20)

#define OLED_WHITE

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of transition directions.
 */
typedef enum _transition_direction_e_
{
    TRANSITION_DIRECTION_UP,
    TRANSITION_DIRECTION_DOWN,
    TRANSITION_DIRECTION_LEFT,
    TRANSITION_DIRECTION_RIGHT
} transition_direction_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Display section type.
 */
typedef struct _display_section_t_
{
    display_section_e id;
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
} display_section_t;

/**
 * @brief Display data type.
 */
typedef struct _display_data_t_
{
    display_data_definition_t definition;
    byte_array_t data;
    unsigned int width;
    unsigned int height;
} display_data_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief Bytes for display communication.
 */
static uint8_t displayBytes[DISPLAY_SIZE_BYTES];

/**
 * @brief Section map.
 */
static display_section_t sections[DISPLAY_SECTION_COUNT] =
{
    // All section
    {
        .id     = DISPLAY_SECTION_ALL,
        .x      = 0,
        .y      = 0,
        .width  = DISPLAY_WIDTH_PIXELS,
        .height = DISPLAY_HEIGHT_PIXELS
    },
    // Center section
    {
        .id     = DISPLAY_SECTION_CENTER,
        .x      = SECTION_CENTER_X,
        .y      = SECTION_CENTER_Y,
        .width  = SECTION_CENTER_WIDTH,
        .height = SECTION_CENTER_HEIGHT
    },
    // Center-top section
    {
        .id     = DISPLAY_SECTION_CENTER_TOP,
        .x      = SECTION_CENTER_TOP_X,
        .y      = SECTION_CENTER_TOP_Y,
        .width  = SECTION_CENTER_TOP_WIDTH,
        .height = SECTION_CENTER_TOP_HEIGHT
    },
    // Center-bottom section
    {
        .id     = DISPLAY_SECTION_CENTER_BOTTOM,
        .x      = SECTION_CENTER_BOTTOM_X,
        .y      = SECTION_CENTER_BOTTOM_Y,
        .width  = SECTION_CENTER_BOTTOM_WIDTH,
        .height = SECTION_CENTER_BOTTOM_HEIGHT
    },
    // Left-top section
    {
        .id     = DISPLAY_SECTION_LEFT_TOP,
        .x      = SECTION_LEFT_TOP_X,
        .y      = SECTION_LEFT_TOP_Y,
        .width  = SECTION_LEFT_TOP_WIDTH,
        .height = SECTION_LEFT_TOP_HEIGHT
    },
    // Left-bottom section
    {
        .id     = DISPLAY_SECTION_LEFT_BOTTOM,
        .x      = SECTION_LEFT_BOTTOM_X,
        .y      = SECTION_LEFT_BOTTOM_Y,
        .width  = SECTION_LEFT_BOTTOM_WIDTH,
        .height = SECTION_LEFT_BOTTOM_HEIGHT
    },
    // Right-top section
    {
        .id     = DISPLAY_SECTION_RIGHT_TOP,
        .x      = SECTION_RIGHT_TOP_X,
        .y      = SECTION_RIGHT_TOP_Y,
        .width  = SECTION_RIGHT_TOP_WIDTH,
        .height = SECTION_RIGHT_TOP_HEIGHT
    },
    // Right-bottom section
    {
        .id     = DISPLAY_SECTION_RIGHT_BOTTOM,
        .x      = SECTION_RIGHT_BOTTOM_X,
        .y      = SECTION_RIGHT_BOTTOM_Y,
        .width  = SECTION_RIGHT_BOTTOM_WIDTH,
        .height = SECTION_RIGHT_BOTTOM_HEIGHT
    }
};

/**
 * @brief Font map.
 */
static const tFont* fontMap[] =
{
    &arial_14pt_vert,   /// DISPLAY_FONT_ARIAL_14PT
    &arial_12pt_vert,   /// DISPLAY_FONT_ARIAL_12PT
    &arial_7pt_vert     /// DISPLAY_FONT_ARIAL_7PT
};

static gpio_id_t gpioRstN =
{
    .gpio.port  = GPIO_PORT_E,
    .gpio.pin   = GPIO_PIN_8
};

static gpio_id_t gpioVccEn =
{
    .gpio.port  = GPIO_PORT_E,
    .gpio.pin   = GPIO_PIN_9
};

static gpio_id_t gpioPwrEn =
{
    .gpio.port  = GPIO_PORT_E,
    .gpio.pin   = GPIO_PIN_10
};

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Sets the given display enabled or disabled.
 * @param display Display to access.
 * @param enabled Flag that indicates if the display should be enabled or
 * disabled.
 * @return Display error.
 */
static display_error_e setEnabled(const display_t* display, const bool enabled);

/**
 * @brief Configures the given display.
 * @param display Display to access.
 * @param id Display to access.
 * @param config Display config.
 * @return Display error.
 */
static display_error_e configure(const display_t* display,
                                 const display_config_t* config);

/**
 * @brief Writes data to the given display.
 * @param display Display to access.
 * @param definition Display data definition.
 * @param data Display data to write.
 * @return Display error.
 */
static display_error_e writeData(const display_t* display,
                                 const display_data_definition_t* definition,
                                 const data_array_t* data);

/**
 * @brief Writes text to the given display.
 * @param display Display to access.
 * @param definition Display data definition.
 * @param fontId Font ID.
 * @param text Text string to write.
 * @return Display error.
 */
static display_error_e writeText(const display_t* display,
                                 const display_data_definition_t* definition,
                                 const display_font_e fontId,
                                 const char* text);

/**
 * @brief Clears the given display.
 * @param display Display to access.
 * @return Display error.
 */
static display_error_e clear(const display_t* display);

/**
 * @brief Sets the given display brightness.
 * @param display Display to access.
 * @param brightness Brightness value.
 * @return Display error.
 */
static display_error_e setBrightness(const display_t* display,
                                     const uint8_t brightness);

/**
 * TODO Comment!
 */
static display_error_e updateDisplay(const display_osd_t* display,
                                     const display_data_t* data);

/**
 * TODO Comment!
 */
static display_error_e slideHoriz(const display_osd_t* display,
                                  const transition_direction_e direction,
                                  const int areaStart,
                                  const int areaEnd,
                                  const int areaSize,
                                  const int areaWidth,
                                  const int areaHeight,
                                  const int increment,
                                  const uint8_t* oldFrameBytes,
                                  const uint8_t* newFrameBytes,
                                  const byte_array_t* data,
                                  const uint32_t transitionDelay);

/**
 * TODO Comment!
 */
static display_error_e slideVert(const display_osd_t* display,
                                 const transition_direction_e direction,
                                 const int areaStart,
                                 const int areaEnd,
                                 const int areaSize,
                                 const int areaWidth,
                                 const int areaHeight,
                                 const int increment,
                                 const uint8_t* oldFrameBytes,
                                 const uint8_t* newFrameBytes,
                                 const byte_array_t* data,
                                 const uint32_t transitionDelay);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void displayDriverOsdInit(void)
{
    display_driver_t driver =
    {
        .setEnabled     = setEnabled,
        .configure      = configure,
        .writeData      = writeData,
        .writeText      = writeText,
        .clear          = clear,
        .setBrightness  = setBrightness
    };

    displaySetDriver(&driver);

    gpio_config_t gpioConfig =
    {
        .mode     = GPIO_MODE_DIGITAL_OUTPUT,
        .resistor = GPIO_RESISTOR_NONE
    };

    // RST_N
    gpioInitId(&gpioRstN);
    gpioSetEnabled(&gpioRstN, true);
    gpioConfigure(&gpioRstN, &gpioConfig);

    // VCC_EN
    gpioInitId(&gpioVccEn);
    gpioSetEnabled(&gpioVccEn, true);
    gpioConfigure(&gpioVccEn, &gpioConfig);

    // PWR_EN
    gpioInitId(&gpioPwrEn);
    gpioSetEnabled(&gpioPwrEn, true);
    gpioConfigure(&gpioPwrEn, &gpioConfig);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static display_error_e setEnabled(const display_t* display, const bool enabled)
{
    // Cast to this display
    display_osd_t* thisDisplay = (display_osd_t*) display;

    int i = 0;
    byte_array_t data;
    data.bytes = displayBytes;

    if (enabled)
    {

#ifdef OLED_WHITE

        // Enable V_DD
        gpioSetLevel(&gpioPwrEn, GPIO_LEVEL_HIGH);
        // Wait for V_DD to stabilize
        systemDelayMs(10);
        // Pull RST low
        gpioSetLevel(&gpioRstN, GPIO_LEVEL_LOW);
        systemDelayMs(1);
        // Push RST high
        gpioSetLevel(&gpioRstN, GPIO_LEVEL_HIGH);
        systemDelayMs(1);

        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xAE;

        data.size = i;

        i2c_error_e error = i2cMasterTx(thisDisplay->i2cDeviceId, &data);

        if (error != I2C_ERROR_NONE)
        {
            return DISPLAY_ERROR_COMMUNICATION;
        }

        i = 0;
        // Set display clock divide ratio / oscillator frequency to ???
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xD5;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xF0;
        // Set multiplex ratio to ???
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xA8;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x0F;
        // Set display offset to ???
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xD3;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x1F;
        // Set display start line to ???
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x40;
        // Set segment re-map to???
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xA0;
        // Set COM scan direction to ???
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xC8;
        // Set COM pins hardware configuration to ???
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xDA;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x12;
        // Set contrast control to ???
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x81;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xFF;
        // Set pre-charge period to ???
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xD9;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x22;
        // Set VCOMH level to ???
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xDB;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x30;
        // Set memory addressing mode to ???
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x20;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x01;
        // Set column start and end address
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x21;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 32;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 127;
        // Set page start and end address
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x22;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x00;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x01;
        // Set entire display on/off
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xA4;
        // Set display normal
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xA6;

        data.size = i;

        if (i2cMasterTx(thisDisplay->i2cDeviceId, &data) != I2C_ERROR_NONE)
        {
            return DISPLAY_ERROR_COMMUNICATION;
        }

        display_error_e displayError = clear(thisDisplay);

        if (displayError != DISPLAY_ERROR_NONE)
        {
            return displayError;
        }

        gpioSetLevel(&gpioVccEn, GPIO_LEVEL_HIGH);
        systemDelayMs(100);

        i = 0;

        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xAF;

        data.size = i;

        if (i2cMasterTx(thisDisplay->i2cDeviceId, &data) != I2C_ERROR_NONE)
        {
            return DISPLAY_ERROR_COMMUNICATION;
        }

        systemDelayMs(100);
#endif

#ifdef OLED_BLUE

        // Pull RST low
        gpioSetLevel(&gpioRstN, GPIO_LEVEL_LOW);
        // Enable V_DD
        gpioSetLevel(&gpioPwrEn, GPIO_LEVEL_HIGH);
        // Wait for V_DD to stabilize
        systemDelayMs(10);
        // Push RST high
        gpioSetLevel(&gpioRstN, GPIO_LEVEL_HIGH);
        systemDelayMs(1);

        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xAE;

        data.size = i;

        if (i2cMasterTx(thisDisplay->i2cDeviceId, &data) != I2C_ERROR_NONE)
        {
            return DISPLAY_ERROR_COMMUNICATION;
        }

        i = 0;
        // Set display clock divide ratio / oscillator frequency to ???
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xD5;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xF0;
        // Set multiplex ratio to ???
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xA8;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x0F;
        // Set display offset to ???
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xD3;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x00;
        // Set display start line to ???
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x40;
        // Set charge pump to ???
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x8D;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x10;
        // Set segment re-map to???
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xA0;
        // Set COM scan direction to ???
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xC0;
        // Set COM pins hardware configuration to ???
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xDA;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x02;
        // Set contrast control to ???
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x81;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xFF;
        // Set pre-charge period to ???
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xD9;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xF1;
        // Set VCOMH level to ???
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xDB;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x30;
        // Set memory addressing mode to ???
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x20;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x01;
        // Set column start and end address
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x21;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 32;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 127;
        // Set page start and end address
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x22;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x00;
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0x01;
        // Set entire display on/off
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xA4;
        // Set display normal
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xA6;

        data.size = i;

        if (i2cMasterTx(thisDisplay->i2cDeviceId, &data) != I2C_ERROR_NONE)
        {
            return DISPLAY_ERROR_COMMUNICATION;
        }

        gpioSetLevel(&gpioVccEn, GPIO_LEVEL_HIGH);
        systemDelayMs(100);

        display_error_e displayError = clear(thisDisplay);

        if (displayError != DISPLAY_ERROR_NONE)
        {
            return displayError;
        }

        i = 0;

        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xAF;

        data.size = i;

        if (i2cMasterTx(display->i2cDeviceId, &data) != I2C_ERROR_NONE)
        {
            return DISPLAY_ERROR_COMMUNICATION;
        }

        systemDelayMs(100);

#endif

    }
    else
    {
        displayBytes[i++] = DISPLAY_FLAG_COMMAND;
        displayBytes[i++] = 0xAE;

        data.size = i;

        if (i2cMasterTx(thisDisplay->i2cDeviceId, &data) != I2C_ERROR_NONE)
        {
            return DISPLAY_ERROR_COMMUNICATION;
        }

        // Disable V_CC
        gpioSetLevel(&gpioVccEn, GPIO_LEVEL_LOW);
        systemDelayMs(100);
        // Disable V_DD
        gpioSetLevel(&gpioPwrEn, GPIO_LEVEL_LOW);
    }

    return DISPLAY_ERROR_NONE;
}

//------------------------------------------------------------------------------
static display_error_e configure(const display_t* display,
                                 const display_config_t* config)
{
    return DISPLAY_ERROR_NONE;
}

//------------------------------------------------------------------------------
static display_error_e writeData(const display_t* display,
                                 const display_data_definition_t* definition,
                                 const data_array_t* data)
{
    display_data_t displayData;
    displayData.definition = *definition;
    displayData.data = data->data;
    displayData.width = data->data.size / 2;
    displayData.height = sections[definition->section].height;

    return updateDisplay(display, &displayData);
}

//------------------------------------------------------------------------------
static display_error_e writeText(const display_t* display,
                                 const display_data_definition_t* definition,
                                 const display_font_e fontId,
                                 const char* text)
{
    // Cast to this display
    display_osd_t* thisDisplay = (display_osd_t*) display;

    int i = 0;
    int j = 0;
    int charIndex = 0;
    int row1Width = 0;
    int row2Width = 0;
    int size = 0;
    int dataIndex = 0;
    int textLen = strlen(text);
    int increment = 1;
    int splitIndex = 0;
    const tFont* font;

    switch (fontId)
    {
        case DISPLAY_FONT_ARIAL_AUTO:
        {
            font = fontMap[DISPLAY_FONT_ARIAL_12PT];

            break;
        }
        default:
        {
            font = fontMap[fontId];

            break;
        }
    }

    for (i = 0; i < textLen; i++)
    {
        charIndex = text[i] - 0x20;
        row1Width += font->chars[charIndex].image->width;
    }

    // If length of text data is longer than chosen section
    if (row1Width > sections[definition->section].width)
    {
        if (fontId == DISPLAY_FONT_ARIAL_AUTO)
        {
            // Search for a space in the string
            for (i = 0; i < textLen; i++)
            {
                // If a space exists, we can split into two rows
                if (text[i] == ' ')
                {
                    splitIndex = i;
                    font = fontMap[DISPLAY_FONT_ARIAL_7PT];
                    increment = 2;
                    row1Width = 0;
                    row2Width = 0;

                    break;
                }
            }

            // If we reached the end of the string, we didn't find a space and can't
            // split the string --> return false
            if (i == textLen)
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    for (i = 0; i < textLen; i++)
    {
        charIndex = text[i] - 0x20;

        if (i < splitIndex)
        {
            row1Width += font->chars[charIndex].image->width;
        }
        else if (i > splitIndex)
        {
            row2Width += font->chars[charIndex].image->width;
        }
    }

    display_data_t data;
    data.definition = *definition;
    data.data.bytes = displayBytes;

    for (i = 0; i < textLen; i++)
    {
        charIndex = text[i] - 0x20;

        if ((i == splitIndex) && (i != 0))
        {
            dataIndex = 1;
        }
        else
        {
            size = font->chars[charIndex].image->width *
                   (font->chars[charIndex].image->height / 8);

            for (j = 0; j < size; j++)
            {
                displayBytes[dataIndex] = font->chars[charIndex].image->data[j];
                dataIndex += increment;
            }
        }
    }

    data.data.size = dataIndex;
    data.width = row1Width;
    data.height = sections[definition->section].height;

    return updateDisplay(thisDisplay, &data);
}

//------------------------------------------------------------------------------
static display_error_e clear(const display_t* display)
{
    // Cast to this display
    display_osd_t* thisDisplay = (display_osd_t*) display;

    int i = 0;

    // Reset memory pointer to top corner of display
    displayBytes[i++] = DISPLAY_FLAG_COMMAND;
    displayBytes[i++] = 0x21;
    displayBytes[i++] = DISPLAY_FLAG_COMMAND;
    displayBytes[i++] = 32;
    displayBytes[i++] = DISPLAY_FLAG_COMMAND;
    displayBytes[i++] = 127;

    byte_array_t data;
    data.bytes = displayBytes;
    data.size = i;

    if (i2cMasterTx(thisDisplay->i2cDeviceId, &data) != I2C_ERROR_NONE)
    {
        return DISPLAY_ERROR_COMMUNICATION;
    }

    data.size = 0;

    display_data_t displayData;
    displayData.definition.section = DISPLAY_SECTION_ALL;
    displayData.definition.alignment = DISPLAY_ALIGNMENT_LEFT;
    displayData.definition.transition = DISPLAY_TRANSITION_NONE;
    displayData.data = data;

    return updateDisplay(thisDisplay, &displayData);
}

//------------------------------------------------------------------------------
static display_error_e setBrightness(const display_t* display,
                                     const uint8_t brightness)
{
    // Cast to this display
    display_osd_t* thisDisplay = (display_osd_t*) display;

    int i = 0;

    displayBytes[i++] = DISPLAY_FLAG_COMMAND;
    displayBytes[i++] = 0x81;
    displayBytes[i++] = DISPLAY_FLAG_COMMAND;
    displayBytes[i++] = brightness;

    byte_array_t data;
    data.bytes = displayBytes;
    data.size = i;

    if (i2cMasterTx(thisDisplay->i2cDeviceId, &data) != I2C_ERROR_NONE)
    {
        return DISPLAY_ERROR_COMMUNICATION;
    }

    return DISPLAY_ERROR_NONE;
}

//------------------------------------------------------------------------------
static display_error_e updateDisplay(const display_osd_t* display,
                                     const display_data_t* data)
{
    // The current display data
    static uint8_t bytes[DISPLAY_SIZE_BYTES + 1] = {DISPLAY_FLAG_DATA};
    // The current display frame (before a transition)
    static uint8_t oldBytes[DISPLAY_SIZE_BYTES + 1] = {DISPLAY_FLAG_DATA};
    //
    static uint8_t newBytes[DISPLAY_SIZE_BYTES + 1] = {DISPLAY_FLAG_DATA};
    static byte_array_t i2cData = BYTE_ARRAY(bytes);

    int i;
    int increment;
    int dataIndex;
    int areaWidth = sections[data->definition.section].width;
    int areaHeight = sections[data->definition.section].height;
    int areaStart = sections[data->definition.section].x * 2;
    int areaSize = areaWidth * (areaHeight / 8);
    int areaEnd = areaStart + areaSize;

    switch (data->definition.alignment)
    {
        case DISPLAY_ALIGNMENT_LEFT:
        {
            dataIndex = areaStart + DISPLAY_OFFSET_DATA;

            break;
        }
        case DISPLAY_ALIGNMENT_CENTER:
        {
            dataIndex = areaStart +
                        (((areaWidth / 2) -
                         (data->width / 2)) * 2) + 1;

            break;
        }
        case DISPLAY_ALIGNMENT_RIGHT:
        {
            dataIndex = areaStart + ((areaWidth - data->width) * 2) + 1;

            break;
        }
    }

    // Save the current data into a frame
    memcpy(oldBytes, bytes, ARRAY_SIZE(bytes));

    // New frame starts from old frame
    memcpy(newBytes, oldBytes, ARRAY_SIZE(bytes));

    // Clear the current area we will use
    memset(&(newBytes[areaStart + 1]), 0, areaSize);

    // The following control statements allow the incoming data to be shorter
    // than the full display height1

    // If the current section doesn't start at page 0, increase the index by 1
    if (sections[data->definition.section].y == 8)
    {
        dataIndex++;
    }

    // If the data is 2 pages high, the increment should include every byte
    if (data->height == 16)
    {
        increment = 1;
    }
    // If the data is 1 page high, the increment should include every other byte
    else if (data->height == 8)
    {
        increment = 2;
    }

    // Copy the new data into a full frame and save it
    for (i = 0; i < data->data.size; i++)
    {
        newBytes[dataIndex] = data->data.bytes[i];
        dataIndex += increment;
    }

    switch (data->definition.transition)
    {
        case DISPLAY_TRANSITION_INSTANT:
        {
            memcpy(bytes, newBytes, ARRAY_SIZE(newBytes));

            if (i2cMasterTx(display->i2cDeviceId, &i2cData) !=
                    I2C_ERROR_NONE)
            {
                return DISPLAY_ERROR_COMMUNICATION;
            }

            return DISPLAY_ERROR_NONE;
        }
        case DISPLAY_TRANSITION_SLIDE_UP:
        {
            return slideVert(display,
                             TRANSITION_DIRECTION_UP,
                             areaStart,
                             areaEnd,
                             areaSize,
                             areaWidth,
                             areaHeight,
                             TRANSITION_VERT_INCREMENT,
                             oldBytes,
                             newBytes,
                             &i2cData,
                             TRANSITION_INCREMENT_DELAY);
        }
        case DISPLAY_TRANSITION_SLIDE_UP_CHANGED:
        {
            int changedStart = -1;
            int changedEnd = -1;

            // Determine what changed from the old frame to the new frame within
            // the desired area
            for (i = areaStart; i < areaEnd; i++)
            {
                if (changedStart == -1)
                {
                    if (newBytes[i] != oldBytes[i])
                    {
                        changedStart = i;
                    }
                }
                else
                {
                    if (newBytes[i] != oldBytes[i])
                    {
                        changedEnd = i;
                    }
                }
            }

            areaStart = changedStart;
            areaEnd = changedEnd;
            areaSize = changedEnd - changedStart;

            return slideVert(display,
                             TRANSITION_DIRECTION_UP,
                             areaStart,
                             areaEnd,
                             areaSize,
                             areaWidth,
                             areaHeight,
                             TRANSITION_VERT_INCREMENT,
                             oldBytes,
                             newBytes,
                             &i2cData,
                             TRANSITION_INCREMENT_DELAY);
        }
        case DISPLAY_TRANSITION_SLIDE_DOWN:
        {
            return slideVert(display,
                             TRANSITION_DIRECTION_DOWN,
                             areaStart,
                             areaEnd,
                             areaSize,
                             areaWidth,
                             areaHeight,
                             TRANSITION_VERT_INCREMENT,
                             oldBytes,
                             newBytes,
                             &i2cData,
                             TRANSITION_INCREMENT_DELAY);
        }
        case DISPLAY_TRANSITION_SLIDE_DOWN_CHANGED:
        {
            int changedStart = -1;
            int changedEnd = -1;

            // Determine what changed from the old frame to the new frame within
            // the desired area
            for (i = areaStart; i < areaEnd; i++)
            {
                if (changedStart == -1)
                {
                    if (newBytes[i] != oldBytes[i])
                    {
                        changedStart = i;
                    }
                }
                else
                {
                    if (newBytes[i] != oldBytes[i])
                    {
                        changedEnd = i;
                    }
                }
            }

            areaStart = changedStart;
            areaEnd = changedEnd;
            areaSize = changedEnd - changedStart;

            return slideVert(display,
                             TRANSITION_DIRECTION_DOWN,
                             areaStart,
                             areaEnd,
                             areaSize,
                             areaWidth,
                             areaHeight,
                             TRANSITION_VERT_INCREMENT,
                             oldBytes,
                             newBytes,
                             &i2cData,
                             TRANSITION_INCREMENT_DELAY);
        }
        case DISPLAY_TRANSITION_SLIDE_LEFT:
        {
            return slideHoriz(display,
                              TRANSITION_DIRECTION_LEFT,
                              areaStart,
                              areaEnd,
                              areaSize,
                              areaWidth,
                              areaHeight,
                              TRANSITION_HORIZ_INCREMENT,
                              oldBytes,
                              newBytes,
                              &i2cData,
                              TRANSITION_INCREMENT_DELAY);
        }
        case DISPLAY_TRANSITION_SLIDE_RIGHT:
        {
            return slideHoriz(display,
                              TRANSITION_DIRECTION_RIGHT,
                              areaStart,
                              areaEnd,
                              areaSize,
                              areaWidth,
                              areaHeight,
                              TRANSITION_HORIZ_INCREMENT,
                              oldBytes,
                              newBytes,
                              &i2cData,
                              TRANSITION_INCREMENT_DELAY);
        }
        default:
        {
            break;
        }
    }

    return DISPLAY_ERROR_NONE;
}

//------------------------------------------------------------------------------
static display_error_e slideHoriz(const display_osd_t* display,
                                  const transition_direction_e direction,
                                  const int areaStart,
                                  const int areaEnd,
                                  const int areaSize,
                                  const int areaWidth,
                                  const int areaHeight,
                                  const int increment,
                                  const uint8_t* oldFrameBytes,
                                  const uint8_t* newFrameBytes,
                                  const byte_array_t* data,
                                  const uint32_t transitionDelay)
{
    int i;

    for (i = increment * 2; i <= areaSize; i += increment * 2)
    {
        switch (direction)
        {
            case TRANSITION_DIRECTION_LEFT:
            {
                memcpy(&data->bytes[areaStart + DISPLAY_OFFSET_DATA],
                       &oldFrameBytes[i + areaStart + DISPLAY_OFFSET_DATA],
                       areaSize - i);

                memcpy(&data->bytes[areaEnd - i + DISPLAY_OFFSET_DATA],
                       &(newFrameBytes[areaStart + DISPLAY_OFFSET_DATA]),
                       i);

                break;
            }
            case TRANSITION_DIRECTION_RIGHT:
            {
                memcpy(&data->bytes[i + areaStart + DISPLAY_OFFSET_DATA],
                       &oldFrameBytes[areaStart + DISPLAY_OFFSET_DATA],
                       areaSize - i);

                memcpy(&data->bytes[areaStart + DISPLAY_OFFSET_DATA],
                       &(newFrameBytes[areaEnd - i + DISPLAY_OFFSET_DATA]),
                       i);

                break;
            }
            default:
            {
                break;
            }
        }

        if (i2cMasterTx(display->i2cDeviceId, data) != I2C_ERROR_NONE)
        {
            return DISPLAY_ERROR_COMMUNICATION;
        }

        systemDelayMs(transitionDelay);
    }

    return DISPLAY_ERROR_NONE;
}

//------------------------------------------------------------------------------
static display_error_e slideVert(const display_osd_t* display,
                                 const transition_direction_e direction,
                                 const int areaStart,
                                 const int areaEnd,
                                 const int areaSize,
                                 const int areaWidth,
                                 const int areaHeight,
                                 const int increment,
                                 const uint8_t* oldFrameBytes,
                                 const uint8_t* newFrameBytes,
                                 const byte_array_t* data,
                                 const uint32_t transitionDelay)
{
    int i, j, dataIndex;
    uint16_t column;
    uint16_t newColumn;

    for (i = 0; i <= areaHeight; i += increment)
    {
        for (j = areaStart; j < areaEnd; j += 2)
        {
            dataIndex = j + DISPLAY_OFFSET_DATA;

            // Shift old frame pixels (bits) down by increment
            column = ((uint16_t) oldFrameBytes[dataIndex]) |
                     (((uint16_t) oldFrameBytes[dataIndex + 1]) << 8);
            newColumn = ((uint16_t) newFrameBytes[dataIndex]) |
                        (((uint16_t) newFrameBytes[dataIndex + 1]) << 8);

            switch (direction)
            {
                case TRANSITION_DIRECTION_UP:
                {
                    column = column >> i;
                    newColumn = newColumn << (areaHeight - i);

                    break;
                }
                case TRANSITION_DIRECTION_DOWN:
                {
                    column = column << i;
                    newColumn = newColumn >> (areaHeight - i);

                    break;
                }
            }

            // Combine
            column = column | newColumn;

            data->bytes[dataIndex] = (uint8_t) column;
            data->bytes[dataIndex + 1] = (uint8_t) (column >> 8);
        }

        if (i2cMasterTx(display->i2cDeviceId, data) != I2C_ERROR_NONE)
        {
            return DISPLAY_ERROR_COMMUNICATION;
        }

        systemDelayMs(transitionDelay);
    }

    return DISPLAY_ERROR_NONE;
}
