//------------------------------------------------------------------------------
//       _______    __                           ___
//      ||  ___ \  || |             __          //  |
//      || |  || | || |   _______  || |__      //   |    _____  ___
//      || |__|| | || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
//      ||  ____/  || | || |  || | || |      // /|| |   || |\\  /\\ \
//      || |       || | || |__|| | || |     // /_|| |_  || | || | || |
//      || |       || |  \\____  | || |__  //_____   _| || | || | || |
//      ||_|       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
//
//
// The MIT License (MIT)
//
// Copyright (c) 2013 Benjamin Minerd
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

///
/// @file RotarySwitch.h
/// @author Ben Minerd
/// @date 8/14/2013
/// @brief RotarySwitch class header file.
///

#ifndef PLAT4M_ROTARY_SWITCH_H
#define PLAT4M_ROTARY_SWITCH_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/UiInput.h>
#include <Plat4m_Core/EnableLine.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class RotarySwitch : public UiInput
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------
    
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
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    RotarySwitch(EnableLine* enableLine1 = 0,
                 EnableLine* enableLine2 = 0,
                 EnableLine* enableLine3 = 0,
                 EnableLine* enableLine4 = 0,
                 EnableLine* enableLine5 = 0,
                 EnableLine* enableLine6 = 0,
                 EnableLine* enableLine7 = 0,
                 EnableLine* enableLine8 = 0,
                 EnableLine* enableLine9 = 0,
                 EnableLine* enableLine10 = 0);
    
    //--------------------------------------------------------------------------
    // Public methods implemented from UiInput
    //--------------------------------------------------------------------------
    
    void poll(const uint32_t timeMs, Array<UiInput::Event>& events);
    
private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    EnableLine* myEnableLines[10];
    
    UiInput::State myStateLog[10];
    
    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------
    
    Module::Error driverEnable(const bool enable);
};

}; // namespace Plat4m

#endif // PLAT4M_ROTARY_SWITCH_H
