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

/*
 * buffer_interface.h
 *
 *  Created on: Mar 18, 2012
 *      Author: ben
 */

#ifndef BUFFER_INTERFACE_H_
#define BUFFER_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define BUFFER_SIZE (256u)

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

typedef struct
{
	uint8_t data[BUFFER_SIZE];
	uint8_t size;
	uint8_t writeIndex;
	uint8_t readIndex;
} buffer_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

extern void bufferInit(buffer_t* buffer);

extern bool bufferWrite(buffer_t* buffer, uint8_t data);

extern bool bufferRead(buffer_t* buffer, uint8_t* data);

extern bool bufferIsEmpty(buffer_t* buffer);

extern bool bufferIsFull(buffer_t* buffer);

#endif /* BUFFER_INTERFACE_H_ */
