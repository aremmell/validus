/**
 * @file validuscli.h
 * @brief Definitions for the Validus CLI application.
 *
 * @author    Ryan M. Lederman \<lederman@gmail.com\>
 * @date      2004-2025
 * @version   1.0.5
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
#ifndef _VALIDUS_CLI_H_INCLUDED
# define _VALIDUS_CLI_H_INCLUDED

# include "validusutil.h"
# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include <version.h>

# if defined(__WIN__)
#  include <conio.h>
# endif

/////////////////////////////// constants //////////////////////////////////////

# define VALIDUS_CLI_HELP "-h"
# define VALIDUS_CLI_STR  "-s"
# define VALIDUS_CLI_FILE "-f"
# define VALIDUS_CLI_PERF "-p"
# define VALIDUS_CLI_VS   "-t"
# define VALIDUS_CLI_VER  "-v"

# define VALIDUS_CLI_NAME "validus"

# define ANSI_ESC   "\x1b["
# define ANSI_WHITE ANSI_ESC "97m"
# define ANSI_RED   ANSI_ESC "31m"
# define ANSI_ULINE ANSI_ESC "4m"
# define ANSI_BOLD  ANSI_ESC "1m"
# define ANSI_RESET ANSI_ESC "0m"

# define VALIDUS_CLI_PERF_BLKS    (1024ULL * 1024ULL)
# define VALIDUS_CLI_PERF_BLKSIZE (1024ULL * 10ULL)

# define VALIDUS_CLI_SANITY_INPUTS 8
# define VALIDUS_CLI_MAX_ERROR     512

/////////////////////////// function exports ///////////////////////////////////

int validus_cli_print_usage(void);
int validus_cli_print_ver(void);
int validus_cli_hash_file(const char* file);
int validus_cli_hash_string(const char* string);
int validus_cli_perf_test(void);
int validus_cli_verify_sanity(void);

//////////////////////////// internal functions ////////////////////////////////

void _validus_cli_print_error(const char* format, ...);

#endif /* !_VALIDUS_CLI_H_INCLUDED */
