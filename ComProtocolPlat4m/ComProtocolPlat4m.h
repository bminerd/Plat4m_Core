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
 * @file ComLink.h
 * @author Ben Minerd
 * @date 4/22/13
 * @brief ComLink class.
 */

#ifndef _COM_LINK_H_
#define _COM_LINK_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <plat4m_types.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class ComLink
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/

    /**
     * @brief Enumeration of communication errors.
     */
    typedef enum _com_error_e_
    {
        COM_ERROR_NONE,
        COM_ERROR_PARAMETER_INVALID,
        COM_ERROR_LINK_NOT_INITIALIZED,
        COM_ERROR_LINK_NOT_ENABLED
    } com_error_e;

    typedef enum _com_parse_status_e_
    {
        COM_PARSE_STATUS_NOT_A_MESSAGE,
        COM_PARSE_STATUS_MID_MESSAGE,
        COM_PARSE_STATUS_FOUND_MESSAGE
    } com_parse_status_e;
    
    /*--------------------------------------------------------------------------
     * Public constructors and destructors
     *------------------------------------------------------------------------*/
    
    com_error_e comInterfaceSetDriverMap(const com_driver_map_t* comDriverMap);

com_error_e comLinkInit(com_link_t* link, const bool enable);

com_error_e comLinkEnable(com_link_t* link, const bool enable);

com_error_e comDriverAddMsgModule(const id_t driverId,
                                  const id_t moduleId,
                                  const com_msg_list_t* msgList);

private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
}

#endif // _COM_LINK_H_