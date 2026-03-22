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
// Copyright (c) 2022-2024 Benjamin Minerd
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
/// @file PrinterComInterface.h
/// @author Ben Minerd
/// @date 12/3/2020
/// @brief PrinterComInterface class header file.
///

#ifndef PLAT4M_PRINTER_COM_INTERFACE_H
#define PLAT4M_PRINTER_COM_INTERFACE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/Printer.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/ComInterface.h>
#include <Plat4m_Core/ComInterfaceDeviceTemplate.h>
#include <Plat4m_Core/ByteArray.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <std::uint32_t TxBufferSize>
class PrinterComInterface : public Printer
{
public:
    
    //--------------------------------------------------------------------------
    // Public Types
    //--------------------------------------------------------------------------
    
    enum ErrorCode
    {
        ERROR_CODE_NONE
    };

    using Error = ErrorTemplate<ErrorCode>;

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    PrinterComInterface(ComInterface& comInterface) :
        Printer(),
        myComInterfaceDevice(comInterface)
    {
    }
    
    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~PrinterComInterface()
    {
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    ComInterface& getComInterface()
    {
        return (myComInterfaceDevice.getComInterface());
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    ComInterfaceDeviceTemplate<TxBufferSize, 1> myComInterfaceDevice;

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for Module
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual Module::Error driverSetEnabled(const bool enabled) override
    {
        myComInterfaceDevice.setEnabled(enabled);

        return Module::Error(Module::ERROR_CODE_NONE);
    }

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for Printer
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual Printer::Error driverPrint(const ByteArray& bytes,
                                       const bool waitUntilDone,
                                       const bool isError) override
    {
        myComInterfaceDevice.transmitBytes(bytes, waitUntilDone);

        return Printer::Error(Printer::ERROR_CODE_NONE);
    }
};

}; // namespace Plat4m

#endif // PLAT4M_PRINTER_COM_INTERFACE_H
