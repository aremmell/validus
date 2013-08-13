/*
 *  Validus.c : Implementation of the Validus hash function
 *
 *  Copyright 2003-2013 - All Rights Reserved
 *
 *  Description:
 *    Validus is a one-way hash function that generates 192-bit
 *    fingerprints.  It is computationally infeasible to find
 *    two messages that hash to the same fingerprint value.
 *
 *  Author:
 *   Ryan Matthew Lederman
 *   lederman@gmail.com
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

#include "Validus.h"

/*
 * FUNCTION: Validus_Append()
 *
 * SYNOPSIS: Processes a block of data, adding the results
 * to the Validus state passed.
 *
 * ARGUMENTS: Pointer to a Validus state, pointer to the data 
 * to be processed, and the length of the data in octets.
 */
void Validus_Append(validus_t *pstate, const void *data, validus_word_t len) {

  const validus_word_t *ptr = (validus_word_t *)data;
  validus_word_t left       = 0;
  validus_word_t done       = 0;

  pstate->bits[1] += (len >> 29);
  
  if (((pstate->bits[0] += (len << 3)) < (len << 3))) { pstate->bits[1]++; }

  while ((left = (len - done))) {

    if (left > 191) {

      _Validus_Process(pstate, ptr);
      done += 192;
      ptr  += 48;

    } else {

      validus_word_t stk[48];

      memcpy(stk, ptr, left);
      memset(((validus_octet_t *)stk) + left, 0, 192 - left);
      _Validus_Process(pstate, stk);
      done += left;

    }

  }

}

/*
 * FUNCTION: _Validus_Process()
 *
 * SYNOPSIS: Processes a 192-octet block of data, appending
 * the results to the Validus state.
 *
 * ARGUMENTS: Pointer to the Validus state, and the block to be
 * processed
 *
 * NOTES: This function is used internally; you should never
 * call this function directly
 */
