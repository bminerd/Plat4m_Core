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
 * @file UiLine.cpp
 * @author Ben Minerd
 * @date 11/6/2013
 * @brief UiLine class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <UiLine.h>

using Plat4m::UiLine;

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
UiLine::UiLine(UiPrinter& printer,
               UiView* view,
               UiItem* parent,
               const int startPointX,
               const int startPointY,
               const int endPointX,
               const int endPointY,
               const int thickness) :
    UiItem(printer, view, parent),
    myStartPoint(0, 0),
    myEndPoint(0, 0),
    myThickness(thickness)
{
    setStartPoint(startPointX, startPointY);
    setEndPoint(endPointX, endPointY);
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
const Point<int>& UiLine::getStartPoint() const
{
    return myStartPoint;
}
     
//------------------------------------------------------------------------------
void UiLine::setStartPoint(const int x, const int y)
{
    myStartPoint.setXY(x, y);
    setXY(x, y);
}

//------------------------------------------------------------------------------
void UiLine::setStartPoint(const Point<int>& point)
{
    myStartPoint = point;
    setXY(myStartPoint.getX(), myEndPoint.getY());
}

//------------------------------------------------------------------------------
const Point<int>& UiLine::getEndPoint() const
{
    return myEndPoint;
}

//------------------------------------------------------------------------------
void UiLine::setEndPoint(const int x, const int y)
{
    myEndPoint.setXY(x, y);
    setWidthHeight(x - myStartPoint.getX() + myThickness,
                   y - myStartPoint.getY() + 1,
                   false);
}

//------------------------------------------------------------------------------
void UiLine::setEndPoint(const Point<int>& point)
{
    myEndPoint = point;
    setWidthHeight(myEndPoint.getX() - myStartPoint.getX() + myThickness,
                   myEndPoint.getY() - myStartPoint.getY() + 1,
                   false);
}

//------------------------------------------------------------------------------
int UiLine::getThickness() const
{
    return myThickness;
}

//------------------------------------------------------------------------------
void UiLine::setThickness(const int thickness)
{
    myThickness = thickness;
    update();
}

/*------------------------------------------------------------------------------
 * Private implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void UiLine::driverPrint()
{
    getPrinter().printLine(*this);
}
