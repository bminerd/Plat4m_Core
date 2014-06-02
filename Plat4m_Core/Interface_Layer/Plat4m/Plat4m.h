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
 * @file Plat4m.h
 * @author Ben Minerd
 * @date 7/30/12
 * @brief Plat4m namespace.
 */

#ifndef _PLAT4M_H_
#define _PLAT4M_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Callback.h>
#include <CallbackFunction.h>
#include <CallbackParameterFunction.h>
#include <CallbackMethod.h>
#include <CallbackParameterMethod.h>

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/


/**
 * @brief Defines a more verbose null pointer.
 */
#define NULL_POINTER 0

#define IS_VALID_POINTER(pointer) pointer

/**
 * @brief Checks to see if the given pointer is null.
 * @param pointer Pointer to check if null.
 */
#define IS_NULL_POINTER(pointer) (!IS_VALID_POINTER(pointer))

/**
 * @brief Returns the number of elements of any given array.
 * @param array Array to get number of elements.
 */
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#define ARRAY_SIZE_2D(array) (sizeof(array) / sizeof(array[0][0]))

/**
 * @brief Initializes a byte array type with the given byte array.
 * @param array Array of bytes to put into the byte array type.
 */
#define BYTE_ARRAY(array) { .bytes = array, .size = ARRAY_SIZE(array) }

/**
 * @brief Initializes a byte array type with the given string.
 * @param string String to put into the byte array type.
 */
#define BYTE_ARRAY_STRING(string) { .bytes = string, .size = strlen(string) }

/**
 * @brief Initializes a data array type with the given array of bytes.
 * @param array Array of bytes to put into the data array type.
 */
#define DATA_ARRAY(array)                               \
                                                        \
    {                                                   \
        .data.bytes     = (uint8_t*) array,             \
        .data.size      = sizeof(array),                \
        .size           = ARRAY_SIZE(array),            \
        .elementSize    = sizeof(array[0])              \
    } // No semicolon

#define DATA_ARRAY_POINTER(name, type, array) \
    type* name = (type*) (array.data.bytes) // No semicolon

#define FOREVER while (true)

#define MHZ * 1000000

#define LOG_N(value, n) (log(value) / log(n))

#define LOG_2(value) LOG_N(value, 2)

#define BIT_SET(value, mask) value |= mask

#define BIT_CLEAR(value, mask) value &= ~mask

#define IS_BIT_SET(value, mask) ((value & mask) == mask)

#define IS_BIT_CLEAR(value, mask) (!IS_BIT_SET(value, mask))

/*------------------------------------------------------------------------------
 * Enums
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of bit values.
 */
typedef enum _bit_value_e_
{
    BIT_VALUE_0 = 0,
    BIT_VALUE_1
} bit_value_e;

/**
 * @brief Enumeration of setting value types.
 */
typedef enum _value_type_e_
{
    VALUE_TYPE_BOOLEAN,
    VALUE_TYPE_UINT,
    VALUE_TYPE_UINT8,
    VALUE_TYPE_UINT16,
    VALUE_TYPE_UINT32,
    VALUE_TYPE_INT,
    VALUE_TYPE_INT8,
    VALUE_TYPE_INT16,
    VALUE_TYPE_INT32,
    VALUE_TYPE_FLOAT32,
    VALUE_TYPE_STRING
} value_type_e;


/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief ID type.
 */
typedef unsigned int id_t;

/**
 * @brief Millisecond time type.
 */
typedef uint32_t time_ms_t;

/**
 * @brief Generic data type.
 */
typedef uint8_t generic_data_t;

/**
 * @brief Function type for a procedure.
 */
typedef void procedure_f(void);

/**
 * @brief Function type that sets a value enabled or disabled.
 * @param enabled Flag that indicates if the value should be enabled or
 * disabled.
 */
typedef void set_enabled_f(bool enabled);

/**
 * @brief Function type for a data callback.
 * @param data Byte to use.
 */
typedef void data_callback_f(const uint8_t data);

/**
 * @brief Byte array type.
 */
typedef struct _byte_array_t_
{
    uint8_t* bytes;
    unsigned int size;
} byte_array_t;

/**
 * @brief Data array type.
 */
typedef struct _data_array_t_
{
    byte_array_t data;
    unsigned int size;
    unsigned int elementSize;
} data_array_t;

