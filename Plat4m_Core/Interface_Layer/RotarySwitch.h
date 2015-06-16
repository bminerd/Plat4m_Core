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
 * @file RotarySwitch.h
 * @author Ben Minerd
 * @date 8/14/2013
 * @brief RotarySwitch class.
 */

#ifndef _ROTARY_SWITCH_H_
#define _ROTARY_SWITCH_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <UiInput.h>
#include <EnableLine.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class RotarySwitch : public UiInput
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    struct Event
    {
        enum Id
        {
            ID_POSITION_1 = 0,
            ID_POSITION_2,
            ID_POSITION_3,
            ID_POSITION_4,
            ID_POSITION_5,
            ID_POSITION_6,
            ID_POSITION_7,
            ID_POSITION_8,
            ID_POSITION_9,
            ID_POSITION_10
        };
    };
    
    /*--------------------------------------------------------------------------
     * Public constructors and destructors
     *------------------------------------------------------------------------*/
    
    RotarySwitch(EnableLine* enableLine1 = NULL_POINTER,
                 EnableLine* enableLine2 = NULL_POINTER,
                 EnableLine* enableLine3 = NULL_POINTER,
                 EnableLine* enableLine4 = NULL_POINTER,
                 EnableLine* enableLine5 = NULL_POINTER,
                 EnableLine* enableLine6 = NULL_POINTER,
                 EnableLine* enableLine7 = NULL_POINTER,
                 EnableLine* enableLine8 = NULL_POINTER,
                 EnableLine* enableLine9 = NULL_POINTER,
                 EnableLine* enableLine10 = NULL_POINTER);
    
    /*--------------------------------------------------------------------------
     * Public implemented methods
     *------------------------------------------------------------------------*/
    
    void poll(const uint32_t timeMs, Array<UiInput::Event>& events);
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/
    
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    EnableLine* myEnableLines[10];
    
    UiInput::State myStateLog[10];
    
    /*--------------------------------------------------------------------------
     * Private implemented methods
     *------------------------------------------------------------------------*/
    
    Error driverEnable(const bool enable);
};

#endif // _ROTARY_SWITCH_H_