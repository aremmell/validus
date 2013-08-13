/*
 *  main.c : Implementation of the Validus hash function console
 *  utility
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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <time.h>
#include "Validus.h"
#include "ValidusLibrary.h"

#ifdef _WIN32
#include <conio.h>
#else
#include <curses.h>
#include <sys/types.h>
#endif /* _WIN32 */


/*
 * Function declarations
 */


int printusage(void);
int hashfile(char *file);
int hashstring(char *string);
int timetrial(void);
int testsuite(void);


/*
 * main() : Entry point
 */
int main(int argc, char *argv[]) {

  if (argc < 2 || strcmp(argv[1], "-?") == 0) {
    /* Print usage */
    return printusage();
  }

  if (strcmp(argv[1], "-s") == 0) {
    /* Hash string */
    return hashstring(argv[2]);
  }

  if (strcmp(argv[1], "-t") == 0) {
    /* Time trial */
    return timetrial();
  }

  if (strcmp(argv[1], "-x") == 0) {
    /* Test suite */
    return testsuite();
  }

  /* Hash file */
  return hashfile(argv[1]);
}

/*
 *printusage() : Displays usage information of this program
 */
int printusage(void) {

  printf("Validus -s [string]\t: Generates fingerprint for string\n");
  printf("Validus -t\t\t: Performs time-trial\n");
  printf("Validus -x\t\t: Displays test suite\n");
  printf("Validus [file]\t\t: Generates fingerprint for file\n");

  return 0;
}

/*
 * hashfile() : Hashes [file] and displays result
 */
int hashfile(char *file) {

  validus_t state = {0};

  if (0 != Validus_HashFile(&state, file)) {
    printf("Validus: Unable to hash file [%s]!\n", file);
    return 1;
  }

  printf("Validus [\"%s\"] = %08x%08x%08x%08x%08x%08x\n",
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
int hashstring(char *string) {

  validus_t state = {0};

  if (!string || !*string) {
    printf("Validus: No string supplied. Quitting.\n");
    return 1;
  }

  Validus_String(&state, string);

  printf("Validus [\"%s\"] = %08x%08x%08x%08x%08x%08x\n",
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
int timetrial(void) {

  validus_octet_t block[10000];
  int  i             = 0;
  float bps          = 0.0f;
  float mbs          = 0.0f;
  time_t elapsed     = 0;
  time_t start       = 0;
  time_t end         = 0;
  validus_t state    = {0};

  printf("Validus speed test - Processing 100000 10000-byte blocks...");

  fflush(stdout);

  time(&start);

  Validus_Init(&state);

  for (i = 100000; i >= 0; i--) {
    Validus_Append(&state, block, 10000);
  }

  Validus_Finish(&state);

  time(&end);

  elapsed = end - start;
  bps     = ((10000.0f * 100000.0f) / (float)elapsed);
  mbs     = (((float)bps / 1024.0f) / 1024.0f);

  printf("done.\nFingerprint = %08x%08x%08x%08x%08x%08x\nTime = %d seconds\nSpeed = %.02f MB/sec (%.02f bytes/sec)\n",
    state.f0,
    state.f1,
    state.f2,
    state.f3,
    state.f4,
    state.f5,
    elapsed,
    mbs,
    bps);

  return 0;
}

/*
 * testsuite() : Hashes predefined strings
 * and displays result; used to determine
 * if Validus is working correctly
 */
int testsuite(void) {

  validus_t state = {0};
  int   n      = 0;
  char *test[] = {
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

    Validus_String(&state, test[n]);

    printf("validus [\"%s\"] = %08x%08x%08x%08x%08x%08x\n",
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
