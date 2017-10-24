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
 * @file BoardStm3240gEval.h
 * @author Ben Minerd
 * @date 4/10/14
 * @brief BoardStm3240gEval class.
 */

#ifndef _BOARD_STM3240G_EVAL_H_
#define _BOARD_STM3240G_EVAL_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <Board.h>

//#include <Stm32f40x.h>
#include <MicroStm32f4xx.h>
#include <GpioPinStm32f4xx.h>

#include <Button.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class BoardStm3240gEval : public Board
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    enum ButtonId
    {
        BUTTON_ID_KEY = 0,
        BUTTON_ID_TAMPER,
        BUTTON_ID_WAKEUP
    };
    
    enum EnableLineId
    {
        ENABLE_LINE_ID_KEY_BUTTON = 0,
        ENABLE_LINE_ID_TAMPER_BUTTON,
        ENABLE_LINE_ID_WAKEUP_BUTTON
    };
    
    enum GpioPinId
    {
        GPIO_PIN_ID_KEY_BUTTON = 0,
        GPIO_PIN_ID_TAMPER_BUTTON,
        GPIO_PIN_ID_WAKEUP_BUTTON
    };
    
    /*--------------------------------------------------------------------------
     * Public constructors and destructors
     *------------------------------------------------------------------------*/
    
    BoardStm3240gEval();
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/

    Button& getButton(const ButtonId buttonId);
    
    EnableLine& getEnableLine(const EnableLineId enableLineId);
    
    GpioPin& getGpioPin(const GpioPinId gpioPinId);
    
    MicroStm32f4xx& getMicro();
    
private:

    // Micro
    MicroStm32f4xx* myMicro;

    // Button
    Button* myKeyButton;
    Button* myTamperButton;
    Button* myWakeupButton;

    // EnableLine / Led
    EnableLine* myKeyButtonEnableLine;
    EnableLine* myTamperButtonEnableLine;
    EnableLine* myWakeupButtonEnableLine;
};

#endif // _BOARD_STM3240G_EVAL_H_