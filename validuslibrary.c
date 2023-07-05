/*
 * validuslibrary.c
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
#include "validuslibrary.h"

/*
 * FUNCTION: validus_hash_string()
 *
 * ARGUMENTS: Pointer to Validus state, and string to hash
 *
 * RETURN VALUE: 0 for success, 1 for failure
 */
bool validus_hash_string(validus_state* state, const char* string)
{
    if (!state || !string)
        return false;

    validus_init(state);
    validus_append(state, string, strnlen(string, VALIDUS_MAX_STRING));
    validus_finalize(state);

    return true;
}

bool validus_hash_mem(validus_state* state, const void* mem, validus_word len)
{
    if (!state || !mem || len == 0)
        return false;

    validus_init(state);
    validus_append(state, mem, len);
    validus_finalize(state);

    return true;
}
/*
 * FUNCTION: validus_hash_file()
 *
 * ARGUMENTS: Pointer to Validus state, and file name
 *
 * RETURN VALUE: 0 for success, 1 for failure
 *
 * NOTES: Change VALIDUS_FILEBLOCKSIZE if you wish to adjust
 * the amount of bytes read per iteration.
 */
bool validus_hash_file(validus_state* state, const char* file) {
  if (!state || (!file || !*file))
      return false;

    FILE *f = fopen(file, "rb");
    if (!f)
        return false;

    validus_octet *buf  = (validus_octet*)calloc(sizeof(validus_octet),
        VALIDUS_FILEBLOCKSIZE);

    if (!buf) {
        fclose(f);
        f = NULL;
        return false;
    }

    bool retval = false;
    validus_init(state);

    while (!feof(f)) {
        size_t result = fread((void*)buf, sizeof(validus_octet), VALIDUS_FILEBLOCKSIZE, f);

        if (0 != result) {
            validus_append(state, buf, result);
            memset(buf, 0, VALIDUS_FILEBLOCKSIZE);
        }
    }

    if (0 == ferror(f)) {
        validus_finalize(state);
        retval = true;
    }

    free(buf);
    buf = NULL;

    fclose(f);
    f = NULL;

    return retval;
}

/* FUNCTION: validus_compare()
 *
 * ARGUMENTS: A pointer to two Validus states to compare
 *
 * RETURN VALUE: 0 if states are identical, 1 otherwise
 */
bool validus_compare(const validus_state* one, const validus_state* two)
{
    if (!one || !two)
        return false;

    return (one->f0 == two->f0 && one->f1 == two->f1 &&
            one->f2 == two->f2 && one->f3 == two->f3 &&
            one->f4 == two->f4 && one->f5 == two->f5);
}

/*
 * FUNCTION: validus_state_to_string()
 *
 * ARGUMENTS: A pointer to a Validus state that contains
 * the fingerprint to be converted.
 *
 * RETURN VALUE: Returns 0 if successful, or 1 otherwise.
 *
 * NOTES: The buffer pointed to by the variable 'out' must
 * be at least 49 bytes (6 words have the potential to be 48 char
 * acters, plus a null-terminator).
 */

/**
 * @brief Convertes a validus_state to hexadecimal string form.
 *
 * @param   state Pointer to the validus_state to convert.
 * @param   out   Pointer to a buffer to receive the formatted string.
 * @param   len   The length of `out` in octets.
 * @returns bool  `true` if input parameters are valid, and conversion succeeds,
 *                `false` otherwise.
 */
bool validus_state_to_string(const validus_state* state, char* out, size_t len)
{
    if (!state || !out || len < 49)
        return false;

    int print = snprintf(out, len, "%08" PRIx32 "%08" PRIx32 "%08" PRIx32 "%08" PRIx32 "%08" PRIx32 "%08" PRIx32,
        state->f0, state->f1, state->f2, state->f3, state->f4, state->f5);

    return -1 != print;
}

void validus_timer_start(validus_timer* timer)
{
    int ret = clock_gettime(CLOCK_REALTIME, timer);
    if (0 != ret) {
        fprintf(stderr, "clock_gettime() failed: %s", strerror(errno));
        timer->tv_nsec = 0;
        timer->tv_nsec = 0;
    }
}

float validus_timer_elapsed(validus_timer* timer)
{
    validus_timer now;

    int ret = clock_gettime(CLOCK_REALTIME, &now);
    if (0 != ret) {
        fprintf(stderr, "clock_gettime() failed: %s", strerror(errno));
        return 0.0f;
    }

    /* milliseconds */
    return (float)((now.tv_sec * 1e3) + (now.tv_nsec / 1e6) - (timer->tv_sec * 1e3) +
            (timer->tv_nsec / 1e6));
}

void validus_get_local_time(const time_t* when, char out[256])
{
    struct tm* lt = localtime(when);
    if (lt)
        strftime(out, 256, "%T (%z)", lt);
}
