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
 * @file DisplayOsd9616.cpp
 * @author Ben Minerd
 * @date 4/11/2013
 * @brief DisplayOsd9616 class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <DisplayOsd9616.h>

#include <System.h>

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

static const uint8_t i2cAddress     = 0x3C;
static const uint8_t commandFlag    = 0x80;
static const uint8_t dataFlag       = 0x40;
static const unsigned int width     = 96;
static const unsigned int height    = 16;

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
DisplayOsd9616::DisplayOsd9616(I2c& i2c,
                               GpioPin& rstNGpioPin,
                               GpioPin& vccEnGpioPin,
                               GpioPin& pwrEnGpioPin) :
    Display(myFrame),
    myI2cDevice(i2cAddress, i2c),
    myRstNGpioPin(rstNGpioPin),
    myVccEnGpioPin(vccEnGpioPin),
    myPwrEnGpioPin(pwrEnGpioPin)
{
    myFrame.data = &(myData[1]);
    myFrame.size = myData.getMaxSize() - 1;
    myFrame.width = width;
    myFrame.height = height;
}

/*------------------------------------------------------------------------------
 * Private methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Display::Frame& DisplayOsd9616::driverGetFrame()
{
    return myFrame;
}

//------------------------------------------------------------------------------
Display::Error DisplayOsd9616::driverEnable(const bool enable)
{
    myData.clear();
    
    if (enable)
    {
        myRstNGpioPin.enable(true);
        myVccEnGpioPin.enable(true);
        myPwrEnGpioPin.enable(true);
        
        GpioPin::Config gpioConfig;
        gpioConfig.mode     = GpioPin::MODE_DIGITAL_OUTPUT;
        gpioConfig.resistor = GpioPin::RESISTOR_NONE;
        
        myRstNGpioPin.configure(gpioConfig);
        myVccEnGpioPin.configure(gpioConfig);
        myPwrEnGpioPin.configure(gpioConfig);

        // Enable V_DD
        myPwrEnGpioPin.setLevel(GpioPin::LEVEL_HIGH);
        // Wait for V_DD to stabilize
        System::timeDelayMs(10);
        // Pull RST low
        myRstNGpioPin.setLevel(GpioPin::LEVEL_LOW);
        System::timeDelayMs(1);
        // Push RST high
        myRstNGpioPin.setLevel(GpioPin::LEVEL_HIGH);
        System::timeDelayMs(1);
        
        myVccEnGpioPin.setLevel(GpioPin::LEVEL_HIGH);
        System::timeDelayMs(100);

        // Set display clock divide ratio / oscillator frequency to ???
        myData.append(commandFlag);
        myData.append(0xD5);
        myData.append(commandFlag);
        myData.append(0xF0);
        // Set multiplex ratio to ???
        myData.append(commandFlag);
        myData.append(0xA8);
        myData.append(commandFlag);
        myData.append(0x0F);
        // Set display offset to ???
        myData.append(commandFlag);
        myData.append(0xD3);
        myData.append(commandFlag);
        myData.append(0x1F);
        // Set display start line to ???
        myData.append(commandFlag);
        myData.append(0x40);
        // Set segment re-map to???
        myData.append(commandFlag);
        myData.append(0xA1);
        // Set COM scan direction to ???
        myData.append(commandFlag);
        myData.append(0xC0);
        // Set COM pins hardware configuration to ???
        myData.append(commandFlag);
        myData.append(0xDA);
        myData.append(commandFlag);
        myData.append(0x12);
        // Set contrast control to ???
        myData.append(commandFlag);
        myData.append(0x81);
        myData.append(commandFlag);
        myData.append(0xFF);
        // Set pre-charge period to ???
        myData.append(commandFlag);
        myData.append(0xD9);
        myData.append(commandFlag);
        myData.append(0x22);
        // Set VCOMH level to ???
        myData.append(commandFlag);
        myData.append(0xDB);
        myData.append(commandFlag);
        myData.append(0x30);
        // Set memory addressing mode to ???
        myData.append(commandFlag);
        myData.append(0x20);
        myData.append(commandFlag);
        myData.append(0x01);
        // Set column start and end address
        myData.append(commandFlag);
        myData.append(0x21);
        myData.append(commandFlag);
        myData.append((uint8_t) 0);
        myData.append(commandFlag);
        myData.append(95);
        // Set page start and end address
        myData.append(commandFlag);
        myData.append(0x22);
        myData.append(commandFlag);
        myData.append((uint8_t) 0);
        myData.append(commandFlag);
        myData.append(0x01);
        // Set entire display on/off
        myData.append(commandFlag);
        myData.append(0xA4);
        // Set display normal
        myData.append(commandFlag);
        myData.append(0xA6);

        if (myI2cDevice.tx(myData) != SlaveDevice::ERROR_NONE)
        {
            return ERROR_COMMUNICATION;
        }

        Error displayError = driverClear();

        if (displayError != ERROR_NONE)
        {
            return displayError;
        }

        myData.clear();

        myData.append(commandFlag);
        myData.append(0xAF);

        if (myI2cDevice.tx(myData) != SlaveDevice::ERROR_NONE)
        {
            return ERROR_COMMUNICATION;
        }

        System::timeDelayMs(100);
    }
    else
    {
        myData.append(commandFlag);
        myData.append(0xAE);

        if (myI2cDevice.tx(myData) != SlaveDevice::ERROR_NONE)
        {
            return ERROR_COMMUNICATION;
        }

        // Disable V_CC
        myVccEnGpioPin.setLevel(GpioPin::LEVEL_LOW);
        System::timeDelayMs(100);
        // Disable V_DD
        myPwrEnGpioPin.setLevel(GpioPin::LEVEL_LOW);
        
        myRstNGpioPin.enable(false);
        myVccEnGpioPin.enable(false);
        myPwrEnGpioPin.enable(false);
    }

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Display::Error DisplayOsd9616::driverConfigure(const Config& config)
{
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Display::Error DisplayOsd9616::driverWriteFrame()
{
    myData.clear();
    myData.append(dataFlag);
    myData.setSize(myData.getMaxSize());
    
    if (myI2cDevice.tx(myData, false) != SlaveDevice::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Display::Error DisplayOsd9616::driverClear()
{
    // Reset memory pointer to top corner of display
    myData.clear();
    myData.append(commandFlag);
    myData.append(0x21);
    myData.append(commandFlag);
    myData.append((uint8_t) 0);
    myData.append(commandFlag);
    myData.append(95);

    if (myI2cDevice.tx(myData) != SlaveDevice::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }

    myData.clear();
    myData.setValue(0);
    
    myData[0] = dataFlag;

    if (myI2cDevice.tx(myData) != SlaveDevice::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Display::Error DisplayOsd9616::driverSetBrightnessPercent(
                                                  const float brightnessPercent)
{
    myData.clear();
    myData.append(commandFlag);
    myData.append(0x81);
    myData.append(commandFlag);
    myData.append((uint8_t) round(255.0f *(brightnessPercent / 100.0f)));

    if (myI2cDevice.tx(myData) != SlaveDevice::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }

    return ERROR_NONE;
}
