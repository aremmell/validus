/**
 * @file validusutil.c
 * @brief Implementation of Validus utility functions.
 *
 * Implements the functions utilized by the Validus CLI application.
 *
 * @author    Ryan M. Lederman \<lederman@gmail.com\>
 * @date      2004-2025
 * @version   1.0.4
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
#include "validusutil.h"

bool validus_hash_string(validus_state* state, const char* string)
{
    if (!state || !string)
        return false;

    validus_init(state);
    validus_append(state, string, strnlen(string, VALIDUS_MAX_STRING));
    validus_finalize(state);

    return true;
}

bool validus_hash_mem(validus_state* state, const void* mem, size_t len)
{
    if (!state || !mem || len == 0)
        return false;

    validus_init(state);
    validus_append(state, mem, len);
    validus_finalize(state);

    return true;
}

bool validus_hash_file(validus_state* state, const char* file) {
  if (!state || (!file || !*file))
      return false;

    FILE *f = fopen(file, "rb");
    if (!f) {
        fprintf(stderr, "failed to open file '%s': %d\n", file, errno);
        return false;
    }

    validus_octet *buf = calloc(sizeof(validus_octet), VALIDUS_FILE_BLOCKSIZE);

    if (!buf) {
        fprintf(stderr, "failed to allocate %zu octets of heap memory: %d\n",
            sizeof(validus_octet) * VALIDUS_FILE_BLOCKSIZE, errno);
        fclose(f);
        f = NULL;
        return false;
    }

    bool retval = false;
    validus_init(state);

    while (!feof(f) && !ferror(f)) {
        size_t result = fread((void*)buf, sizeof(validus_octet),
            VALIDUS_FILE_BLOCKSIZE, f);

        if (0 != result)
            validus_append(state, buf, result);
    }

    if (0 != ferror(f)) {
        fprintf(stderr, "failed to read from file '%s': %d\n", file, errno);
    } else {
        validus_finalize(state);
        retval = true;
    }

    free(buf);
    buf = NULL;

    fclose(f);
    f = NULL;

    return retval;
}

bool validus_state_to_string(const validus_state* state, char* out, size_t len)
{
    if (!state || !out || len < VALIDUS_FP_SIZE_O + 1)
        return false;

    int prn = snprintf(out, len, VALIDUS_FP_FMT_SPEC, state->f0, state->f1,
        state->f2, state->f3, state->f4, state->f5);

    return -1 != prn;
}

////////////////////////// internal functions //////////////////////////////////

void validus_timer_start(validus_timer* timer)
{
#if !defined(__WIN__)
    int ret = clock_gettime(CLOCK_REALTIME, &timer->ts);
    if (0 != ret) {
        fprintf(stderr, "clock_gettime() failed: %d\n", errno);
        timer->ts.tv_sec = 0;
        timer->ts.tv_nsec = 0;
    }
#else /* __WIN__ */
    GetSystemTimePreciseAsFileTime(&timer->ft);
#endif
}

double validus_timer_elapsed(const validus_timer* timer)
{
    validus_timer now;

#if !defined(__WIN__)
    int ret = clock_gettime(CLOCK_REALTIME, &now.ts);
    if (0 != ret) {
        fprintf(stderr, "clock_gettime() failed: %d\n", errno);
        return 0.0;
    }

    return ((((double)now.ts.tv_sec * 1e3) + ((double)now.ts.tv_nsec / 1e6)) -
        (((double)timer->ts.tv_sec * 1e3) + ((double)timer->ts.tv_nsec / 1e6)));
#else /* __WIN__ */
    GetSystemTimePreciseAsFileTime(&now.ft);

    ULARGE_INTEGER ulnow;
    ulnow.LowPart = now.ft.dwLowDateTime;
    ulnow.HighPart = now.ft.dwHighDateTime;

    ULARGE_INTEGER ultimer;
    ultimer.LowPart = timer->ft.dwLowDateTime;
    ultimer.HighPart = timer->ft.dwHighDateTime;

    return (float)((ulnow.QuadPart - ultimer.QuadPart) / 1e4);
#endif
}

const char* validus_get_local_time(void)
{
    static _Thread_local char buf[256] = {0};
    time_t now = time(NULL);
    struct tm lt = {0};
    bool lt_ret = false;

#if !defined(__WIN__)
# define LOCALTIME_VARIANT "r"
    lt_ret = NULL != localtime_r(&now, &lt);
#else /* __WIN__ */
# define LOCALTIME_VARIANT "s"
    lt_ret = 0 == localtime_s(&lt, &now);
#endif

    if (!lt_ret) {
        fprintf(stderr, "localtime_%s() failed: %d\n", LOCALTIME_VARIANT, errno);
    }

    if (0 == strftime(buf, sizeof(buf), "%T", &lt)) {
        buf[0] = '\0';
    }

    return &buf[0];
}
