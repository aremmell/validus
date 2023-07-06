/*
 * validuslibrary.h
 *
 * Author:    Ryan M. Lederman <lederman@gmail.com>
 * Copyright: Copyright (c) 2004-2023
 * Version:   1.0.1
 * License:   The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef _VALIDUS_LIBRARY_H_INCLUDED
#define _VALIDUS_LIBRARY_H_INCLUDED

#include "validus.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#if defined(_WIN32)
# define WIN32_LEAN_AND_MEAN
# define WINVER       0x0A00
# define _WIN32_WINNT 0x0A00
# include <windows.h>
# define __WIN__
#endif

/** The size, in octets that is used to read chunks of data from a file. */
#define VALIDUS_FILEBLOCKSIZE 8192

/** The maximum allowable size of a string, in octets. */
#define VALIDUS_MAX_STRING 2048

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////// function exports //////////////////////////////////

/**
 * @brief Hashes a string.
 *
 * @param   state  Pointer to a validus_state object which will contain the
 *                 results of the operation upon success.
 * @param   string Pointer to the string to hash. Will be read up until the first
 *                 null terminator, or VALIDUS_MAX_STRING, whichever comes first.
 * @returns bool   `true` if input parameters are valid, `false` otherwise.
 */
bool validus_hash_string(validus_state* state, const char* string);

/**
 * @brief Hashes a block of memory.
 *
 * @param   state Pointer to a validus_state object which will contain the
 *                results of the operation upon success.
 * @param   mem   Pointer to the area of memory to hash.
 * @param   len   Length of `mem` in octets.
 * @returns bool `true` if input parameters are valid, `false` otherwise.
 */
bool validus_hash_mem(validus_state* state, const void* mem, validus_word len);

/**
 * @brief Hashes a file.
 *
 * @note If a relative pathname is supplied, it shall be treated as relative
 * to the current working directory. This may or may not be the same directory
 * that this program resides in.
 *
 * @note The preprocessor macro VALIDUS_FILEBLOCKSIZE may be modified at compile
 * time to suit your needs if the default value (8 KiB) is insufficient.
 *
 * @param   state Pointer to a validus_state object which will contain the
 *                results of the operation upon success.
 * @param   file  Absolute or relative pathname to the file to hash.
 * @returns bool  `true` if the file is opened and read successfully, `false`
 *                otherwise.
 */
bool validus_hash_file(validus_state *state, const char *file);

/**
 * @brief Compares two validus_state objects for equality.
 *
 * @param   one  Pointer to the left hand side of the comparison.
 * @param   two  Poitner to the right hand side of the comparision.
 * @returns bool `true` if both states are identical, `false` otherwise.
 */
bool validus_compare(const validus_state *one, const validus_state *two);

/**
 * @brief Convertes a validus_state to hexadecimal string form.
 *
 * @param   state Pointer to the validus_state to convert.
 * @param   out   Pointer to a buffer to receive the formatted string.
 * @param   len   The length of `out` in octets. Must be at minimum 49.
 * @returns bool  `true` if input parameters are valid, and conversion succeeds,
 *                `false` otherwise.
 */
bool validus_state_to_string(const validus_state *state, char *out, size_t len);

////////////////////////// internal functions //////////////////////////////////

typedef struct {
#if defined(__WIN__)
    FILETIME ft;
#else
    struct timespec ts;
#endif
} validus_timer;

void validus_timer_start(validus_timer* timer);
float validus_timer_elapsed(validus_timer* timer);
void validus_get_local_time(const time_t* when, char out[256]);

#ifdef __cplusplus
}
#endif

#endif /* _VALIDUS_LIBRARY_H_INCLUDED */
