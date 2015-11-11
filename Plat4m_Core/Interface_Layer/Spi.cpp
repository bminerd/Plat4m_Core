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
 * @file Spi.cpp
 * @author Ben Minerd
 * @date 4/4/13
 * @brief Spi class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Spi.h>
#include <System.h>

using Plat4m::Spi;
using Plat4m::Module;

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Spi::Error Spi::configure(const Config& config)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    Error error = driverConfigure(config);
    
    if (error.getCode() == ERROR_CODE_NONE)
    {
        myConfig = config;
    }

    return error;
}

//------------------------------------------------------------------------------
Spi::Error Spi::masterTransfer(const TransferMode transferMode,
                               GpioPin* chipSelectGpioPin,
                               const ByteArray& txByteArray,
                               ByteArray& rxByteArray,
                               const uint32_t timeoutMs)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }
    
    Transfer transfer;
    transfer.transferMode       = transferMode;
    transfer.chipSelectGpioPin  = chipSelectGpioPin;
    transfer.txCount            = txByteArray.getSize();
    transfer.rxCount            = rxByteArray.getMaxSize();
    transfer.error.setCode(ERROR_CODE_NONE);
    
    for (int i = 0; i < transfer.txCount; i++)
    {
        if (!(transfer.txBuffer.write(txByteArray[i])))
        {
            return Error(ERROR_CODE_TX_BUFFER_FULL);
        }
    }
    
    if (!(myTransferBuffer.write(transfer)))
    {
        return Error(ERROR_CODE_TX_BUFFER_FULL);
    }
    
    myState = STATE_BUSY;

    // Chip select
    if (IS_VALID_POINTER(transfer.chipSelectGpioPin))
    {
        chipSelectGpioPin->setLevel(GpioPin::LEVEL_LOW);
    }

    // Enable Tx and Rx interrupts to start transfer
    
    if (transfer.rxCount != 0)
    {
        transfer.error = driverInterruptEnable(INTERRUPT_RX, true);
    }
    
    if (transfer.txCount != 0)
    {
        transfer.error = driverInterruptEnable(INTERRUPT_TX, true);
    }
    
    if (transferMode == TRANSFER_MODE_TX)
    {
        // Return immediately and let the SPI interface continue asynchronously
        return transfer.error;
    }
    
    uint32_t timeoutTimeMs = System::timeGetMs() + timeoutMs;
    transfer.error.setCode(ERROR_CODE_NONE); // Does this make sense?

    while (myState != STATE_IDLE)
    {
        if (System::timeCheckMs(timeoutTimeMs))
        {
            transfer.error.setCode(ERROR_CODE_TIMEOUT);
            transfer.error = driverInterruptEnable(INTERRUPT_TX, false);
            transfer.error = driverInterruptEnable(INTERRUPT_RX, false);

            break;
        }
    }
    
    myTransferBuffer.read(transfer);

    if (transfer.error.getCode() == ERROR_CODE_NONE)
    {
        int size = rxByteArray.getSize();
        
        for (int i = 0; i < size; i++)
        {
            if (!(transfer.rxBuffer.read(rxByteArray[i])))
            {
                // Error?
            }
        }
    }

    return transfer.error;
}

//------------------------------------------------------------------------------
void Spi::interruptHandler(const Interrupt interrupt)
{
    Transfer* transfer;

    myTransferBuffer.peek(transfer);

    if (IS_NULL_POINTER(transfer))
    {
        return;
    }
    
    // Putting this check here implies that an additional interrupt is required
    // to stop the transfer
    //
    // TODO Verify that this is optimal, especially for various architectures
    if ((transfer->txBuffer.isEmpty()) &&
        (((transfer->rxCount) == 0) ||
         ((transfer->rxCount) == transfer->rxBuffer.count())))
    {
        transfer->error = driverInterruptEnable(INTERRUPT_TX, false);
        transfer->error = driverInterruptEnable(INTERRUPT_RX, false);
        
        if (IS_VALID_POINTER(transfer->chipSelectGpioPin))
        {
            transfer->chipSelectGpioPin->setLevel(GpioPin::LEVEL_HIGH);
        }
        
        myState = STATE_IDLE;
        
        return;
    }

    // TODO Change to support 16bit data
    // TODO Verify this scheme works!
    uint8_t byte = 0;
    bool txIsEmpty, rxIsEmpty;

    txIsEmpty = transfer->txBuffer.isEmpty();
    rxIsEmpty = transfer->rxBuffer.isEmpty();

    switch (interrupt)
    {
        case INTERRUPT_TX:
        {
            switch (transfer->transferMode)
            {
                case TRANSFER_MODE_TX_RX_SEQUENTIAL: // Fall through
                case TRANSFER_MODE_TX_RX_SEQUENTIAL_MAILBOX:
                {
                    if (transfer->txBuffer.read(byte))
                    {
                        transfer->error = driverTx(byte);
                    }
                    else
                    {
                        transfer->error = driverTx(0);
                    }

                    break;
                }
                case TRANSFER_MODE_TX:                  // Fall through
                case TRANSFER_MODE_TX_WAIT:             // Fall through
                case TRANSFER_MODE_TX_RX_CONCURRENT:    // Fall through
                case TRANSFER_MODE_TX_RX_CONCURRENT_MAILBOX:
                {
                    if (transfer->txBuffer.read(byte))
                    {
                        transfer->error = driverTx(byte);
                    }

                    break;
                }
                default:
                {
                    break; // Does this make sense?
                }
            }

            break;
        }
        case INTERRUPT_RX:
        {
            bool isLastByte = (transfer->rxCount + 1) ==
                              transfer->rxBuffer.count();
            transfer->error = driverRx(byte, isLastByte);

            switch (transfer->transferMode)
            {
                case TRANSFER_MODE_TX_RX_SEQUENTIAL: // Fall through
                case TRANSFER_MODE_TX_RX_SEQUENTIAL_MAILBOX:
                {
                    if (transfer->txBuffer.isEmpty())
                    {
                        if (!(transfer->rxBuffer.write(byte)))
                        {
                        }
                    }

                    break;
                }
                case TRANSFER_MODE_TX_RX_CONCURRENT: // Fall through
                case TRANSFER_MODE_TX_RX_CONCURRENT_MAILBOX:
                {
                    if (!(transfer->rxBuffer.write(byte)))
                    {
                        // Error?
                    }

                    break;
                }
            }

            break;
        }
    }
}

/*------------------------------------------------------------------------------
 * Protected constructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Spi::Spi(const TransmissionMode transmissionMode) :
    Module(),
    myTransmissionMode(transmissionMode),
    myState(STATE_IDLE)
{
}

/*------------------------------------------------------------------------------
 * Protected virtual destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Spi::~Spi()
{
}
