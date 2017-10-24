/*------------------------------------------------------------------------------
 *                  __                           ___
 *                 || |             __          //  |
 *       _______   || |   _______  || |__      //   |    _____  ___
 *      ||  ___ \  || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
 *      || |  || | || | || |  || | || |      // /|| |   || |\\  /\\ \
 *      || |__|| | || | || |__|| | || |     // /_|| |_  || | || | || |
 *      ||  ____/  || |  \\____  | || |__  //_____   _| || | || | || |
 *      || |       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
 *      || |
 *      ||_|
 *
 * Copyright (c) 2013 Ben Minerd. All rights reserved.
 *
 * GNU Lesser General Public License Usage
 * This file may be used under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation and
 * appearing in the file LICENSE.LGPL included in the packaging of this file.
 * Please review the following information to ensure the GNU Lesser General
 * Public License version 2.1 requirements will be met:
 * http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *----------------------------------------------------------------------------*/

/**
 * @file com_interface.c
 * @author Ben Minerd
 * @date 1/9/13
 * @brief Communication interface layer.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <com_interface.h>
#include <system_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/
     
static const com_driver_map_t* driverMap = NULL_POINTER;

static unsigned int currentLinkCount = 0;

static com_link_t* idToLinkMap[COM_LINK_MAX_COUNT];
     
/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

static void rxCallback(com_link_t* link, const uint8_t data);

static void taskCode(void);
     
/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
system_task_callback_f* comInterfaceInit(void)
{
    return taskCode;
}

//------------------------------------------------------------------------------
com_error_e comInterfaceSetDriverMap(const com_driver_map_t* comDriverMap)
{
#ifdef PLAT4M_DEBUG
    
    if (IS_NULL_POINTER(comDriverMap)           ||
        IS_NULL_POINTER(comDriverMap->entries)  ||
        ((comDriverMap->size) == 0))
    {
        return COM_ERROR_PARAMETER_INVALID;
    }
    
    int i;
    
    for (i = 0; i < comDriverMap->size; i++)
    {
        if (IS_NULL_POINTER(comDriverMap->entries[i]->processData))
        {
            return COM_ERROR_PARAMETER_INVALID;
        }
    }
    
#endif // PLAT4M_DEBUG
    
    driverMap = comDriverMap;

    return COM_ERROR_NONE;
}

//------------------------------------------------------------------------------
com_error_e comLinkInit(com_link_t* link, const bool enable)
{
#ifdef PLAT4M_DEBUG
    
    if (IS_NULL_POINTER(link))
    {
        return COM_ERROR_PARAMETER_INVALID;
    }
    
#endif // PLAT4M_DEBUG
    
    com_link_info_t* info = (com_link_info_t*) (link->info);
    
    if (info->isInitialized)
    {
        return COM_ERROR_NONE;
    }
    
    info->isInitialized = true;
    info->isEnabled     = enable;
    
    MUTEX_INIT(&(info->mutex));
    
    info->currentDriverId   = COM_DRIVER_ID_NONE;
    info->txBufferIndex     = -1;
    info->rxBufferIndex     = -1;
    
    info->rxCallback = rxCallback;
    
    idToLinkMap[currentLinkCount++] = link;
    
    return COM_ERROR_NONE;
}

//------------------------------------------------------------------------------
com_error_e comLinkEnable(com_link_t* link, const bool enable)
{
#ifdef PLAT4M_DEBUG
    
    if (IS_NULL_POINTER(link))
    {
        return COM_ERROR_PARAMETER_INVALID;
    }
    
#endif // PLAT4M_DEBUG
    
    com_link_info_t* info = (com_link_info_t*) (link->info);
    
#ifdef PLAT4M_DEBUG
    
    if (!(info->isInitialized))
    {
        return COM_ERROR_LINK_NOT_INITIALIZED;
    }
    
#endif // PLAT4M_DEBUG
    
    if ((info->isEnabled) == enable)
    {
        return COM_ERROR_NONE;
    }
    
    info->isEnabled = enable;
    
    return COM_ERROR_NONE;
}

//------------------------------------------------------------------------------
com_error_e comLinkBufferData(com_link_t* link, const uint8_t data)
{
#ifdef PLAT4M_DEBUG
    
    if (IS_NULL_POINTER(link))
    {
        return COM_ERROR_PARAMETER_INVALID;
    }
    
#endif // PLAT4M_DEBUG
    
    com_link_info_t* info = (com_link_info_t*) (link->info);
    
#ifdef PLAT4M_DEBUG
    
    if (!(info->isInitialized))
    {
        return COM_ERROR_LINK_NOT_INITIALIZED;
    }
    
    if (!(info->isEnabled))
    {
        return COM_ERROR_LINK_NOT_ENABLED;
    }
    
#endif // PLAT4M_DEBUG
    
    if (info->rxBufferIndex < COM_LINK_RX_BUFFER_SIZE)
    {
        info->rxBuffer[info->rxBufferIndex++];
    }
    
    return COM_ERROR_NONE;
}

//------------------------------------------------------------------------------
com_error_e comDriverAddMessages(const id_t driverId,
                                 const com_msg_list_t* msgList)
{
    
    
    return COM_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void rxCallback(com_link_t* link, const uint8_t data)
{
//#ifdef PLAT4M_DEBUG
//    
//    if (IS_NULL_POINTER(handle))
//    {
//        return COM_ERROR_PARAMETER_INVALID;
//    }
//    
//#endif // PLAT4M_DEBUG
    
    com_link_info_t* info = (com_link_info_t*) (link->info);
    
//#ifdef PLAT4M_DEBUG
//    
//    if (!(info->isInitialized))
//    {
//        return COM_ERROR_LINK_NOT_INITIALIZED;
//    }
//    
//    if (!(info->isEnabled))
//    {
//        return COM_ERROR_LINK_NOT_ENABLED;
//    }
//    
//#endif // PLAT4M_DEBUG
    
    if (info->rxBufferIndex < COM_LINK_RX_BUFFER_SIZE)
    {
        info->rxBuffer[++(info->rxBufferIndex)] = data;
    }
    
//    return COM_ERROR_NONE;
}

#define TASK(taskName)              \
static void taskName ## Test(void); \
static void taskName(void)          \
{                                   \
    while (true)                    \
    {                               \
        taskName ## Test();         \
    }                               \
}                                   \
                                    \
static void taskName ## Test(void)  \


//------------------------------------------------------------------------------
//TASK(task)
static void taskCode(void)
{
    int i, j;
    
    for (i = 0; i < currentLinkCount; i++)
    {
        com_link_t* link = idToLinkMap[i];
        com_link_info_t* info = (com_link_info_t*) (link->info);
        
        if (info->rxBufferIndex > -1)
        {
            byte_array_t rxData =
            {
                .bytes  = info->rxBuffer,
                .size   = info->rxBufferIndex + 1
            };
            
            byte_array_t* txData = NULL_POINTER;
            
            for (j = 0; j < link->driverIdMap.size; j++)
            {
                id_t driverId = link->driverIdMap.entries[j];
                com_driver_t* driver = driverMap->entries[driverId];
                
                switch (driver->processData(&rxData, &txData))
                {
                    case COM_PARSE_STATUS_NOT_A_MESSAGE:
                    {
                        break;
                    }
                    case COM_PARSE_STATUS_MID_MESSAGE:
                    {
                        info->currentDriverId = driverId;
                        
                        break;
                    }
                    case COM_PARSE_STATUS_FOUND_MESSAGE:
                    {
                        if (!IS_NULL_POINTER(txData))
                        {
                            info->txCallback(info->component, txData);
                        }
                        
                        info->currentDriverId   = COM_DRIVER_ID_NONE;
                        info->rxBufferIndex     = -1;
                        
                        break;
                    }
                }
                
                if (info->currentDriverId != COM_DRIVER_ID_NONE)
                {
                    break;
                }
            }
            
            if (j == (link->driverIdMap.size))
            {
                info->rxBufferIndex = -1;
            }
        }
    }
}