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
 * @file DacIc.h
 * @author Ben Minerd
 * @date 4/3/2013
 * @brief DacIc class.
 */

#ifndef _DAC_IC_H_
#define _DAC_IC_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class DacIc
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Enumeration of DAC IC errors.
     */
    enum Error
    {
        ERROR_NONE,
        ERROR_PARAMETER_INVALID,
        ERROR_NOT_ENABLED,
        ERROR_COMMUNICATION
    };
    
    /*--------------------------------------------------------------------------
     * Public virtual destructors
     *------------------------------------------------------------------------*/
    
    virtual ~DacIc();
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Enables or disables this DAC IC.
     * @param enable Flag that indicates if this DAC IC should be enabled or
     * disabled.
     * @return DAC IC error.
     */
    Error enable(const bool enable);
    
    /**
     * @brief Checks to see if this DAC IC is enabled or disabled.
     * @param isEnabled Flag that indicates if the DAC IC is enabled or
     * disabled.
     * @return DAC IC error.
     */
    Error isEnabled(bool& isEnabled);
    
    Error registerRead(const id_t registerId, uint16_t& value);
    
    Error registerWrite(const id_t registerId, const uint16_t value);
    
    Error registerReadBit(const id_t registerId,
                          const unsigned int bit,
                          Plat4m::BitValue& bitValue);
    
    Error registerWriteBit(const id_t registerId,
                           const unsigned int bit,
                           const Plat4m::BitValue bitValue);
    
protected:
    
    /*--------------------------------------------------------------------------
     * Protected constructors
     *------------------------------------------------------------------------*/
    
    DacIc();
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    bool myIsEnabled;
    
    /*--------------------------------------------------------------------------
     * Private virtual methods
     *------------------------------------------------------------------------*/
        
    virtual Error driverEnable(const bool enable) = 0;
    
    virtual Error driverRegisterRead(const id_t registerId,
                                     uint16_t& value) = 0;
    
    virtual Error driverRegisterWrite(const id_t registerId,
                                      const uint16_t value) = 0;
    
    virtual Error driverRegisterReadBit(const id_t registerId,
                                        const unsigned int bit,
                                        Plat4m::BitValue& bitValue) = 0;
    
    virtual Error driverRegisterWriteBit(const id_t registerId,
                                         const unsigned int bit,
                                         const Plat4m::BitValue bitValue) = 0;
};

#endif // _DAC_IC_H_