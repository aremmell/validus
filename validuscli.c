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
#include "validuscli.h"

int main(int argc, char *argv[])
{
    /* Check argument count. */
    if (argc < 2) {
        _validus_cli_print_error("no argument supplied");
        goto _print_usage;
    }

    /* Print usage */
    if (strncmp(argv[1], VALIDUS_CLI_HELP, 2) == 0)
        goto _print_usage;

    /* Hash string */
    if (strncmp(argv[1], VALIDUS_CLI_STR, 2) == 0)
        return validus_cli_hash_string(argv[2]);

    /* Hash file */
    if (strncmp(argv[1], VALIDUS_CLI_FILE, 2) == 0)
        return validus_cli_hash_file(argv[2]);

    /* Performance measurement */
    if (strncmp(argv[1], VALIDUS_CLI_PERF, 2) == 0)
        return validus_cli_perf_test();

    /* Verify sanity */
    if (strncmp(argv[1], VALIDUS_CLI_VS, 2) == 0)
        return validus_cli_verify_sanity();

    /* Version info */
    if (strncmp(argv[1], VALIDUS_CLI_VER, 2) == 0)
        return validus_cli_print_ver();

    _validus_cli_print_error("unknown argument: '%s'", argv[1]);

_print_usage:
    return validus_cli_print_usage();
}

int validus_cli_print_usage(void)
{
    fprintf(stderr, "\n"VALIDUS_CLI_NAME" usage:\n");
    fprintf(stderr, "\t"VALIDUS_CLI_STR" <string>: Hash string and show fingerprint.\n");
    fprintf(stderr, "\t"VALIDUS_CLI_PERF": Performance measurement.\n");
    fprintf(stderr, "\t"VALIDUS_CLI_VS": Verify sanity.\n");
    fprintf(stderr, "\t"VALIDUS_CLI_FILE" <file>: Hash file and show fingerprint.\n");
    fprintf(stderr, "\t"VALIDUS_CLI_VER": Display version information.\n");

    return EXIT_FAILURE;
}

int validus_cli_print_ver(void)
{
    printf(VALIDUS_CLI_NAME" v%"PRIu16".%"PRIu16".%"PRIu16"-%s\n",
        VERSION_MAJ, VERSION_MIN, VERSION_BLD, GIT_COMMIT_HASH);
    return EXIT_SUCCESS;
}

int validus_cli_hash_file(const char *file)
{
    validus_state state = {0};
    if (!validus_hash_file(&state, file))
        return EXIT_FAILURE;

    printf(VALIDUS_CLI_NAME" ['%s'] = " VALIDUS_FP_FMT_SPEC "\n",
        file, state.f0, state.f1, state.f2, state.f3, state.f4, state.f5);

    return EXIT_SUCCESS;
}

int validus_cli_hash_string(const char *string)
{
    if (!string || !*string) {
        _validus_cli_print_error("invalid string supplied; ignoring.");
        return EXIT_FAILURE;
    }

    validus_state state = {0};
    if (!validus_hash_string(&state, string))
        return EXIT_FAILURE;

    printf(VALIDUS_CLI_NAME" ['%s'] = " VALIDUS_FP_FMT_SPEC "\n", string,
        state.f0, state.f1, state.f2, state.f3, state.f4, state.f5);

    return EXIT_SUCCESS;
}

int validus_cli_perf_test(void)
{
    validus_octet block[VALIDUS_CLI_PERF_BLOCKSIZE];
    memset(block, 0xee, sizeof(validus_octet) * VALIDUS_CLI_PERF_BLOCKSIZE);
    char timebuf[256] = {0};
    time_t now;

    time(&now);
    validus_get_local_time(&now, timebuf);

    printf(
        VALIDUS_CLI_NAME" perf test: begin at %s; %zu %zu-byte blocks (%zu GiB)...",
        timebuf,
        VALIDUS_CLI_PERF_BLOCKS,
        VALIDUS_CLI_PERF_BLOCKSIZE,
        (VALIDUS_CLI_PERF_BLOCKS * VALIDUS_CLI_PERF_BLOCKSIZE) / 1024ul / 1024ul / 1024ul
    );
    fflush(stdout);

    validus_timer timer = {0};
    validus_state state = {0};

    validus_timer_start(&timer);
    validus_init(&state);

    for (size_t i = VALIDUS_CLI_PERF_BLOCKS; i > 0; i--)
        validus_append(&state, block, VALIDUS_CLI_PERF_BLOCKSIZE);

    validus_finalize(&state);

    float elapsed_msec = validus_timer_elapsed(&timer);
    float bps = (float)(VALIDUS_CLI_PERF_BLOCKS * VALIDUS_CLI_PERF_BLOCKSIZE)
        / (elapsed_msec / 1000.0f);
    float mbs = (bps / 1024.0f) / 1024.0f;

    time(&now);
    validus_get_local_time(&now, timebuf);

    printf("done at %s.\nElapsed: %.03f sec\nThroughput: %.2f MiB/sec\n"
           "Fingerprint: " VALIDUS_FP_FMT_SPEC "\n", timebuf, (elapsed_msec / 1e3),
           mbs, state.f0, state.f1, state.f2, state.f3, state.f4, state.f5);

    return EXIT_SUCCESS;
}

