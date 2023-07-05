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
#include <stdbool.h>
#include <sys/stat.h>

#define VALIDUS_FILEBLOCKSIZE 8192
#define VALIDUS_MAX_STRING    2048

#ifdef __cplusplus
extern "C" {
#endif

/*
 * validus_hash_string()
 * Use this function to hash a string
 */
bool validus_hash_string(validus_state *state, const char *string);

/*
 * validus_hash_mem()
 * Hashes a block of memory
 */
bool validus_hash_mem(validus_state *state, const void *mem, validus_int size);

/*
 * validus_hash_file()
 * Hashes a file on disk
 */
bool validus_hash_file(validus_state *state, char *file);

/*
 * validus_compare()
 * Compares two validus_state structures to
 * verify that they contain identical fingerprint.
 */
bool validus_compare(const validus_state *one, const validus_state *two);

/*
 * validus_state_to_string()
 * Formats a Validus fingerprint as a string
 * usable for displaying.
 */
bool validus_state_to_string(const validus_state *state, char *out, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* _VALIDUS_LIBRARY_H_INCLUDED */
