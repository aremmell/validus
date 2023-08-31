/**
 * @file validusutil.h
 * @brief Definitions of the Validus utility functions.
 *
 * Defines the functions utilized by the Validus CLI application.
 *
 * @author    Ryan M. Lederman \<lederman@gmail.com\>
 * @date      2004-2023
 * @version   1.0.3
 * @copyright The MIT License (MIT)
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
#ifndef _VALIDUS_UTIL_H_INCLUDED
# define _VALIDUS_UTIL_H_INCLUDED

# include "validus.h"
# include <stdio.h>
# include <stdlib.h>
# include <inttypes.h>
# include <string.h>
# include <errno.h>
# include <time.h>

# if defined(_WIN32)
#  define WIN32_LEAN_AND_MEAN
#  define WINVER       0x0A00
#  define _WIN32_WINNT 0x0A00
#  include <windows.h>
#  define __WIN__
# endif

/**
 * @defgroup util Utility
 *
 * Utility wrappers around the core Validus implementation. Provides functions
 * for hashing files, strings, and blocks of memory, etc.
 *
 * @addtogroup util
 * @{
 */

///////////////////////////////// macros ///////////////////////////////////////

/** The size, in octets used to read blocks of data from a file. */
# define VALIDUS_FILE_BLOCKSIZE 8192ul

/** The maximum size, in octets of a string to hash. */
# define VALIDUS_MAX_STRING 2048ul

/** Format specifier string for a Validus fingerprint. */
# define VALIDUS_FP_FMT_SPEC \
    "%08" PRIx32 "%08" PRIx32 "%08" PRIx32 "%08" PRIx32 "%08" PRIx32 "%08" PRIx32

//////////////////////////// function exports //////////////////////////////////

# ifdef __cplusplus
extern "C" {
# endif

/**
 * @brief Hashes a string.
 *
 * @param   state  Pointer to a validus_state object which will contain the
 *                 results of the operation upon success.
 * @param   string Pointer to the string to hash. Will be read up until the first
 *                 null terminator, or VALIDUS_MAX_STRING; whichever comes first.
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
bool validus_hash_mem(validus_state* state, const void* mem, size_t len);

/**
 * @brief Hashes a file.
 *
 * @note If a relative pathname is supplied, it shall be treated as relative
 * to the current working directory. This may or may not be the same directory
 * that this program resides in.
 *
 * @note The preprocessor macro VALIDUS_FILE_BLOCKSIZE may be modified at compile
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
 * @brief Converts a validus_state to hexadecimal string form.
 *
 * @param   state Pointer to the validus_state to convert.
 * @param   out   Pointer to a buffer to receive the formatted string.
 * @param   len   The length of `out` in octets. Must be at minimum 49.
 * @returns bool  `true` if input parameters are valid, and conversion succeeds,
 *                `false` otherwise.
 */
bool validus_state_to_string(const validus_state *state, char *out, size_t len);

/** @} */

////////////////////////// internal functions //////////////////////////////////

/** A platform-dependent timer used for performance measurement. */
typedef struct {
# if defined(__WIN__)
    FILETIME ft;        /**< The timer type on Windows. */
# else
    struct timespec ts; /**< The timer type on *nix */
# endif
} validus_timer;

/**
 * @brief Starts a validus_timer.
 *
 * @param timer Pointer to the validus_timer which should be initialized to the
 *              chosen clock's current value.
 */
void validus_timer_start(validus_timer* timer);

/**
 * @brief Returns the milliseconds that have elapsed since `timer` was started.
 *
 * @param timer Pointer to the validus_timer which should be examined
 *              for its elapsed time.
 * @returns float The number of milliseconds that have elapsed since
 *                ::validus_timer_start was called for `timer`.
 */
float validus_timer_elapsed(validus_timer* timer);

/** Returns the local time, formatted as a string. */
const char* validus_get_local_time(void);

# ifdef __cplusplus
}
# endif

#endif /* !_VALIDUS_UTIL_H_INCLUDED */
