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
/// @file UiRectangle.cpp
/// @author Ben Minerd
/// @date 9/19/2013
/// @brief UiRectangle class source file.
///

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <UiRectangle.h>

using Plat4m::UiRectangle;

/*------------------------------------------------------------------------------
 * Public constructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
UiRectangle::UiRectangle(UiPrinter& printer,
                         UiView* view,
                         UiItem* parent,
                         const int width,
                         const int height) :
    UiItem(printer, view, parent),
    myLineWidth(1),
    myFill(FILL_NONE),
    myLineColor(0xFFFF),
    myFillColor(0x0000)
{
    setWidthHeight(width, height);
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
int UiRectangle::getLineWidth() const
{
    return myLineWidth;
}

//------------------------------------------------------------------------------
void UiRectangle::setLineWidth(const int lineWidth)
{
    if (lineWidth != myLineWidth)
    {
        myLineWidth = lineWidth;
        update();
    }
}

//------------------------------------------------------------------------------
UiRectangle::Fill UiRectangle::getFill() const
{
    return myFill;
}

//------------------------------------------------------------------------------
void UiRectangle::setFill(const Fill fill)
{
    myFill = fill;
}

//------------------------------------------------------------------------------
uint16_t UiRectangle::getLineColor() const
{
    return myLineColor;
}

//------------------------------------------------------------------------------
void UiRectangle::setLineColor(const uint16_t lineColor)
{
    myLineColor = lineColor;
}

//------------------------------------------------------------------------------
uint16_t UiRectangle::getFillColor() const
{
    return myFillColor;
}

//------------------------------------------------------------------------------
void UiRectangle::setFillColor(const uint16_t fillColor)
{
    myFillColor = fillColor;
}

/*------------------------------------------------------------------------------
 * Private implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void UiRectangle::driverPrint()
{
    getPrinter().printRectangle(*this);
}
