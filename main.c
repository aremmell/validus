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
#include <stdlib.h>

#include "validus.h"
#include "validuslibrary.h"
#include <version.h>

#if defined(_WIN32)
# include <conio.h>
#endif

int printusage(void);
int printversion(void);
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

    /* Version info */
    if (strncmp(argv[1], "-v", 2) == 0)
        return printversion();

    /* Hash file */
    return hashfile(argv[1]);
}

int printusage(void)
{
    fprintf(stderr, "\nvalidus usage:\n");
    fprintf(stderr, "\t-s <string>\t: Generates fingerprint for string\n");
    fprintf(stderr, "\t-t\t\t: Runs performance test\n");
    fprintf(stderr, "\t-x\t\t: Displays test suite\n");
    fprintf(stderr, "\t<file>\t\t: Generates fingerprint for file\n");
    fprintf(stderr, "\t-v\t\tDisplays version information\n");

    return EXIT_FAILURE;
}

int printversion(void)
{
    printf("validus v%"PRIu16".%"PRIu16".%"PRIu16"-%s\n",
        VERSION_MAJ, VERSION_MIN, VERSION_BLD, GIT_COMMIT_HASH);

    return EXIT_SUCCESS;
}

int hashfile(const char *file)
{
    validus_state state = {0};
    if (!validus_hash_file(&state, file)) {
        fprintf(stderr, "validus: Unable to hash file '%s'!\n", file);
        return EXIT_FAILURE;
    }

    printf("validus [\"%s\"] = %08"PRIx32"%08"PRIx32"%08"PRIx32"%08"PRIx32"%08"PRIx32"%08"PRIx32"\n",
        file, state.f0, state.f1, state.f2, state.f3, state.f4, state.f5);

    return EXIT_SUCCESS;
}

int hashstring(const char *string)
{
    if (!string || !*string) {
        fprintf(stderr, "validus: Invalid string supplied; ignoring.\n");
        return EXIT_FAILURE;
    }

    validus_state state = {0};
    validus_hash_string(&state, string);

    printf("validus [\"%s\"] = %08"PRIx32"%08"PRIx32"%08"PRIx32"%08"PRIx32"%08"PRIx32"%08"PRIx32"\n",
        string, state.f0, state.f1, state.f2, state.f3, state.f4, state.f5);

    return EXIT_SUCCESS;
}

int timetrial(void)
{
    static const size_t blocks = (size_t)1e6;
    static const size_t block_size = (size_t)1e5;

    validus_octet block[block_size];
    char timebuf[256] = {0};
    validus_timer timer = {0};
    time_t now;

    time(&now);
    validus_get_local_time(&now, timebuf);

    printf("Validus speed test: begin at %s; processing %zu %zu-byte blocks (%zu GB)... ",
        timebuf, blocks, block_size, (blocks * block_size) / 1000 / 1000 / 1000);
    fflush(stdout);

    validus_state state = {0};

    validus_timer_start(&timer);
    validus_init(&state);

    for (size_t i = blocks; i > 0; i--)
        validus_append(&state, block, block_size);

    validus_finalize(&state);

    float elapsed_msec = validus_timer_elapsed(&timer);
    float bps = ((float)block_size * (float)blocks) / (elapsed_msec / 1000.0f);
    float mbs = (bps / 1000.0f) / 1000.0f;

    time(&now);
    validus_get_local_time(&now, timebuf);

    printf("done; end at %s.\n"
           "Elapsed: %.03f seconds\n"
           "Throughput: %.2f MB/s\n"
           "Fingerprint: %08"PRIx32"%08"PRIx32"%08"PRIx32"%08"PRIx32"%08"PRIx32"%08"PRIx32"\n",
           timebuf, (elapsed_msec / 1e3), mbs, state.f0, state.f1, state.f2, state.f3, state.f4, state.f5);

    return EXIT_SUCCESS;
}

int testsuite(void)
{
    validus_state state = {0};
    int n               = 0;
    char *test_inputs[] = {
        "",
        "abc",
        "ABC",
        "validus",
        "111",
        "112",
        "hello, world",
        "dlrow ,olleh"
    };

    uint32_t word = 0xABCD1234;
    uint8_t fb    = *((uint8_t*)&word);
	if (fb == 0xAB)
		printf("\n=== Endianess detection: Big endian (fb = %" PRIx8 ") ===\n", fb);
	else
		printf("\n=== Endianess detection: Little endian (fb = %" PRIx8 ") ===\n", fb);

    for (n = 0; n < 8; ++n) {
        validus_hash_string(&state, test_inputs[n]);
        printf("validus [\"%s\"] = %08"PRIx32"-%08"PRIx32"-%08"PRIx32"-%08"PRIx32"-%08"PRIx32"-%08"PRIx32"\n",
            test_inputs[n], state.f0, state.f1, state.f2, state.f3, state.f4, state.f5);
    }

    return EXIT_SUCCESS;
}
