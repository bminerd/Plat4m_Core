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
 * @file BoardStm3240gEval.cpp
 * @author Ben Minerd
 * @date 4/10/14
 * @brief BoardStm3240gEval class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <BoardStm3240gEval.h>

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

static const float coreVoltage                  = 3.3f;
static const uint32_t externalClockFrequencyHz  = 25000000;

static const GpioPin::Id gpioPinIdMap[] =
{
    // GPIO_PIN_ID_KEY_BUTTON
    {.portId = GpioStm32f4xx::PORT_ID_G, .pinId = GpioStm32f4xx::PIN_ID_15},
    // GPIO_PIN_ID_TAMPER_BUTTON
    {.portId = GpioStm32f4xx::PORT_ID_C, .pinId = GpioStm32f4xx::PIN_ID_13},
    // GPIO_PIN_ID_WAKEUP_BUTTON
    {.portId = GpioStm32f4xx::PORT_ID_A, .pinId = GpioStm32f4xx::PIN_ID_0}
};

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
BoardStm3240gEval::BoardStm3240gEval() :
    Board("STM3240G-EVAL"),
    myMicro(NULL_POINTER),
    myKeyButton(NULL_POINTER),
    myTamperButton(NULL_POINTER),
    myWakeupButton(NULL_POINTER),
    myKeyButtonEnableLine(NULL_POINTER),
    myTamperButtonEnableLine(NULL_POINTER),
    myWakeupButtonEnableLine(NULL_POINTER)
{
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Button& BoardStm3240gEval::getButton(const ButtonId buttonId)
{
    Button* button;
    
    switch (buttonId)
    {
        case BUTTON_ID_KEY:
        {
            if (IS_NULL_POINTER(myKeyButton))
            {
                myKeyButton = new Button(
                                      getEnableLine(ENABLE_LINE_ID_KEY_BUTTON));
            }
            
            button = myKeyButton;
            
            break;
        }
        case BUTTON_ID_TAMPER:
        {
            if (IS_NULL_POINTER(myTamperButton))
            {
                myTamperButton = new Button(
                                   getEnableLine(ENABLE_LINE_ID_TAMPER_BUTTON));
            }
            
            button = myTamperButton;
            
            break;
        }
        case BUTTON_ID_WAKEUP:
        {
            if (IS_NULL_POINTER(myWakeupButton))
            {
                myWakeupButton = new Button(
                                   getEnableLine(ENABLE_LINE_ID_WAKEUP_BUTTON));
            }
            
            button = myWakeupButton;
            
            break;
        }
    }
    
    return *button;
}

//------------------------------------------------------------------------------
EnableLine& BoardStm3240gEval::getEnableLine(const EnableLineId enableLineId)
{
    EnableLine* enableLine;
    
    switch (enableLineId)
    {
        case ENABLE_LINE_ID_KEY_BUTTON:
        {
            if (IS_NULL_POINTER(myKeyButtonEnableLine))
            {
                myKeyButtonEnableLine = new EnableLine(
                                                   EnableLine::MODE_INPUT,
                                                   EnableLine::ACTIVE_LEVEL_LOW,
                                                   getGpioPin(
                                                       GPIO_PIN_ID_KEY_BUTTON),
                                                   false);
            }
            
            enableLine = myKeyButtonEnableLine;
            
            break;
        }
        case ENABLE_LINE_ID_TAMPER_BUTTON:
        {
            if (IS_NULL_POINTER(myTamperButtonEnableLine))
            {
                myTamperButtonEnableLine = new EnableLine(
                                                   EnableLine::MODE_INPUT,
                                                   EnableLine::ACTIVE_LEVEL_LOW,
                                                   getGpioPin(
                                                    GPIO_PIN_ID_TAMPER_BUTTON),
                                                   false);
            }
            
            enableLine = myTamperButtonEnableLine;
            
            break;
        }
        case ENABLE_LINE_ID_WAKEUP_BUTTON:
        {
            if (IS_NULL_POINTER(myWakeupButtonEnableLine))
            {
                myWakeupButtonEnableLine = new EnableLine(
                                                   EnableLine::MODE_INPUT,
                                                   EnableLine::ACTIVE_LEVEL_HIGH,
                                                   getGpioPin(
                                                    GPIO_PIN_ID_WAKEUP_BUTTON),
                                                   false);
            }
            
            enableLine = myWakeupButtonEnableLine;
            
            break;
        }
    }
    
    return *enableLine;
}

//------------------------------------------------------------------------------
GpioPin& BoardStm3240gEval::getGpioPin(const GpioPinId gpioPinId)
{
    return (GpioPinStm32f4xx::get(gpioPinIdMap[gpioPinId]));
}

//------------------------------------------------------------------------------
MicroStm32f4xx& BoardStm3240gEval::getMicro()
{
    return MicroStm32f4xx::get(coreVoltage, externalClockFrequencyHz);
}