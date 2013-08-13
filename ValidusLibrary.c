/*
 *  ValidusLibrary.c : Implementation of the Validus hash function
 *  utility functions
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

#include "ValidusLibrary.h"

/*
 * FUNCTION: Validus_String()
 *
 * ARGUMENTS: Pointer to Validus state, and string to hash
 *
 * RETURN VALUE: 0 for success, 1 for failure
 */
int Validus_String(validus_t *state, const char *string) {

  int r = 1;

  if ((NULL != state) && (NULL != string)) {

    Validus_Init(state);
    Validus_Append(state, string, strlen(string));
    Validus_Finish(state);

    r = 0;

  }

  return r;
}

/*
 * FUNCTION: Validus_HashFile()
 *
 * ARGUMENTS: Pointer to Validus state, and file name
 *
 * RETURN VALUE: 0 for success, 1 for failure
 *
 * NOTES: Change VALIDUS_FILEBLOCKSIZE if you wish to adjust
 * the amount of bytes read per iteration.
 */
int Validus_HashFile(validus_t *state, char *file) {

  int r = 1;

  if ((NULL != state) && (NULL != file)) {

    FILE *f = fopen(file, "rb");

    if (NULL != f) {

      validus_octet_t *buf  = NULL;
    
      buf = (validus_octet_t *)calloc(sizeof(validus_octet_t), VALIDUS_FILEBLOCKSIZE);

      if (NULL != buf) {
        
        size_t result = 0;

        Validus_Init(state);

        while (!feof(f)) {

          result = fread((void*)buf, 1, VALIDUS_FILEBLOCKSIZE, f);

          if (0 != result) {

            Validus_Append(state, buf, result);
            memset(buf, 0, VALIDUS_FILEBLOCKSIZE);

          }

        }

        if (0 == ferror(f)) {

          Validus_Finish(state);
          r = 0;

        }

        free(buf);

      }

      fclose(f);

    }

  }

  return r;
}

/* FUNCTION: Validus_FingerprintCompare()
 *
 * ARGUMENTS: A pointer to two Validus states to compare
 *
 * RETURN VALUE: 0 if states are identical, 1 otherwise
 */
int Validus_FingerprintCompare(const validus_t *one, const validus_t *two) {

  int r = 1;

  if (one->f0 == two->f0 && one->f1 == two->f1 &&
      one->f2 == two->f2 && one->f3 == two->f3 &&
      one->f4 == two->f4 && one->f5 == two->f5)
  {

    r = 0;
  
  }

  return r;
}

/*
 * FUNCTION: Validus_FingerprintToString()
 *
 * ARGUMENTS: A pointer to a Validus state that contains
 * the fingerprint to be converted.
 *
 * RETURN VALUE: Returns 0 if successful, or 1 otherwise.
 *
 * NOTES: The buffer pointed to by the variable 'out' must
 * be at least 49 bytes (6 words have the potential to be 48 char
 * acters, plus a null-terminator.)  This function /WILL/ crash
 * if your buffer is not at least 49 bytes long.
 */
int Validus_FingerprintToString(const validus_t *state, char *out) {

  int r = 1;

  if ((NULL != state) && (NULL != out)) {

    memset(out, 0, 49);

    if (-1 != sprintf(out, "%08X%08X%08X%08X%08X%08X",
              state->f0, state->f1, state->f2,
              state->f3, state->f4, state->f5))
    {

      r = 0;

    }

  }

  return r;
}
