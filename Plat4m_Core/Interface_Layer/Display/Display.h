/*------------------------------------------------------------------------------
 *       _______    __                           ___
 *      ||  ___ \  || |             __          //  |
 *      || |  || | || |   _______  || |__      //   |    _____  ___
 *      || |__|| | || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
 *      ||  ____/  || | || |  || | || |      // /|| |   || |\\  /\\ \
 *      || |       || | || |__|| | || |     // /_|| |_  || | || | || |
 *      || |       || |  \\____  | || |__  //_____   _| || | || | || |
 *      ||_|       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
 *
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 Benjamin Minerd
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *----------------------------------------------------------------------------*/

/**
 * @file Display.h
 * @author Ben Minerd
 * @date 4/11/2013
 * @brief Display class.
 */

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class Display
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Enumeration of display errors.
     */
    enum Error
    {
        ERROR_NONE,
        ERROR_PARAMETER_INVALID,
        ERROR_SECTION_INVALID,
        ERROR_ALIGNMENT_INVALID,
        ERROR_TRANSITION_INVALID,
        ERROR_FONT_INVALID,
        ERROR_NOT_ENABLED,
        ERROR_COMMUNICATION
    };
    
    /*--------------------------------------------------------------------------
     * Public structures
     *------------------------------------------------------------------------*/
    
    struct Config
    {
        int a; // Placeholder
    };
    
    struct Frame // Maybe should be a class for parameter safety
    {
        uint8_t* data;
        unsigned int size;
        unsigned int width;
        unsigned int height;
    };
    
    /*--------------------------------------------------------------------------
     * Public virtual destructors
     *------------------------------------------------------------------------*/
    
    virtual ~Display();
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/

    unsigned int getWidth() const;
    
    unsigned int getHeight() const;
    
    Frame& getFrame();
    
    /**
     * @brief Sets this display enabled or disabled.
     * @param enable Flag that indicates if this display should be enabled or
     * disabled.
     * @return Display error.
     */
    Error enable(const bool enable);

    /**
     * @brief Checks to see if this display is enabled or disabled.
     * @param isEnabled Flag that indicates if this display is enabled or
     * disabled.
     * @return Display error.
     */
    Error isEnabled(bool& isEnabled);

    /**
     * @brief Configures this display.
     * @param config Display configuration.
     * @return Display error.
     */
    Error configure(const Config& config);

    Error writeFrame();

    /**
     * @brief Clears this display.
     * @param display Display to access.
     * @return Display error.
     */
    Error clear();

    /**
     * @brief Sets this display brightness.
     * @param display Display to access.
     * @param brightness Brightness value.
     * @return Display error.
     */
    Error setBrightnessPercent(const float brightnessPercent);
    
protected:
    
    /*--------------------------------------------------------------------------
     * Protected constructors
     *------------------------------------------------------------------------*/

    Display(Frame& frame);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    bool myIsEnabled;
    
    Frame& myFrame;
    
    /*--------------------------------------------------------------------------
     * Private virtual methods
     *------------------------------------------------------------------------*/
    
    virtual Error driverEnable(const bool enable) = 0;
    
    virtual Error driverConfigure(const Config& config) = 0;
    
    virtual Error driverWriteFrame() = 0;
    
    virtual Error driverClear() = 0;
    
    virtual Error driverSetBrightnessPercent(const float brightnessPercent) = 0;
};

#endif // _DISPLAY_H_
