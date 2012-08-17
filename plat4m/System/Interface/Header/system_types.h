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
 * Copyright (c) 2012 Ben Minerd. All rights reserved.
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
 * @file system_types.h
 * @author Ben Minerd
 * @date 7/30/12
 * @brief
 */

#ifndef _SYSTEM_TYPES_H_
#define _SYSTEM_TYPES_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define ARRAY_SIZE(array, type) sizeof(array) / sizeof(type)

#define DRIVER_ADD(array, driver, type) memcpy((uint8_t*) &array[driver.id], \
                                               (uint8_t*) &driver, \
                                               sizeof(type))

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 *
 */
typedef void procedure_f(void);

/**
 *
 */
typedef void set_enabled_f(bool enabled);

#endif // _SYSTEM_TYPES_H_
