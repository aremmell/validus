/*
 * main.c
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
#include <stdio.h>

#include "validus.h"
#include "validuslibrary.h"

#ifdef _WIN32
#include <conio.h>
#else
/* #include <curses.h> */
#include <sys/types.h>
#endif /* _WIN32 */

int printusage(void);
int hashfile(const char *file);
int hashstring(const char *string);
int timetrial(void);
int testsuite(void);

int main(int argc, char *argv[])
{
    /* Print usage */
    if (argc < 2 || strncmp(argv[1], "-h", 2) == 0)
        return printusage();

    /* Hash string */
    if (strncmp(argv[1], "-s", 2) == 0)
        return hashstring(argv[2]);

    /* Time trial */
    if (strncmp(argv[1], "-t", 2) == 0)
        return timetrial();

    /* Test suite */
    if (strncmp(argv[1], "-x", 2) == 0)
        return testsuite();

    /* Hash file */
    return hashfile(argv[1]);
}

/*
 *printusage() : Displays usage information of this program
 */
int printusage(void)
{
    printf("validus -s [string]\t: Generates fingerprint for string\n");
    printf("validus -t\t\t: Performs time-trial\n");
    printf("validus -x\t\t: Displays test suite\n");
    printf("validus [file]\t\t: Generates fingerprint for file\n");

    return 0;
}

/*
 * hashfile() : Hashes [file] and displays result
 */
int hashfile(const char *file)
{
    validus_state state = {0};
    if (0 != validus_hash_file(&state, file)) {
        printf("validus: Unable to hash file [%s]!\n", file);
        return 1;
    }

    printf("validus [\"%s\"] = %08x%08x%08x%08x%08x%08x\n",
        file,
        state.f0,
        state.f1,
        state.f2,
        state.f3,
        state.f4,
        state.f5);

    return 0;
}

/*
 * hashstring() : Hashes [string] and displays result
 */
int hashstring(const char *string)
{
    if (!string || !*string) {
        printf("validus: No string supplied. Quitting.\n");
        return 1;
    }

    validus_state state = {0};
    validus_hash_string(&state, string);

    printf("validus [\"%s\"] = %08x%08x%08x%08x%08x%08x\n",
        string,
        state.f0,
        state.f1,
        state.f2,
        state.f3,
        state.f4,
        state.f5);

    return 0;
}

/*
 * timetrial() : Hashes 1GB of data and
 * displays elapsed time and result
 */
int timetrial(void)
{
    validus_octet block[10000];
    int  i              = 0;
    float bps           = 0.0f;
    float mbs           = 0.0f;
    time_t elapsed      = 0;
    time_t start        = 0;
    time_t end          = 0;
    validus_state state = {0};

    printf("Validus speed test: processing 100000 10000-byte blocks... ");
    fflush(stdout);

    time(&start);
    validus_init(&state);

    for (i = 100000; i >= 0; i--)
        validus_append(&state, block, 10000);

    validus_finalize(&state);
    time(&end);

    elapsed = end - start;
    bps     = ((10000.0f * 100000.0f) / (float)elapsed);
    mbs     = (((float)bps / 1024.0f) / 1024.0f);

    printf("done.\nFingerprint = %08" PRIx32 "%08" PRIx32 "%08" PRIx32 "%08" PRIx32 "%08" PRIx32 "%08" PRIx32 "\nTime = %ld seconds\nSpeed = %.02f MiB/sec (%.02f bytes/sec)\n",
        state.f0, state.f1, state.f2, state.f3, state.f4, state.f5, elapsed,mbs, bps);

    return 0;
}

/*
 * testsuite() : Hashes predefined strings
 * and displays result; used to determine
 * if Validus is working correctly
 */
int testsuite(void)
{
    validus_state state = {0};
    int n               = 0;
    char *test[]        = {
        "",
        "abc",
        "ABC",
        "validus",
        "hash function",
        "testing123",
        "0123456789",
        "0987654321"
    };

    for (n = 0; n < 8; ++n) {
        validus_hash_string(&state, test[n]);
        printf("validus [\"%s\"] = %08"PRIx32"%08"PRIx32"%08"PRIx32"%08"PRIx32"%08"PRIx32"%08"PRIx32"\n",
            test[n],
            state.f0,
            state.f1,
            state.f2,
            state.f3,
            state.f4,
            state.f5);
    }

    return 0;
}
