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
 * @file RotarySwitch.cpp
 * @author Ben Minerd
 * @date 7/23/2013
 * @brief RotarySwitch class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <RotarySwitch.h>

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

static const uint32_t debounceTimeMs = 10;

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
RotarySwitch::RotarySwitch(EnableLine* enableLine1,
                           EnableLine* enableLine2,
                           EnableLine* enableLine3,
                           EnableLine* enableLine4,
                           EnableLine* enableLine5,
                           EnableLine* enableLine6,
                           EnableLine* enableLine7,
                           EnableLine* enableLine8,
                           EnableLine* enableLine9,
                           EnableLine* enableLine10) :
    UiInput()
{
    myEnableLines[0] = enableLine1;
    myEnableLines[1] = enableLine2;
    myEnableLines[2] = enableLine3;
    myEnableLines[3] = enableLine4;
    myEnableLines[4] = enableLine5;
    myEnableLines[5] = enableLine6;
    myEnableLines[6] = enableLine7;
    myEnableLines[7] = enableLine8;
    myEnableLines[8] = enableLine9;
    myEnableLines[9] = enableLine10;
    
    for (int i = 0; i < ARRAY_SIZE(myStateLog); i++)
    {
        myStateLog[i].isActive      = false;
        myStateLog[i].timeStampMs   = 0;
    }
}

/*------------------------------------------------------------------------------
 * Public implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void RotarySwitch::poll(const uint32_t timeMs, Array<UiInput::Event>& events)
{
    for (int i = 0; i < ARRAY_SIZE(myEnableLines); i++)
    {
        EnableLine* enableLine = myEnableLines[i];
        
        if (IS_VALID_POINTER(enableLine))
        {
            bool isActive;
            
            enableLine->isActive(isActive);
            
            if ((isActive != myStateLog[i].isActive) &&
                ((timeMs - myStateLog[i].timeStampMs) >= debounceTimeMs))
            {
                if (isActive)
                {
                    // Should the event be created here? (vs Ui)
                    UiInput::Event event;
                    event.id        = i;
                    event.timeMs    = timeMs;
                    
                    events.append(event);
                }
                
                myStateLog[i].isActive      = isActive;
                myStateLog[i].timeStampMs   = timeMs;
            }
        }
    }
}

/*------------------------------------------------------------------------------
 * Private implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
UiInput::Error RotarySwitch::driverEnable(const bool enable)
{
    for (int i = 0; i < ARRAY_SIZE(myEnableLines); i++)
    {
        EnableLine* enableLine = myEnableLines[i];
        
        if (IS_VALID_POINTER(enableLine))
        {
            enableLine->enable(enable);
        }
    }
    
    return ERROR_NONE;
}