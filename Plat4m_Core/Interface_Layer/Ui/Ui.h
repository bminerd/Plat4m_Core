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
 * @file Ui.h
 * @author Ben Minerd
 * @date 6/28/2013
 * @brief Ui class.
 */

#ifndef _UI_H_
#define _UI_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <List.h>
#include <UiState.h>
#include <Task.h>
#include <Buffer.h>
#include <UiInput.h>

/*------------------------------------------------------------------------------
 * Forward class declarations
 *----------------------------------------------------------------------------*/

class UiItem;

/*------------------------------------------------------------------------------
 * classes
 *----------------------------------------------------------------------------*/

class Ui
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    enum Error
    {
        ERROR_NONE
    };
    
    /*--------------------------------------------------------------------------
     * Public constructors and destructors
     *------------------------------------------------------------------------*/
    
    Ui();
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/

    Error enable(const bool enable);
    
    Error isEnabled(bool& enable);
    
    Error addInput(UiInput* uiInput);
    
    Error addState(UiState* uiState);
    
    UiItem* getFocusItem();
    
    void setFocusItem(UiItem* focusItem);
    
    bool isFocusItem(UiItem& item);

private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    bool myIsEnabled;
    
    List<UiInput*> myInputList;
    
    Buffer<UiInput::EventTag, 10> myInputEventTagBuffer;
    
    List<UiState*> myStateList;
    
    UiState* myCurrentState;
    
    Task myInputTask;
    
    Task myEventTask;
    
    UiItem* myFocusItem;

    /*--------------------------------------------------------------------------
     * Private virtual methods
     *------------------------------------------------------------------------*/
    
    virtual Error driverEnable(const bool enable) = 0;
    
    /*--------------------------------------------------------------------------
     * Private methods
     *------------------------------------------------------------------------*/
    
    void inputTaskCallback();
    
    void eventTaskCallback();
};

#endif // _UI_H_