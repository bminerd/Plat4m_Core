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
 * Copyright (c) 2014 Benjamin Minerd
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
 * @file BoardStm32f4Discovery.cpp
 * @author Ben Minerd
 * @date 4/9/14
 * @brief BoardStm32f4Discovery class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <BoardStm32f4Discovery.h>

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

static const float coreVoltage                  = 3.0f;
static const uint32_t externalClockFrequencyHz  = 8000000;

static const GpioPin::Id gpioPinIdMap[] =
{
    // GPIO_PIN_ID_USER_BUTTON
    {.portId = GpioStm32f4xx::PORT_ID_A, .pinId = GpioStm32f4xx::PIN_ID_0}
};

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
BoardStm32f4Discovery::BoardStm32f4Discovery() :
    Board("STM32F4-DISCOVERY"),
    myMicro(NULL_POINTER),
    myUserButton(NULL_POINTER),
    myUserButtonEnableLine(NULL_POINTER)
{
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Button& BoardStm32f4Discovery::getButton()
{
    if (IS_NULL_POINTER(myUserButton))
    {
        myUserButton = new Button(getEnableLine());
    }
    
    return *myUserButton;
}

//------------------------------------------------------------------------------
EnableLine& BoardStm32f4Discovery::getEnableLine()
{
    if (IS_NULL_POINTER(myUserButtonEnableLine))
    {
        myUserButtonEnableLine = new EnableLine(EnableLine::MODE_INPUT,
                                                EnableLine::ACTIVE_LEVEL_HIGH,
                                                getGpioPin(),
                                                false);
    }
    
    return *myUserButtonEnableLine;
}

//------------------------------------------------------------------------------
GpioPin& BoardStm32f4Discovery::getGpioPin(const GpioPinId gpioPinId)
{
    return (GpioPinStm32f4xx::get(gpioPinIdMap[gpioPinId]));
}

//------------------------------------------------------------------------------
MicroStm32f4xx& BoardStm32f4Discovery::getMicro()
{
    return MicroStm32f4xx::get(coreVoltage, externalClockFrequencyHz);
}