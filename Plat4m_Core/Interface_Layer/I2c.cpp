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
 * @file i2c_interface.c
 * @author Ben Minerd
 * @date 2/25/12
 * @brief I2C interface layer.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <I2c.h>
#include <System.h>

/*------------------------------------------------------------------------------
 * Public virtual destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
I2c::~I2c()
{
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
I2c::Error I2c::enable(const bool enable)
{
    if (enable == myIsEnabled)
    {
        return ERROR_NONE;
    }

    Error error;

    error = driverEnable(enable);

    if (error != ERROR_NONE)
    {
        return error;
    }

    if (error == ERROR_NONE)
    {
        myIsEnabled = enable;
    }

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
I2c::Error I2c::isEnabled(bool& isEnabled)
{
    isEnabled = myIsEnabled;

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
I2c::Error I2c::configure(const Config& config)
{
    Error error = driverConfigure(config);

    if (error == ERROR_NONE)
    {
        myConfig = config;
    }

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
I2c::Error I2c::reset()
{
    Error error;

    error = driverReset();

    // Error checking?

    myIsEnabled = false;
    error = enable(true);

    // Error checking?

    error = configure(myConfig);

    return error;
}

//------------------------------------------------------------------------------
I2c::Error I2c::masterTransfer(const TransferMode transferMode,
                               const uint8_t address,
                               const ByteArray& txByteArray,
                               ByteArray& rxByteArray,
                               const uint32_t timeoutMs)
{
    if (!myIsEnabled)
    {
        return ERROR_NOT_ENABLED;
    }

    Transfer transfer;
    transfer.transferMode   = transferMode;
    transfer.address        = address;
    transfer.txCount        = txByteArray.getSize();
    transfer.rxCount        = rxByteArray.getMaxSize();
    transfer.error          = ERROR_NONE;

    for (int i = 0; i < transfer.txCount; i++)
    {
        if (!(transfer.txBuffer.write(txByteArray[i])))
        {
            return ERROR_TX_BUFFER_FULL;
        }
    }

    if (!(myTransferBuffer.write(transfer)))
    {
        return ERROR_TX_BUFFER_FULL;
    }

    // Start transfer
    transfer.error = driverMasterStart();

    myState = STATE_BUSY;

    transfer.error = driverInterruptEnable(INTERRUPT_NACK_RECEIVED, true);
    transfer.error = driverInterruptEnable(INTERRUPT_MASTER_START_TRANSMITTED,
                                           true);

    if (transferMode == TRANSFER_MODE_TX)
    {
        // Return immediately and let the I2C interface continue asynchronously
        return transfer.error;
    }

    
    uint32_t timeoutTimeMs = System::timeGetMs() + timeoutMs;

    while (myState != STATE_IDLE)
    {
        if (System::timeCheckMs(timeoutTimeMs))
        {
            transfer.error = ERROR_TIMEOUT;
            reset();

            break;
        }

        System::timeDelayMs(1); // Does this make sense?
    }

    switch (myState)
    {
        case STATE_ERROR:
        {
            reset();

            break;
        }
        default:
        {
            break;
        }
    }

    myTransferBuffer.read(transfer);
    myLastTransfer = transfer;

    if (transfer.error == ERROR_NONE)
    {
        int size = transfer.rxBuffer.count();

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
void I2c::interruptHandler(const Interrupt interrupt)
{
    Transfer* transfer;
    myTransferBuffer.peek(transfer);

    if (IS_NULL_POINTER(transfer))
    {
        return;
    }

    unsigned int txBufferCount = 0;
    uint8_t byte = 0;

    txBufferCount = transfer->txBuffer.count();

    switch (interrupt)
    {
        case INTERRUPT_MASTER_START_TRANSMITTED:
        {
            if (txBufferCount > 0)
            {
                driverInterruptEnable(INTERRUPT_BYTE_TRANSMITTING, true);
                driverMasterTxAddress(transfer->address, MASTER_MODE_TX);
            }
            else if (transfer->rxCount > 0)
            {
                if (transfer->rxCount > 0)
                {
                    driverRx(((transfer->rxCount) == 1));
                }

                driverInterruptEnable(INTERRUPT_BYTE_RECEIVED, true);
                driverMasterTxAddress(transfer->address, MASTER_MODE_RX);
            }

            break;
        }
        case INTERRUPT_MASTER_TRANSMITTER_SELECTED:
        {
            if (txBufferCount > 0)
            {
                if (transfer->txBuffer.read(byte))
                {
                    driverTx(byte);
                }
            }

            break;
        }
        case INTERRUPT_MASTER_RECEIVER_SELECTED:
        {
            if (transfer->rxCount > 0)
            {
                driverRx(((transfer->rxCount) == 1));
            }

            break;
        }
        case INTERRUPT_BYTE_TRANSMITTING:
        {
            if (txBufferCount > 0)
            {
                if (transfer->txBuffer.read(byte))
                {
                    if ((txBufferCount == 1) && (transfer->rxCount == 0))
                    {
                        driverInterruptEnable(INTERRUPT_BYTE_TRANSMITTING,
                                              false);
                    }

                    driverTx(byte);
                }
            }
            else
            {
                if ((transfer->rxCount) > 0)
                {
                    // Restart condition
                    driverMasterStart();
                    driverInterruptEnable(INTERRUPT_BYTE_TRANSMITTING, false);
                }
                else
                {
                    driverInterruptEnable(INTERRUPT_BYTE_TRANSMITTING, false);
                    driverInterruptEnable(INTERRUPT_BYTE_TRANSMITTED, false);
                    driverMasterStop();
                    myState = STATE_IDLE;

                    if (transfer->transferMode == TRANSFER_MODE_TX)
                    {
                        Transfer empty;
                        myTransferBuffer.read(empty);
                        myLastTransfer = empty;
                    }
                }
            }

            break;
        }
        case INTERRUPT_BYTE_TRANSMITTED:
        {
            if (txBufferCount > 0)
            {
                if (transfer->txBuffer.read(byte))
                {
                    if (txBufferCount == 1)
                    {
                        driverInterruptEnable(INTERRUPT_BYTE_TRANSMITTING,
                                              false);
                    }

                    driverTx(byte);
                }
            }
            else
            {
                if ((transfer->rxCount) > 0)
                {
                    // Restart condition
                    driverMasterStart();
                }
                else
                {
                    driverInterruptEnable(INTERRUPT_BYTE_TRANSMITTING, false);
                    driverInterruptEnable(INTERRUPT_BYTE_TRANSMITTED, false);
                    driverMasterStop();
                    myState = STATE_IDLE;

                    if (transfer->transferMode == TRANSFER_MODE_TX)
                    {
                        Transfer empty;
                        myTransferBuffer.read(empty);
                        myLastTransfer = empty;
                    }
                }
            }

            break;
        }
        case INTERRUPT_BYTE_RECEIVED:
        {
            unsigned int rxBufferCount = transfer->rxBuffer.count();

            if (rxBufferCount < (transfer->rxCount))
            {
                driverRx(((rxBufferCount + 1) == (transfer->rxCount)));

                if (rxBufferCount == (transfer->rxCount - 1))
                {
                    driverInterruptEnable(INTERRUPT_MASTER_START_TRANSMITTED,
                                          false);
                    driverInterruptEnable(INTERRUPT_BYTE_RECEIVED, false);
                    driverInterruptEnable(INTERRUPT_BYTE_TRANSMITTED, false);
                    driverMasterStop();
                    myState = STATE_IDLE;
                }
            }

            driverRead(byte);

            if (!(transfer->rxBuffer.write(byte)))
            {
                // Error?
            }

            break;
        }
        case INTERRUPT_SLAVE_STOP_RECEIVED:
        {
            if ((transfer->txCount) > 0)
            {
                if (transfer->txBuffer.read(byte))
                {
                    driverTx(byte);
                }
                else
                {
                    driverMasterStop();
                }
            }
            else if ((transfer->rxCount) > 0)
            {

            }

            break;
        }
        case INTERRUPT_NACK_RECEIVED:
        {
            driverInterruptEnable(INTERRUPT_MASTER_START_TRANSMITTED, false);
            driverInterruptEnable(INTERRUPT_BYTE_TRANSMITTING, false);
            driverInterruptEnable(INTERRUPT_NACK_RECEIVED, false);
            
            myState = STATE_ERROR;

            break;
        }
        default:
        {
            break;
        }
    }
}

/*------------------------------------------------------------------------------
 * Protected constructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
I2c::I2c(const char* idString) :
    myIdString(idString),
    myIsEnabled(false),
    myState(STATE_IDLE)
{
}