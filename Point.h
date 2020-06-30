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
// Copyright (c) 2020 Benjamin Minerd
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
/// @file Point.h
/// @author Ben Minerd
/// @date 11/6/13
/// @brief Point class.
///

#ifndef PLAT4M_POINT_H
#define PLAT4M_POINT_H

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename T>
class Point
{
public:
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    Point(const T x = 0, const T y = 0) :
        myX(x),
        myY(y)
    {
    }
    
    //--------------------------------------------------------------------------
    Point(const Point<T>& point) :
        myX(point.myX),
        myY(point.myY)
    {
    }
    
    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    T getX() const
    {
        return myX;
    }
    
    //--------------------------------------------------------------------------
    void setX(const T x)
    {
        myX = x;
    }
    
    //--------------------------------------------------------------------------
    T getY() const
    {
        return myY;
    }
    
    //--------------------------------------------------------------------------
    void setY(const T y)
    {
        myY = y;
    }
    
    //--------------------------------------------------------------------------
    void setXY(const T x, const T y)
    {
        myX = x;
        myY = y;
    }
    
    //--------------------------------------------------------------------------
    Point& operator=(const Point<T>& point)
    {
        myX = point.myX;
        myY = point.myY;
        
        return (*this);
    }
    
private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    T myX;
    T myY;
};

#endif // PLAT4M_POINT_H
