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
// Copyright (c) 2017 Benjamin Minerd
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
/// @file ComInterfaceDeviceTemplate.h
/// @author Ben Minerd
/// @date 4/12/2017
/// @brief ComInterfaceDeviceTemplate class header file.
///

#ifndef PLAT4M_COM_INTERFACE_DEVICE_TEMPLATE_H
#define PLAT4M_COM_INTERFACE_DEVICE_TEMPLATE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

#include <Plat4m_Core/ComInterfaceDevice.h>
#include <Plat4m_Core/BufferN.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <uint32_t TxBufferSize, uint32_t RxBufferSize>
class ComInterfaceDeviceTemplate : public ComInterfaceDevice
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    ComInterfaceDeviceTemplate() :
        ComInterfaceDevice(myTxBuffer, myRxBuffer),
        myTxBuffer(),
        myRxBuffer()
    {
    }

    //--------------------------------------------------------------------------
    ComInterfaceDeviceTemplate(ComInterface& comInterface) :
        ComInterfaceDevice(myTxBuffer, myRxBuffer, comInterface),
        myTxBuffer(),
        myRxBuffer()
    {
    }

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~ComInterfaceDeviceTemplate()
    {
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    BufferN<uint8_t, TxBufferSize> myTxBuffer;

    BufferN<uint8_t, RxBufferSize> myRxBuffer;
};

}; // namespace Plat4m

#endif // PLAT4M_COM_INTERFACE_DEVICE_TEMPLATE_H
