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
// Copyright (c) 2023 Benjamin Minerd
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
/// @file DataObjectBase.h
/// @author Ben Minerd
/// @date 5/30/2023
/// @brief DataObjectBase class header file.
///

#ifndef PLAT4M_DATA_OBJECT_BASE_H
#define PLAT4M_DATA_OBJECT_BASE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/Module.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class DataObjectBase : public Module
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum Access
    {
        ACCESS_READ_WRITE = 0,
        ACCESS_READ_ONLY_PUSH,
        ACCESS_READ_ONLY_PULL,
        ACCESS_WRITE_ONLY
    };

    enum ErrorCode
    {
        ERROR_CODE_NONE = 0,
        ERROR_CODE_DATA_OBJECT_NOT_INSTANTIATED,
        ERROR_CODE_DATA_OBJECT_TYPE_ID_MISMATCH,
        ERROR_CODE_READ_FAILED,
        ERROR_CODE_WRITE_FAILED
    };

    using Error = ErrorTemplate<ErrorCode>;

    using Id = std::uint32_t;

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~DataObjectBase();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    void setId(const Id id);

    Id getId() const;

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    DataObjectBase(const Id id);

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Id myId;
};

}; // namespace Plat4m

#endif // PLAT4M_DATA_OBJECT_BASE_H
