// Plat4m

///
/// @file DisplaySTLCD.h
/// @author Ben Minerd
/// @date 4/23/2014
/// @brief DisplaySTLCD class header file.
///

#ifndef DISPLAY_ST_LCD_H
#define DISPLAY_ST_LCD_H

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <Display.h>
#include <GpioPin.h>
#include <Spi.h>
#include <Module.h>
#include <SpiDevice.h>
#include <ByteArrayN.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

namespace Plat4m
{

class DisplaySTLCD : public Display
{
public:
    
    /*--------------------------------------------------------------------------
     * Public constructors
     *------------------------------------------------------------------------*/
    
    DisplaySTLCD(GpioPin& chipSelectGpioPin, Spi& spi);
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/

    bool setRegister(const uint8_t registerNumber,
                     const uint16_t value,
                     ByteArray& byteArray);

    Error writeRegister(const uint8_t registerNumber, const uint16_t value);

    Error setGraphicsMemory();

    Error writeToGraphicsMemory(const uint16_t value);

    Error setGraphicsMemoryWindow(const unsigned int startX,
                                  const unsigned int startY,
                                  const unsigned int endX,
                                  const unsigned int endY);

private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    ByteArrayN<641> myData;
    
    Frame myFrame;

    SpiDevice mySpiDevice;

    /*--------------------------------------------------------------------------
     * Private methods implemented from Module
     *------------------------------------------------------------------------*/

    Module::Error driverEnable(const bool enable);
    
    /*--------------------------------------------------------------------------
     * Private methods implemented from Display
     *------------------------------------------------------------------------*/
    
    Frame& driverGetFrame();
    
    Error driverConfigure(const Config& config);
    
    Error driverWriteFrame();
    
    Error driverClear();
    
    Error driverSetBrightnessPercent(const float brightnessPercent);
};

}; // namespace Plat4m

#endif // DISPLAY_ST_LCD_H