void _Validus_Process(validus_t *pstate, const validus_word_t *blk32) {

  validus_word_t a = pstate->f0;
  validus_word_t b = pstate->f1;
  validus_word_t c = pstate->f2;
  validus_word_t d = pstate->f3;
  validus_word_t e = pstate->f4;
  validus_word_t f = pstate->f5;

#ifdef VALIDUS_ENDIAN_BIG
{
  validus_int_t n;
  validus_word_t stk[48];

  for(n = 47; n >= 0; n--) { OCTETSWAP(stk[n], ((validus_octet_t *)&blk32[n])); }

  blk32 = stk;
}
#else
{  
  if (0 == WORDALIGNED(blk32)) {

    validus_word_t stk[48];
    memcpy(stk, blk32, 192);
    blk32 = stk;

  }
}
#endif

  VC_0(d, c, b, a, f, e,  2,  3, blk32[47], VALIDUS_0);
  VC_0(c, b, a, f, e, d,  7,  6, blk32[46], VALIDUS_1);
  VC_0(b, a, f, e, d, c, 10,  9, blk32[45], VALIDUS_2);
  VC_0(a, f, e, d, c, b, 15, 12, blk32[44], VALIDUS_3);
  VC_0(f, e, d, c, b, a, 20, 21, blk32[43], VALIDUS_4);
  VC_0(e, d, c, b, a, f, 25, 24, blk32[42], VALIDUS_5);
  VC_0(d, c, b, a, f, e,  2,  3, blk32[41], VALIDUS_6);
  VC_0(c, b, a, f, e, d,  7,  6, blk32[40], VALIDUS_7);
  VC_0(b, a, f, e, d, c, 10,  9, blk32[39], VALIDUS_8);
  VC_0(a, f, e, d, c, b, 15, 12, blk32[38], VALIDUS_9);
  VC_0(f, e, d, c, b, a, 20, 21, blk32[37], VALIDUS_10);
  VC_0(e, d, c, b, a, f, 25, 24, blk32[36], VALIDUS_11);
  VC_0(d, c, b, a, f, e,  2,  3, blk32[35], VALIDUS_12);
  VC_0(c, b, a, f, e, d,  7,  6, blk32[34], VALIDUS_13);
  VC_0(b, a, f, e, d, c, 10,  9, blk32[33], VALIDUS_14);
  VC_0(a, f, e, d, c, b, 15, 12, blk32[32], VALIDUS_15);
  VC_0(f, e, d, c, b, a, 20, 21, blk32[31], VALIDUS_16);
  VC_0(e, d, c, b, a, f, 25, 24, blk32[30], VALIDUS_17);
  VC_0(d, c, b, a, f, e,  2,  3, blk32[29], VALIDUS_18);
  VC_0(c, b, a, f, e, d,  7,  6, blk32[28], VALIDUS_19);
  VC_0(b, a, f, e, d, c, 10,  9, blk32[27], VALIDUS_20);
  VC_0(a, f, e, d, c, b, 15, 12, blk32[26], VALIDUS_21);
  VC_0(f, e, d, c, b, a, 20, 21, blk32[25], VALIDUS_22);
  VC_0(e, d, c, b, a, f, 25, 24, blk32[24], VALIDUS_23);
  VC_0(d, c, b, a, f, e,  2,  3, blk32[ 0], VALIDUS_24);
  VC_0(c, b, a, f, e, d,  7,  6, blk32[ 1], VALIDUS_25);
  VC_0(b, a, f, e, d, c, 10,  9, blk32[ 2], VALIDUS_26);
  VC_0(a, f, e, d, c, b, 15, 12, blk32[ 3], VALIDUS_27);
  VC_0(f, e, d, c, b, a, 20, 21, blk32[ 4], VALIDUS_28);
  VC_0(e, d, c, b, a, f, 25, 24, blk32[ 5], VALIDUS_29);
  VC_0(d, c, b, a, f, e,  2,  3, blk32[ 6], VALIDUS_30);
  VC_0(c, b, a, f, e, d,  7,  6, blk32[ 7], VALIDUS_31);
  VC_0(b, a, f, e, d, c, 10,  9, blk32[ 8], VALIDUS_32);
  VC_0(a, f, e, d, c, b, 15, 12, blk32[ 9], VALIDUS_33);
  VC_0(f, e, d, c, b, a, 20, 21, blk32[10], VALIDUS_34);
  VC_0(e, d, c, b, a, f, 25, 24, blk32[11], VALIDUS_35);
  VC_0(d, c, b, a, f, e,  2,  3, blk32[12], VALIDUS_36);
  VC_0(c, b, a, f, e, d,  7,  6, blk32[13], VALIDUS_37);
  VC_0(b, a, f, e, d, c, 10,  9, blk32[14], VALIDUS_38);
  VC_0(a, f, e, d, c, b, 15, 12, blk32[15], VALIDUS_39);
  VC_0(f, e, d, c, b, a, 20, 21, blk32[16], VALIDUS_40);
  VC_0(e, d, c, b, a, f, 25, 24, blk32[17], VALIDUS_41);
  VC_0(d, c, b, a, f, e,  2,  3, blk32[18], VALIDUS_42);
  VC_0(c, b, a, f, e, d,  7,  6, blk32[19], VALIDUS_43);
  VC_0(b, a, f, e, d, c, 10,  9, blk32[20], VALIDUS_44);
  VC_0(a, f, e, d, c, b, 15, 12, blk32[21], VALIDUS_45);
  VC_0(f, e, d, c, b, a, 20, 21, blk32[22], VALIDUS_46);
  VC_0(e, d, c, b, a, f, 25, 24, blk32[23], VALIDUS_47);

  VC_1(d, c, b, a, f, e,  5,  4, blk32[22], VALIDUS_48);
  VC_1(c, b, a, f, e, d, 13, 14, blk32[20], VALIDUS_49);
  VC_1(b, a, f, e, d, c, 17, 16, blk32[18], VALIDUS_50);
  VC_1(a, f, e, d, c, b, 22, 19, blk32[23], VALIDUS_51);
  VC_1(f, e, d, c, b, a, 26, 23, blk32[21], VALIDUS_52);
  VC_1(e, d, c, b, a, f, 28, 29, blk32[19], VALIDUS_53);
  VC_1(d, c, b, a, f, e,  5,  4, blk32[16], VALIDUS_54);
  VC_1(c, b, a, f, e, d, 13, 14, blk32[14], VALIDUS_55);
  VC_1(b, a, f, e, d, c, 17, 16, blk32[12], VALIDUS_56);
  VC_1(a, f, e, d, c, b, 22, 19, blk32[17], VALIDUS_57);
  VC_1(f, e, d, c, b, a, 26, 23, blk32[15], VALIDUS_58);
  VC_1(e, d, c, b, a, f, 28, 29, blk32[13], VALIDUS_59);
  VC_1(d, c, b, a, f, e,  5,  4, blk32[10], VALIDUS_60);
  VC_1(c, b, a, f, e, d, 13, 14, blk32[ 8], VALIDUS_61);
  VC_1(b, a, f, e, d, c, 17, 16, blk32[ 6], VALIDUS_62);
  VC_1(a, f, e, d, c, b, 22, 19, blk32[11], VALIDUS_63);
  VC_1(f, e, d, c, b, a, 26, 23, blk32[ 9], VALIDUS_64);
  VC_1(e, d, c, b, a, f, 28, 29, blk32[ 7], VALIDUS_65);
  VC_1(d, c, b, a, f, e,  5,  4, blk32[ 4], VALIDUS_66);
  VC_1(c, b, a, f, e, d, 13, 14, blk32[ 2], VALIDUS_67);
  VC_1(b, a, f, e, d, c, 17, 16, blk32[ 0], VALIDUS_68);
  VC_1(a, f, e, d, c, b, 22, 19, blk32[ 5], VALIDUS_69);
  VC_1(f, e, d, c, b, a, 26, 23, blk32[ 3], VALIDUS_70);
  VC_1(e, d, c, b, a, f, 28, 29, blk32[ 1], VALIDUS_71);
  VC_1(d, c, b, a, f, e,  5,  4, blk32[25], VALIDUS_72);
  VC_1(c, b, a, f, e, d, 13, 14, blk32[27], VALIDUS_73);
  VC_1(b, a, f, e, d, c, 17, 16, blk32[29], VALIDUS_74);
  VC_1(a, f, e, d, c, b, 22, 19, blk32[24], VALIDUS_75);
  VC_1(f, e, d, c, b, a, 26, 23, blk32[26], VALIDUS_76);
  VC_1(e, d, c, b, a, f, 28, 29, blk32[28], VALIDUS_77);
  VC_1(d, c, b, a, f, e,  5,  4, blk32[31], VALIDUS_78);
  VC_1(c, b, a, f, e, d, 13, 14, blk32[33], VALIDUS_79);
  VC_1(b, a, f, e, d, c, 17, 16, blk32[35], VALIDUS_80);
  VC_1(a, f, e, d, c, b, 22, 19, blk32[30], VALIDUS_81);
  VC_1(f, e, d, c, b, a, 26, 23, blk32[32], VALIDUS_82);
  VC_1(e, d, c, b, a, f, 28, 29, blk32[34], VALIDUS_83);
  VC_1(d, c, b, a, f, e,  5,  4, blk32[37], VALIDUS_84);
  VC_1(c, b, a, f, e, d, 13, 14, blk32[39], VALIDUS_85);
  VC_1(b, a, f, e, d, c, 17, 16, blk32[41], VALIDUS_86);
  VC_1(a, f, e, d, c, b, 22, 19, blk32[36], VALIDUS_87);
  VC_1(f, e, d, c, b, a, 26, 23, blk32[38], VALIDUS_88);
  VC_1(e, d, c, b, a, f, 28, 29, blk32[40], VALIDUS_89);
  VC_1(d, c, b, a, f, e,  5,  4, blk32[43], VALIDUS_90);
  VC_1(c, b, a, f, e, d, 13, 14, blk32[45], VALIDUS_91);
  VC_1(b, a, f, e, d, c, 17, 16, blk32[47], VALIDUS_92);
  VC_1(a, f, e, d, c, b, 22, 19, blk32[42], VALIDUS_93);
  VC_1(f, e, d, c, b, a, 26, 23, blk32[44], VALIDUS_94);
  VC_1(e, d, c, b, a, f, 28, 29, blk32[46], VALIDUS_95);

  VC_2(d, c, b, a, f, e,  3,  2, blk32[ 1], VALIDUS_96);
  VC_2(c, b, a, f, e, d,  6,  7, blk32[ 0], VALIDUS_97);
  VC_2(b, a, f, e, d, c,  9, 10, blk32[ 3], VALIDUS_98);
  VC_2(a, f, e, d, c, b, 12, 15, blk32[ 2], VALIDUS_99);
  VC_2(f, e, d, c, b, a, 21, 20, blk32[ 5], VALIDUS_100);
  VC_2(e, d, c, b, a, f, 24, 25, blk32[ 4], VALIDUS_101);
  VC_2(d, c, b, a, f, e,  3,  2, blk32[ 7], VALIDUS_102);
  VC_2(c, b, a, f, e, d,  6,  7, blk32[ 6], VALIDUS_103);
  VC_2(b, a, f, e, d, c,  9, 10, blk32[ 9], VALIDUS_104);
  VC_2(a, f, e, d, c, b, 12, 15, blk32[ 8], VALIDUS_105);
  VC_2(f, e, d, c, b, a, 21, 20, blk32[11], VALIDUS_106);
  VC_2(e, d, c, b, a, f, 24, 25, blk32[10], VALIDUS_107);
  VC_2(d, c, b, a, f, e,  3,  2, blk32[13], VALIDUS_108);
  VC_2(c, b, a, f, e, d,  6,  7, blk32[12], VALIDUS_109);
  VC_2(b, a, f, e, d, c,  9, 10, blk32[15], VALIDUS_110);
  VC_2(a, f, e, d, c, b, 12, 15, blk32[14], VALIDUS_111);
  VC_2(f, e, d, c, b, a, 21, 20, blk32[17], VALIDUS_112);
  VC_2(e, d, c, b, a, f, 24, 25, blk32[16], VALIDUS_113);
  VC_2(d, c, b, a, f, e,  3,  2, blk32[19], VALIDUS_114);
  VC_2(c, b, a, f, e, d,  6,  7, blk32[18], VALIDUS_115);
  VC_2(b, a, f, e, d, c,  9, 10, blk32[21], VALIDUS_116);
  VC_2(a, f, e, d, c, b, 12, 15, blk32[20], VALIDUS_117);
  VC_2(f, e, d, c, b, a, 21, 20, blk32[23], VALIDUS_118);
  VC_2(e, d, c, b, a, f, 24, 25, blk32[22], VALIDUS_119);
  VC_2(d, c, b, a, f, e,  3,  2, blk32[46], VALIDUS_120);
  VC_2(c, b, a, f, e, d,  6,  7, blk32[47], VALIDUS_121);
  VC_2(b, a, f, e, d, c,  9, 10, blk32[44], VALIDUS_122);
  VC_2(a, f, e, d, c, b, 12, 15, blk32[45], VALIDUS_123);
  VC_2(f, e, d, c, b, a, 21, 20, blk32[42], VALIDUS_124);
  VC_2(e, d, c, b, a, f, 24, 25, blk32[43], VALIDUS_125);
  VC_2(d, c, b, a, f, e,  3,  2, blk32[40], VALIDUS_126);
  VC_2(c, b, a, f, e, d,  6,  7, blk32[41], VALIDUS_127);
  VC_2(b, a, f, e, d, c,  9, 10, blk32[38], VALIDUS_128);
  VC_2(a, f, e, d, c, b, 12, 15, blk32[39], VALIDUS_129);
  VC_2(f, e, d, c, b, a, 21, 20, blk32[36], VALIDUS_130);
  VC_2(e, d, c, b, a, f, 24, 25, blk32[37], VALIDUS_131);
  VC_2(d, c, b, a, f, e,  3,  2, blk32[34], VALIDUS_132);
  VC_2(c, b, a, f, e, d,  6,  7, blk32[35], VALIDUS_133);
  VC_2(b, a, f, e, d, c,  9, 10, blk32[32], VALIDUS_134);
  VC_2(a, f, e, d, c, b, 12, 15, blk32[33], VALIDUS_135);
  VC_2(f, e, d, c, b, a, 21, 20, blk32[30], VALIDUS_136);
  VC_2(e, d, c, b, a, f, 24, 25, blk32[31], VALIDUS_137);
  VC_2(d, c, b, a, f, e,  3,  2, blk32[28], VALIDUS_138);
  VC_2(c, b, a, f, e, d,  6,  7, blk32[29], VALIDUS_139);
  VC_2(b, a, f, e, d, c,  9, 10, blk32[26], VALIDUS_140);
  VC_2(a, f, e, d, c, b, 12, 15, blk32[27], VALIDUS_141);
  VC_2(f, e, d, c, b, a, 21, 20, blk32[24], VALIDUS_142);
  VC_2(e, d, c, b, a, f, 24, 25, blk32[25], VALIDUS_143);

  VC_3(d, c, b, a, f, e,  4,  5, blk32[24], VALIDUS_144);
  VC_3(c, b, a, f, e, d, 14, 13, blk32[26], VALIDUS_145);
  VC_3(b, a, f, e, d, c, 16, 17, blk32[28], VALIDUS_146);
  VC_3(a, f, e, d, c, b, 19, 22, blk32[25], VALIDUS_147);
  VC_3(f, e, d, c, b, a, 23, 26, blk32[27], VALIDUS_148);
  VC_3(e, d, c, b, a, f, 29, 28, blk32[29], VALIDUS_149);
  VC_3(d, c, b, a, f, e,  4,  5, blk32[30], VALIDUS_150);
  VC_3(c, b, a, f, e, d, 14, 13, blk32[32], VALIDUS_151);
  VC_3(b, a, f, e, d, c, 16, 17, blk32[34], VALIDUS_152);
  VC_3(a, f, e, d, c, b, 19, 22, blk32[31], VALIDUS_153);
  VC_3(f, e, d, c, b, a, 23, 26, blk32[33], VALIDUS_154);
  VC_3(e, d, c, b, a, f, 29, 28, blk32[35], VALIDUS_155);
  VC_3(d, c, b, a, f, e,  4,  5, blk32[36], VALIDUS_156);
  VC_3(c, b, a, f, e, d, 14, 13, blk32[38], VALIDUS_157);
  VC_3(b, a, f, e, d, c, 16, 17, blk32[40], VALIDUS_158);
  VC_3(a, f, e, d, c, b, 19, 22, blk32[37], VALIDUS_159);
  VC_3(f, e, d, c, b, a, 23, 26, blk32[39], VALIDUS_160);
  VC_3(e, d, c, b, a, f, 29, 28, blk32[41], VALIDUS_161);
  VC_3(d, c, b, a, f, e,  4,  5, blk32[42], VALIDUS_162);
  VC_3(c, b, a, f, e, d, 14, 13, blk32[44], VALIDUS_163);
  VC_3(b, a, f, e, d, c, 16, 17, blk32[46], VALIDUS_164);
  VC_3(a, f, e, d, c, b, 19, 22, blk32[43], VALIDUS_165);
  VC_3(f, e, d, c, b, a, 23, 26, blk32[45], VALIDUS_166);
  VC_3(e, d, c, b, a, f, 29, 28, blk32[47], VALIDUS_167);
  VC_3(d, c, b, a, f, e,  4,  5, blk32[23], VALIDUS_168);
  VC_3(c, b, a, f, e, d, 14, 13, blk32[21], VALIDUS_169);
  VC_3(b, a, f, e, d, c, 16, 17, blk32[19], VALIDUS_170);
  VC_3(a, f, e, d, c, b, 19, 22, blk32[22], VALIDUS_171);
  VC_3(f, e, d, c, b, a, 23, 26, blk32[20], VALIDUS_172);
  VC_3(e, d, c, b, a, f, 29, 28, blk32[18], VALIDUS_173);
  VC_3(d, c, b, a, f, e,  4,  5, blk32[17], VALIDUS_174);
  VC_3(c, b, a, f, e, d, 14, 13, blk32[15], VALIDUS_175);
  VC_3(b, a, f, e, d, c, 16, 17, blk32[13], VALIDUS_176);
  VC_3(a, f, e, d, c, b, 19, 22, blk32[16], VALIDUS_177);
  VC_3(f, e, d, c, b, a, 23, 26, blk32[14], VALIDUS_178);
  VC_3(e, d, c, b, a, f, 29, 28, blk32[12], VALIDUS_179);
  VC_3(d, c, b, a, f, e,  4,  5, blk32[11], VALIDUS_180);
  VC_3(c, b, a, f, e, d, 14, 13, blk32[ 9], VALIDUS_181);
  VC_3(b, a, f, e, d, c, 16, 17, blk32[ 7], VALIDUS_182);
  VC_3(a, f, e, d, c, b, 19, 22, blk32[10], VALIDUS_183);
  VC_3(f, e, d, c, b, a, 23, 26, blk32[ 8], VALIDUS_184);
  VC_3(e, d, c, b, a, f, 29, 28, blk32[ 6], VALIDUS_185);
  VC_3(d, c, b, a, f, e,  4,  5, blk32[ 5], VALIDUS_186);
  VC_3(c, b, a, f, e, d, 14, 13, blk32[ 3], VALIDUS_187);
  VC_3(b, a, f, e, d, c, 16, 17, blk32[ 1], VALIDUS_188);
  VC_3(a, f, e, d, c, b, 19, 22, blk32[ 4], VALIDUS_189);
  VC_3(f, e, d, c, b, a, 23, 26, blk32[ 2], VALIDUS_190);
  VC_3(e, d, c, b, a, f, 29, 28, blk32[ 0], VALIDUS_191);

  pstate->f0 += a;
  pstate->f1 += b;
  pstate->f2 += c;
  pstate->f3 += d;
  pstate->f4 += e;
  pstate->f5 += f;
}

