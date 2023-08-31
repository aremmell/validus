/**
 * @file validuscli.c
 * @brief Implementation of the Validus CLI application.
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

    _validus_cli_print_error("unknown option: '%s'", argv[1]);

_print_usage:
    return validus_cli_print_usage();
}

int validus_cli_print_usage(void)
{
    fprintf(stderr, ANSI_BOLD VALIDUS_CLI_NAME " usage:" ANSI_RESET "\n");
    fprintf(stderr, "\t" VALIDUS_CLI_STR " " ANSI_ULINE "string" ANSI_RESET
        " Hash string and output fingerprint\n");
    fprintf(stderr, "\t" VALIDUS_CLI_FILE " " ANSI_ULINE "file" ANSI_RESET
        "   Hash file and output fingerprint\n");
    fprintf(stderr, "\t" VALIDUS_CLI_PERF "        Performance evaluation test\n");
    fprintf(stderr, "\t" VALIDUS_CLI_VS "        Verify that Validus is functioning correctly\n");
    fprintf(stderr, "\t" VALIDUS_CLI_VER "        Display version information\n");
    fprintf(stderr, "\t" VALIDUS_CLI_HELP "        Show this message\n");

    return EXIT_FAILURE;
}

int validus_cli_print_ver(void)
{
    printf("%" PRIu16 ".%" PRIu16 ".%" PRIu16 "%s (%s)\n",
        VERSION_MAJ, VERSION_MIN, VERSION_BLD, VERSION_TYPE, GIT_COMMIT_HASH);
    return EXIT_SUCCESS;
}

int validus_cli_hash_file(const char *file)
{
    if (!file || !*file) {
        _validus_cli_print_error("invalid file name supplied; ignoring.");
        return EXIT_FAILURE;
    }

    validus_state state = {0};
    if (!validus_hash_file(&state, file))
        return EXIT_FAILURE;

    printf(VALIDUS_FP_FMT_SPEC "\n", state.f0, state.f1, state.f2, state.f3,
        state.f4, state.f5);

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

    printf(VALIDUS_FP_FMT_SPEC "\n", state.f0, state.f1, state.f2, state.f3,
        state.f4, state.f5);

    return EXIT_SUCCESS;
}

int validus_cli_perf_test(void)
{
    validus_octet block[VALIDUS_CLI_PERF_BLKSIZE];
    memset(block, 0xee, sizeof(validus_octet) * VALIDUS_CLI_PERF_BLKSIZE);

    printf(
        VALIDUS_CLI_NAME" perf test: begin at %s: %llu %llu-byte blocks (%llu GiB)...",
        validus_get_local_time(),
        VALIDUS_CLI_PERF_BLKS,
        VALIDUS_CLI_PERF_BLKSIZE,
        ((VALIDUS_CLI_PERF_BLKS * VALIDUS_CLI_PERF_BLKSIZE) / 3072ULL)
    );
    fflush(stdout);

    validus_timer timer = {0};
    validus_state state = {0};

    validus_timer_start(&timer);
    validus_init(&state);

    for (size_t i = VALIDUS_CLI_PERF_BLKS; i > 0; i--)
        validus_append(&state, block, VALIDUS_CLI_PERF_BLKSIZE);

    validus_finalize(&state);

    float elapsed_msec = validus_timer_elapsed(&timer);
    double bps = (double)(VALIDUS_CLI_PERF_BLKS * VALIDUS_CLI_PERF_BLKSIZE)
        / ((double)elapsed_msec / 1e3);
    double mbs = bps / 1024.0 / 1024.0;

    printf(" done at %s:\n\telapsed: %.03f sec\n\tthroughput: %.2f MiB/sec\n\t"
           "fingerprint: " VALIDUS_FP_FMT_SPEC "\n", validus_get_local_time(),
           ((double)elapsed_msec / 1e3), mbs, state.f0, state.f1, state.f2, state.f3,
           state.f4, state.f5);

    return EXIT_SUCCESS;
}

void print_test_result(bool result, validus_state* const state, const char* input) {
    const int color = result ? 32 : 31;
    static const size_t longest_input = 12;

    char* padding = calloc(longest_input + 1, sizeof(char));
    if (padding) {
        size_t input_len = strnlen(input, longest_input);
        for (size_t n = input_len, off = 0; n < longest_input; n++, off++)
            padding[off] = ' ';

        printf(ANSI_WHITE VALIDUS_CLI_NAME " ['%s']%s = "
            ANSI_ESC "%dm" VALIDUS_FP_FMT_SPEC ANSI_RESET "\n", input, padding, color,
            state->f0, state->f1, state->f2, state->f3, state->f4, state->f5);

        free(padding);
    }
}

int validus_cli_verify_sanity(void)
{
    typedef struct {
        const char* const str;
        validus_state kv;
    } test_value;

    static const test_value test_inputs[VALIDUS_CLI_SANITY_INPUTS] = {
        {"",
            {{0}, 0xd3f0ad33U, 0x79790917U, 0x69135e44U, 0xeb28aedaU, 0x40e5423dU, 0xd2e956e7U}},
        {"abc",
            {{0}, 0xf7ffabe5U, 0x4ddb09a9U, 0x3ebde51bU, 0x90d1796aU, 0x63ea3cc1U, 0xa5ed093fU}},
        {"ABC",
            {{0}, 0x9c273091U, 0x9216af67U, 0xc3d9a325U, 0x4401ade8U, 0x5920b7c1U, 0xd707c65dU}},
        {"validus",
            {{0}, 0xa16bbad7U, 0x293dac29U, 0x04cc1807U, 0x6636125cU, 0x2c68c29cU, 0xcffa779dU}},
        {"1111111",
            {{0}, 0x4f7879dfU, 0xe986f48eU, 0x047190feU, 0x0961783aU, 0x177b6dc1U, 0x9d5f30d1U}},
        {"1111112",
            {{0}, 0x5f26b88dU, 0xd4c24f7dU, 0xe828d3edU, 0x18dc0a05U, 0x45f26eb0U, 0xc0b09061U}},
        {"hello, world",
            {{0}, 0xa54b0badU, 0xf8061b9bU, 0x6f14c542U, 0x0d2bd823U, 0x9fbb7f67U, 0x50b67af7U}},
        {"dlrow ,olleh",
            {{0}, 0x3a39f172U, 0xc900b9d8U, 0x6efe31ddU, 0xc065bdf9U, 0xe02c4837U, 0x50f9af86U}}
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

    char buf[VALIDUS_CLI_MAX_ERROR] = {0};

    va_list args;
    va_start(args, format);
    (void)vsnprintf(buf, VALIDUS_CLI_MAX_ERROR, format, args);
    va_end(args);

    fprintf(stderr, "%s%s%s%s", ANSI_RED, VALIDUS_CLI_NAME ": ", buf,
        ANSI_RESET "\n");
}