/**
 * @brief Dimension data array type.
 */
typedef struct _dimension_data_array_t_
{
    data_array_t data;
    unsigned int width;
    unsigned int height;
} dimension_data_array_t;

/**
 * @brief Value type.
 */
typedef union _value_t_
{
    bool boolean;
    unsigned int intUnsigned;
    int intSigned;
    float float32;
    char* string;
} value_t;

typedef void handle_t;

typedef struct _handle_map_t_
{
    handle_t** entries;
    unsigned int size;
} handle_map_t;

typedef void layer_info_t;

/**
 * @brief Function type that handles received data from a UART.
 * @param byte Byte received.
 */
typedef void rx_callback_f(const id_t id, const uint8_t byte);

/*------------------------------------------------------------------------------
 * Namespaces
 *----------------------------------------------------------------------------*/

namespace Plat4m
{
    /*--------------------------------------------------------------------------
     * Enumerations
     *------------------------------------------------------------------------*/

    /**
     * @brief Enumeration of bit values.
     */
    enum BitValue
    {
        BIT_VALUE_0 = 0,
        BIT_VALUE_1 = 1
    };
    
    enum LogicLevel
    {
        LOGIC_LEVEL_LOW     = 0,
        LOGIC_LEVEL_HIGH    = 1
    };

    /**
     * @brief Enumeration of setting value types.
     */
    enum ValueType
    {
        VALUE_TYPE_BOOLEAN,
        VALUE_TYPE_UINT,
        VALUE_TYPE_UINT8,
        VALUE_TYPE_UINT16,
        VALUE_TYPE_UINT32,
        VALUE_TYPE_INT,
        VALUE_TYPE_INT8,
        VALUE_TYPE_INT16,
        VALUE_TYPE_INT32,
        VALUE_TYPE_FLOAT32,
        VALUE_TYPE_STRING
    };

    /*--------------------------------------------------------------------------
     * Typedefs
     *------------------------------------------------------------------------*/

    /**
     * @brief ID type.
     */
    typedef unsigned int Id;

    /**
     * @brief Millisecond time type.
     */
    typedef uint32_t TimeMs;

    /**
     * @brief Generic data type.
     */
    typedef uint8_t GenericData;

    /*--------------------------------------------------------------------------
     * Structures
     *------------------------------------------------------------------------*/

    /**
     * @brief Byte array type.
     */
    struct ByteArray
    {
        uint8_t* bytes;
        unsigned int size;
    };

    /**
     * @brief Data array type.
     */
    struct DataArray
    {
        byte_array_t data;
        unsigned int size;
        unsigned int elementSize;
    };

    /**
     * @brief Dimension data array type.
     */
    struct DimensionDataArray
    {
        data_array_t data;
        unsigned int width;
        unsigned int height;
    };

    /**
     * @brief Value type.
     */
    union Value
    {
        bool boolean;
        unsigned int intUnsigned;
        int intSigned;
        float float32;
        Id id;
        char* string;
    };

    /*--------------------------------------------------------------------------
     * Variables
     *------------------------------------------------------------------------*/

    static const char* version = "0.0.2";

    /*--------------------------------------------------------------------------
     * Functions
     *------------------------------------------------------------------------*/

    template <typename TReturn = void>
    Callback<TReturn>& createCallback(TReturn (*callback)())
    {
        return *(new CallbackFunction<TReturn>(callback));
    }

    template <typename TReturn = void, typename TParameter = void*>
    Callback<TReturn, TParameter>& createCallback(
                                                TReturn (*callback)(TParameter))
    {
        return *(new CallbackParameterFunction<TReturn, TParameter>(callback));
    }

    template <class TClass, typename TReturn = void>
    Callback<TReturn>& createCallback(TClass* object,
                                      TReturn (TClass::*callback)())
    {
        return *(new CallbackMethod<TClass, TReturn>(object, callback));
    }

    template <class TClass, typename TReturn = void, typename TParameter = void*>
    Callback<TReturn, TParameter>& createCallback(
                                        TClass* object,
                                        TReturn (TClass::*callback)(TParameter))
    {
        return *(new CallbackParameterMethod<TClass, TReturn, TParameter>(
                                                                     object,
                                                                     callback));
    }
};

#endif // _PLAT4M_H_
