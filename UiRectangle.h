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
 * Copyright (c) 2016 Benjamin Minerd
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

///
/// @file UiRectangle.h
/// @author Ben Minerd
/// @date 9/19/2013
/// @brief UiRectangle class header file.
///

#ifndef UI_RECTANGLE_H
#define UI_RECTANGLE_H

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <UiItem.h>

/*------------------------------------------------------------------------------
 * Namespace
 *----------------------------------------------------------------------------*/

namespace Plat4m
{

/*------------------------------------------------------------------------------
 * Class
 *----------------------------------------------------------------------------*/

class UiRectangle : public UiItem
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/

    enum Fill
    {
        FILL_NONE = 0,
        FILL_SOLID
    };

    /*--------------------------------------------------------------------------
     * Public constructors
     *------------------------------------------------------------------------*/
    
    UiRectangle(UiPrinter& printer,
                UiView* view,
                UiItem* parent,
                const int width = 0,
                const int height = 0);
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/
    
    int getLineWidth() const;
    
    void setLineWidth(const int lineWidth);
    
    Fill getFill() const;

    void setFill(const Fill fill);

    // TODO: For now

    uint16_t getLineColor() const;

    void setLineColor(const uint16_t lineColor);

    uint16_t getFillColor() const;

    void setFillColor(const uint16_t fillColor);

private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    int myLineWidth;
    
    Fill myFill;

    // TODO: For now

    uint16_t myLineColor;

    uint16_t myFillColor;

    /*--------------------------------------------------------------------------
     * Private implemented methods
     *------------------------------------------------------------------------*/
    
    void driverPrint();
};

}; // namespace Plat4m

#endif // UI_RECTANGLE_H