void print_test_result(bool result, validus_state* state, const char* input) {
    const int color = result ? 32 : 31;
    static const size_t longest_input = 12;

    char padding[longest_input] = {0};
    size_t input_len = strnlen(input, longest_input);
    for (size_t n = input_len, off = 0; n < longest_input; n++, off++)
        padding[off] = ' ';

    printf(ANSI_ESC "97m" VALIDUS_CLI_NAME" ['%s'] %s = "
        ANSI_ESC "%dm" VALIDUS_FP_FMT_SPEC ANSI_ESC "0m" "\n", input, padding, color,
        state->f0, state->f1, state->f2, state->f3, state->f4, state->f5);
}

int validus_cli_verify_sanity(void)
{
    static const struct test_value {
        const char* str;
        validus_state kv;
    } test_inputs[VALIDUS_CLI_SANITY_INPUTS] = {
        {"",
            {{0}, 0xd3f0ad33, 0x79790917, 0x69135e44, 0xeb28aeda, 0x40e5423d, 0xd2e956e7}},
        {"abc",
            {{0}, 0xf7ffabe5, 0x4ddb09a9, 0x3ebde51b, 0x90d1796a, 0x63ea3cc1, 0xa5ed093f}},
        {"ABC",
            {{0}, 0x9c273091, 0x9216af67, 0xc3d9a325, 0x4401ade8, 0x5920b7c1, 0xd707c65d}},
        {"validus",
            {{0}, 0xa16bbad7, 0x293dac29, 0x04cc1807, 0x6636125c, 0x2c68c29c, 0xcffa779d}},
        {"1111111",
            {{0}, 0x4f7879df, 0xe986f48e, 0x047190fe, 0x0961783a, 0x177b6dc1, 0x9d5f30d1}},
        {"1111112",
            {{0}, 0x5f26b88d, 0xd4c24f7d, 0xe828d3ed, 0x18dc0a05, 0x45f26eb0, 0xc0b09061}},
        {"hello, world",
            {{0}, 0xa54b0bad, 0xf8061b9b, 0x6f14c542, 0x0d2bd823, 0x9fbb7f67, 0x50b67af7}},
        {"dlrow ,olleh",
            {{0}, 0x3a39f172, 0xc900b9d8, 0x6efe31dd, 0xc065bdf9, 0xe02c4837, 0x50f9af86}}
    };

    bool all_pass = true;
    validus_state state = {0};

    for (size_t n = 0; n < VALIDUS_CLI_SANITY_INPUTS; ++n) {
        validus_hash_string(&state, test_inputs[n].str);
        bool pass = validus_compare(&state, &test_inputs[n].kv);
        print_test_result(pass, &state, test_inputs[n].str);
        all_pass &= pass;
    }

    return all_pass ? EXIT_SUCCESS : EXIT_FAILURE;
}

void _validus_cli_print_error(const char* format, ...)
{
    if (!format || !*format)
        return;

    va_list args  = {0};
    char buf[VALIDUS_CLI_MAX_ERROR] = {0};

    va_start(args, format);
    (void)vsnprintf(buf, VALIDUS_CLI_MAX_ERROR, format, args);

    fprintf(stderr, "%s%s%s%s", ANSI_ESC"31m", VALIDUS_CLI_NAME": ", buf,
        ANSI_ESC"0m" "\n");
}
