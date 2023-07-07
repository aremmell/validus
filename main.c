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
    if (!validus_hash_file(&state, file))
        return EXIT_FAILURE;

    printf("validus ['%s'] = " VALIDUS_FP_FMT_SPEC "\n",
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

    printf("validus ['%s'] = " VALIDUS_FP_FMT_SPEC "\n", string, state.f0,
        state.f1, state.f2, state.f3, state.f4, state.f5);

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

    printf("Validus speed test: begin at %s; %zu %zu-byte blocks (%zu GB)... ",
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

    printf("done; end at %s.\nElapsed: %.03f seconds\nThroughput: %.2f MB/s\n"
           "Fingerprint: " VALIDUS_FP_FMT_SPEC "\n", timebuf, (elapsed_msec / 1e3),
           mbs, state.f0, state.f1, state.f2, state.f3, state.f4, state.f5);

    return EXIT_SUCCESS;
}

void print_test_result(bool result, validus_state* state, const char* input) {
    const int color = result ? 32 : 31;
    printf("\x1b[%dmvalidus ['%s'] = " VALIDUS_FP_FMT_SPEC "\x1b[0m\n",
        color, input, state->f0, state->f1, state->f2, state->f3, state->f4, state->f5);
}

int testsuite(void)
{
    static const size_t num_inputs = 8;
    static const struct test_value {
        const char* str;
        validus_state kv;
    } test_inputs[num_inputs] = {
        {"",
            {0, 0, 0xd3f0ad33, 0x79790917, 0x69135e44, 0xeb28aeda, 0x40e5423d, 0xd2e956e7}},
        {"abc",
            {0, 0, 0xf7ffabe5, 0x4ddb09a9, 0x3ebde51b, 0x90d1796a, 0x63ea3cc1, 0xa5ed093f}},
        {"ABC",
            {0, 0, 0x9c273091, 0x9216af67, 0xc3d9a325, 0x4401ade8, 0x5920b7c1, 0xd707c65d}},
        {"validus",
            {0, 0, 0xa16bbad7, 0x293dac29, 0x04cc1807, 0x6636125c, 0x2c68c29c, 0xcffa779d}},
        {"1111111",
            {0, 0, 0x4f7879df, 0xe986f48e, 0x047190fe, 0x0961783a, 0x177b6dc1, 0x9d5f30d1}},
        {"1111112",
            {0, 0, 0x5f26b88d, 0xd4c24f7d, 0xe828d3ed, 0x18dc0a05, 0x45f26eb0, 0xc0b09061}},
        {"hello, world",
            {0, 0, 0xa54b0bad, 0xf8061b9b, 0x6f14c542, 0x0d2bd823, 0x9fbb7f67, 0x50b67af7}},
        {"dlrow ,olleh",
            {0, 0, 0x3a39f172, 0xc900b9d8, 0x6efe31dd, 0xc065bdf9, 0xe02c4837, 0x50f9af86}}
    };

    bool all_pass = true;
    validus_state state = {0};

    for (size_t n = 0; n < num_inputs; ++n) {
        validus_hash_string(&state, test_inputs[n].str);
        bool pass = validus_compare(&state, &test_inputs[n].kv);
        print_test_result(pass, &state, test_inputs[n].str);
        all_pass &= pass;
    }

    return all_pass ? EXIT_SUCCESS : EXIT_FAILURE;
}
