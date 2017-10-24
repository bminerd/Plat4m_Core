// Plat4m

///
/// @file DisplaySTLCD.h
/// @author Ben Minerd
/// @date 4/23/2014
/// @brief DisplaySTLCD class source file.
///

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <DisplaySTLCD.h>
#include <SlaveDevice.h>
#include <System.h>

using Plat4m::DisplaySTLCD;
using Plat4m::Display;
using Plat4m::Module;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

static const unsigned int width  = 320;
static const unsigned int height = 240;

/*------------------------------------------------------------------------------
 * Public constructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
DisplaySTLCD::DisplaySTLCD(GpioPin& chipSelectGpioPin, Spi& spi) :
    Display(myFrame),
    myData(),
    myFrame(),
    mySpiDevice(&chipSelectGpioPin, spi)
{
    myFrame.data   = myData.getItems();
    myFrame.size   = myData.getMaxSize();
    myFrame.width  = width;
    myFrame.height = height;
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
bool DisplaySTLCD::setRegister(const uint8_t registerNumber,
                               const uint16_t value,
                               ByteArray& byteArray)
{
    return (byteArray.append(registerNumber) & byteArray.append(value));
}

//------------------------------------------------------------------------------
Display::Error DisplaySTLCD::writeRegister(const uint8_t registerNumber,
                                           const uint16_t value)
{
    // Set register index

    myData.clear();

    myData.append((uint8_t) 0x70);
    myData.append((uint8_t) 0x00);
    myData.append(registerNumber);

    SlaveDevice::Error error = mySpiDevice.tx(myData);

    if (error.getCode() != SlaveDevice::ERROR_CODE_NONE)
    {
        return Display::Error(Display::ERROR_CODE_COMMUNICATION_FAILED);
    }

    // Write to register

    myData.clear();

    myData.append((uint8_t) 0x72);
    myData.append((uint8_t) (value >> 8));
    myData.append((uint8_t) value);

    error = mySpiDevice.tx(myData);

    if (error.getCode() != SlaveDevice::ERROR_CODE_NONE)
    {
        return Display::Error(Display::ERROR_CODE_COMMUNICATION_FAILED);
    }

    return Display::Error(Display::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Display::Error DisplaySTLCD::setGraphicsMemory()
{
    myData.clear();

    myData.append((uint8_t) 0x70);
    myData.append((uint8_t) 0x00);
    myData.append((uint8_t) 0x22);

    SlaveDevice::Error error = mySpiDevice.tx(myData);

    if (error.getCode() != SlaveDevice::ERROR_CODE_NONE)
    {
        return Display::Error(Display::ERROR_CODE_COMMUNICATION_FAILED);
    }

    return Display::Error(Display::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Display::Error DisplaySTLCD::writeToGraphicsMemory(const uint16_t value)
{
    myData.clear();

    myData.append((uint8_t) 0x72);
    myData.append((uint8_t) (value >> 8));
    myData.append((uint8_t) value);

    SlaveDevice::Error error = mySpiDevice.tx(myData);

    if (error.getCode() != SlaveDevice::ERROR_CODE_NONE)
    {
        return Display::Error(Display::ERROR_CODE_COMMUNICATION_FAILED);
    }

    return Display::Error(Display::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Display::Error DisplaySTLCD::setGraphicsMemoryWindow(const unsigned int startX,
                                                     const unsigned int startY,
                                                     const unsigned int endX,
                                                     const unsigned int endY)
{
    Error error = writeRegister(0x50, startY);

    if (error.getCode() != ERROR_CODE_NONE)
    {
        return error;
    }

    // Assume subsequent writes are good
    writeRegister(0x51, endY);
    writeRegister(0x52, startX);
    writeRegister(0x53, endX);

    return Error(ERROR_CODE_NONE);
}

/*------------------------------------------------------------------------------
 * Private methods implemented from Module
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Module::Error DisplaySTLCD::driverEnable(const bool enable)
{
    if (enable)
    {
        mySpiDevice.enable(true);

        // Configuration based on ST eval code

        System::timeDelayMs(50); // TODO: Is this needed? (copied from example)

        // Start initial sequence

        Error error = writeRegister(0xE5, 0x8000);

        if (error.getCode() != Display::ERROR_CODE_NONE)
        {
            return Module::Error(Module::ERROR_CODE_ENABLE_FAILED);
        }

        // All subsequent writes are assumed good

        writeRegister(0x00, 0x0001);
        writeRegister(0x01, 0x0100);
        writeRegister(0x02, 0x0000);
//        writeRegister(0x03, 0x1038);
        writeRegister(0x03, 0x1238);
        writeRegister(0x04, 0x0000);
        writeRegister(0x08, 0x0202);
        writeRegister(0x09, 0x0000);
        writeRegister(0x0A, 0x0000);
        writeRegister(0x0B, 0x0000);
        writeRegister(0x0C, 0x0000);
        writeRegister(0x0D, 0x0000);
        writeRegister(0x0F, 0x0000);

        // Power on sequence

        writeRegister(0x10, 0x0000);
        writeRegister(0x11, 0x0000);
        writeRegister(0x12, 0x0000);
        writeRegister(0x13, 0x0000);

        System::timeDelayMs(200);

        writeRegister(0x10, 0x17B0);
        writeRegister(0x11, 0x0137);

        System::timeDelayMs(50);

        writeRegister(0x12, 0x0139);

        System::timeDelayMs(50);

        writeRegister(0x13, 0x1D00);
        writeRegister(0x29, 0x0013);

        System::timeDelayMs(50);

        writeRegister(0x20, 0x0000);
        writeRegister(0x21, 0x0000);

        // Adjust the gamma curve

        writeRegister(0x30, 0x0006);
        writeRegister(0x31, 0x0101);
        writeRegister(0x32, 0x0003);
        writeRegister(0x35, 0x0106);
        writeRegister(0x36, 0x0B02);
        writeRegister(0x37, 0x0302);
        writeRegister(0x38, 0x0707);
        writeRegister(0x39, 0x0007);
        writeRegister(0x3C, 0x0600);
        writeRegister(0x3D, 0x020B);

        // Set GRAM area

        writeRegister(0x50, 0x0000);
        writeRegister(0x51, 0x00EF);
        writeRegister(0x52, 0x0000);
        writeRegister(0x53, 0x013F);
        writeRegister(0x60, 0x2700);
        writeRegister(0x61, 0x0001);
        writeRegister(0x6A, 0x0000);

        // Partial display control

        writeRegister(0x80, 0x0000);
        writeRegister(0x81, 0x0000);
        writeRegister(0x82, 0x0000);
        writeRegister(0x83, 0x0000);
        writeRegister(0x84, 0x0000);
        writeRegister(0x85, 0x0000);

        // Panel control

        writeRegister(0x90, 0x0010);
        writeRegister(0x92, 0x0000);
        writeRegister(0x93, 0x0003);
        writeRegister(0x95, 0x0110);
        writeRegister(0x97, 0x0000);
        writeRegister(0x98, 0x0000);

        // Set GRAM write direction and BGR = 1

        driverClear();

        writeRegister(0x07, 0x0173);
    }
    else
    {
        Display::Error error = writeRegister(0x07, 0x0173);

        if (error.getCode() != Display::ERROR_CODE_NONE)
        {
            return Module::Error(Module::ERROR_CODE_ENABLE_FAILED);
        }

        mySpiDevice.enable(false);
    }

    return Module::Error(Module::ERROR_CODE_NONE);
}

/*------------------------------------------------------------------------------
 * Private methods implemented from Display
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Display::Frame& DisplaySTLCD::driverGetFrame()
{
    return myFrame;
}

//------------------------------------------------------------------------------
Display::Error DisplaySTLCD::driverConfigure(const Config& config)
{
    return Display::Error(Display::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Display::Error DisplaySTLCD::driverWriteFrame()
{
    return Display::Error(Display::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Display::Error DisplaySTLCD::driverClear()
{
    writeRegister(0x20, 0);
    writeRegister(0x21, 0);
    setGraphicsMemory();

    myData.clear();
    myData.setValue(0x00);

    myData[0] = ((uint8_t) 0x72);

    for (int i = 0; i < getHeight(); i++)
    {
        SlaveDevice::Error error = mySpiDevice.tx(myData);

        if (error.getCode() != SlaveDevice::ERROR_CODE_NONE)
        {
            return Display::Error(Display::ERROR_CODE_COMMUNICATION_FAILED);
        }
    }

    return Display::Error(Display::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Display::Error DisplaySTLCD::driverSetBrightnessPercent(
                                                  const float brightnessPercent)
{


    return Display::Error(Display::ERROR_CODE_NONE);
}