/*
 * FUNCTION: Validus_Init()
 *
 * SYNOPSIS: Initializes the Validus state.
 * Must be called before calls to Validus_Append()
 *
 * ARGUMENTS: Pointer to the Validus state to initialize
 */
void Validus_Init(validus_t *pstate) {

  pstate->bits[0] = pstate->bits[1] = 0;
  pstate->f0 = VALIDUS_INIT_0;
  pstate->f1 = VALIDUS_INIT_1;
  pstate->f2 = VALIDUS_INIT_2;
  pstate->f3 = VALIDUS_INIT_3;
  pstate->f4 = VALIDUS_INIT_4;
  pstate->f5 = VALIDUS_INIT_5;

}

/*
 *FUNCTION: Validus_Finish()
 *
 * SYNOPSIS: Finalizes the Validus operation.
 * Must be called after the last call to Validus_Append()
 *
 * ARGUMENTS: Pointer to the Validus state to finalize
 */
void Validus_Finish(validus_t *pstate) {

  validus_octet_t finish[192] = {
    0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  OCTETSWAP(pstate->bits[1], ((validus_octet_t *)&pstate->bits[1]));
  OCTETSWAP(pstate->bits[0], ((validus_octet_t *)&pstate->bits[0]));

  memcpy(&finish[184], &pstate->bits[1], 4);
  memcpy(&finish[188], &pstate->bits[0], 4);

  _Validus_Process(pstate, (validus_word_t *)finish);

}
