/*
 *  ValidusLibrary.h : Implementation of the Validus hash function
 *  library functions
 *
 *  Copyright 2003-2013 - All Rights Reserved
 *
 *  Description:
 *    Validus is a one-way hash function that generates 192-bit
 *    fingerprints.  It is computationally infeasible to find
 *    two messages that hash to the same fingerprint value.
 *
 *  Author:
 *    Ryan Matthew Lederman
 *    lederman@gmail.com
 *
 *  Date:
 *    March 03, 2004
 *
 *  Version: 1.0.0
 *
 *  License:
 *    This software is provided to the end user "as-is", with no warranty
 *    implied or otherwise.  The author is for all intents and purposes
 *    the owner of this source code.  The author grants the end user the
 *    privilege to use this software in any application, commercial or
 *    otherwise, with the following restrictions:
 *      1.) Mention of the Validus name must be present in any product
 *       implementing this software.
 *      2.) If the end user wishes to modify this source code, he/she must
 *       not distribute the source code as the original source code, and
 *       must clearly document the changes made to the source code in any
 *       distribution of said source code.
 *      3.) This license agreement must always be displayed in any version
 *       of the Validus source code.
 *    The author will not be held liable for ANY damage, loss of data,
 *    loss of income, or any other form of loss that results directly or 
 *    indirectly from the use of this software.
 */

#ifndef _VALIDUS_LIBRARY_H
#define _VALIDUS_LIBRARY_H

#include "Validus.h"
#include <stdio.h>
#include <sys/stat.h>


/*
 * Constants
 */
#define VALIDUS_FILEBLOCKSIZE 8192


/*
 * Function signatures
 */


#ifdef __cplusplus
  extern "C" {
#endif

/*
 * Validus_String()
 * Use this function to hash a string
 */
int Validus_String(validus_t *state, const char *string);

/*
 * Validus_Memory()
 * Hashes a block of memory
 */
int Validus_Memory(validus_t *state, const void *mem, validus_int_t size);

/*
 * Validus_HashFile()
 * Hashes a file on disk
 */
int Validus_HashFile(validus_t *state, char *file);

/*
 * Validus_FingerprintCompare()
 * Compares two validus_t structures to
 * verify that they contain identical fingerprint.
 */
int Validus_FingerprintCompare(const validus_t *one, const validus_t *two);

/*
 * Validus_FingerprintToString()
 * Formats a Validus fingerprint as a string
 * usable for displaying.
 */
int Validus_FingerprintToString(const validus_t *state, char *out);

#ifdef __cplusplus
  }
#endif

#endif /* _VALIDUS_LIBRARY_H */
