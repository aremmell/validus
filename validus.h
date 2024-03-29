/**
 * @file validus.h
 * @brief Definitions for the Validus hash function.
 *
 * Configures the build, defines types, macros, constants, and functions
 * that comprise the core of Validus.
 *
 * @author    Ryan M. Lederman \<lederman@gmail.com\>
 * @date      2004-2023
 * @version   1.0.4
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
#ifndef _VALIDUS_H_INCLUDED
# define _VALIDUS_H_INCLUDED

///////////////////////////// preprocessor /////////////////////////////////////

# if defined(__linux__)  || defined(__GNU__)    || defined(__CYGWIN__) || \
    defined(__GLIBC__)  || defined(__HAIKU__)  || defined(__APPLE__)
#  if defined(__APPLE__)
#   include <machine/endian.h>
#  else
#   include <endian.h>
#  endif
#  if !defined(LITTLE_ENDIAN) && defined(__LITTLE_ENDIAN)
#   define LITTLE_ENDIAN __LITTLE_ENDIAN
#  endif
#  if !defined(BIG_ENDIAN) && defined(__BIG_ENDIAN)
#   define BIG_ENDIAN __BIG_ENDIAN
#  endif
#  if !defined(BYTE_ORDER) && defined(__BYTE_ORDER)
#   define BYTE_ORDER __BYTE_ORDER
#  endif
# endif

# if defined(__sun)
#  include <sys/byteorder.h>
#  define LITTLE_ENDIAN 1234
#  define BIG_ENDIAN    4321
#  if defined(_BIG_ENDIAN)
#   define BYTE_ORDER BIG_ENDIAN
#  elif defined(_LITTLE_ENDIAN)
#  else
#   error "failed to determine endian-ness of this Sun system."
#  endif
# endif

# if defined(_AIX) && !defined(BYTE_ORDER)
#  define LITTLE_ENDIAN 1234
#  define BIG_ENDIAN    4321
#  if defined(__BIG_ENDIAN__)
#   define BYTE_ORDER BIG_ENDIAN
#  elif defined(__LITTLE_ENDIAN__)
#   define BYTE_ORDER LITTLE_ENDIAN
#  else
#   error "failed to determine endian-ness of this IBM AIX system."
#  endif
# endif

# if defined(_WIN32)
#  define LITTLE_ENDIAN 1234
#  define BIG_ENDIAN    4321
#  define BYTE_ORDER LITTLE_ENDIAN
# endif

# if !defined(BYTE_ORDER) || !defined(LITTLE_ENDIAN) || !defined(BIG_ENDIAN)
#  error "failed to determine endian-ness of this system."
# endif

# if BYTE_ORDER == BIG_ENDIAN
#  define VALIDUS_BIG_ENDIAN
# endif

# if !defined(_DEFAULT_SOURCE)
# define _DEFAULT_SOURCE 1
# endif

# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>

/**
 * @defgroup core Core
 *
 * The heart of the Validus hash function.
 *
 * @addtogroup core
 * @{
 *
 * @defgroup corefuncs Functions and Types
 * @{
 */

/////////////////////////////// typedefs ///////////////////////////////////////

typedef uint8_t  validus_octet; /**< Validus byte (8-bit). */
typedef uint32_t validus_word;  /**< Validus word (32-bit). */

/**
 * @struct validus_state
 * @brief Represents the state of an individual Validus hash operation.
 *
 * Contains the working state of a hash operation (i.e. the fingerprint and
 * state data). Not valid for examination until ::validus_finalize is called.
 */
typedef struct {
    validus_word bits[2]; /**< 64-bit counter. */
    validus_word f0;      /**< Fingerprint word 0. */
    validus_word f1;      /**< Fingerprint word 1. */
    validus_word f2;      /**< Fingerprint word 2. */
    validus_word f3;      /**< Fingerprint word 3. */
    validus_word f4;      /**< Fingerprint word 4. */
    validus_word f5;      /**< Fingerprint word 5. */
} validus_state;

/////////////////////////// function exports ///////////////////////////////////

# if defined(__cplusplus)
extern "C" {
# endif

/**
 * @brief Initializes a validus_state object.
 *
 * Fills the `state` structure with initial values and prepares it for use
 * with ::validus_append, and ::validus_finalize.
 *
 * @note Must be called before any other Validus function that takes a
 * ::validus_state input argument.
 *
 * @note If `state` is NULL, this function will return early, and have no effect.
 *
 * @param state Pointer to the ::validus_state object to initialize.
 */
void validus_init(validus_state* state);

/**
 * @brief Processes a block of data, accumulating the results in
 * the validus_state object.
 *
 * @note If `state` or `data` are NULL, or `len` is zero, this function will
 * return early, and have no effect.
 *
 * @param state Pointer to the validus_state object in use for this series of data.
 * @param data  Pointer to a new block of data to process.
 * @param len   Length of `data` in octets.
 */
void validus_append(validus_state* state, const void* data, size_t len);

/**
 * @brief Finalizes a Validus hashing operation.
 *
 * @note Must be called after the final call to ::validus_append.
 * @note If `state` is NULL, this function will return early and have no effect.
 *
 * @param state Pointer to the validus_state object to finalize.
 */
void validus_finalize(validus_state* state);

/**
 * @brief Compares two validus_state objects for equality.
 *
 * @param   one  Pointer to a validus_state to compare for equality.
 * @param   two  Pointer to a validus_state to compare for equality.
 * @returns bool `true` if both states are identical, `false` otherwise.
 */
bool validus_compare(const validus_state *one, const validus_state *two);

/**
 * @brief Processes a 192-bit block of data, accumulating the results
 * in the validus_state object.
 *
 * @attention This function is only called by other Validus functions; do not
 * call it directly.
 *
 * @param state Pointer to the validus_state object in use for this series of data.
 * @param blk32 Pointer to the block of data to be processed.
 */
void _validus_process(validus_state* state, const validus_word* blk32);

# if defined(__cplusplus)
}
# endif

/**
 * @}
 * @}
 */

////////////////////////////////// macros //////////////////////////////////////

/**
 * @addtogroup core
 * @{
 *
 * @defgroup coremacros Macros
 * @{
 */

/** The size of a Validus fingerprint, in bits. */
# define VALIDUS_FP_SIZE_B 192

/** The size of a Validus fingerprint, in octets. */
# define VALIDUS_FP_SIZE_O 48

/** Determines if address `addr` is aligned on 4-byte boundaries. */
# define WORDALIGNED(addr) (((uintptr_t)(addr) & 0x3) == 0)

/** Swaps octet order of a 32-bit value `b` and stores the result in `a`. */
# define OCTETSWAP(a, b) (a = ((b[3] << 24) | (b[2] << 16) | (b[1] << 8) | (b[0])))

/** Cyclically rotates word `a` by `b` bits to the left. */
# define ROL(a, b) (((a) << (b)) | ((a) >> (32 - b)))

/** Cyclically rotates word `a` by `b` bits to the right. */
# define ROR(a, b) (((a) >> (b)) | ((a) << (32 - b)))

/** Mixer function 0. */
# define M0(a, b, c, d, e) ((((a) & (b)) ^ (((c) & (d)) ^ (e))))

/** Mixer function 1. */
# define M1(a, b, c, d, e) ((((a) & (b)) ^ ((b) ^ ((c) & (d)) ^ (e))))

/** Mixer function 2. */
# define M2(a, b, c, d, e) ((((a) & ((b) ^ (c))) ^ (~(d) & (e)) ^ (c)))

/** Mixer function 3. */
# define M3(a, b, c, d, e) ((((a) & (b)) ^ ((c) & ((d) ^ (e))) ^ (e)))

/** Compression function 0. */
# define VC_0(a, b, c, d, e, f, r1, r2, blk, hcv)       \
    do {                                                \
        register validus_word t;                        \
        t = a + M0(b, c, d, e, f) + ROL(blk + hcv, r1); \
        a = ROR(t + blk, r2);                           \
    } while (false)

/** Compression function 1. */
# define VC_1(a, b, c, d, e, f, r1, r2, blk, hcv)       \
    do {                                                \
        register validus_word t;                        \
        t = a + M1(b, c, d, e, f) + ROL(blk + hcv, r1); \
        a = ROR(t + blk, r2);                           \
    } while (false)

/** Compression function 2. */
# define VC_2(a, b, c, d, e, f, r1, r2, blk, hcv)       \
    do {                                                \
        register validus_word t;                        \
        t = a + M2(b, c, d, e, f) + ROL(blk + hcv, r1); \
        a = ROR(t + blk, r2);                           \
    } while (false)

/** Compression function 3. */
# define VC_3(a, b, c, d, e, f, r1, r2, blk, hcv)       \
    do {                                                \
        register validus_word t;                        \
        t = a + M3(b, c, d, e, f) + ROL(blk + hcv, r1); \
        a = ROR(t + blk, r2);                           \
    } while (false)

/* Initial state values. */
# define VALIDUS_INIT_0  0x81010881U  /**< 10000001000000010000100010000001 */
# define VALIDUS_INIT_1  0xA529298BU  /**< 10100101001010010010100110001011 */
# define VALIDUS_INIT_2  0x66AC654AU  /**< 01100110101011000110010101001010 */
# define VALIDUS_INIT_3  0x52865650U  /**< 01010010100001100101011001010000 */
# define VALIDUS_INIT_4  0x18529234U  /**< 00011000010100101001001000110100 */
# define VALIDUS_INIT_5  0x08508024U  /**< 00001000010100001000000000100100 */

/* Constants used in compression functions. */
# define VALIDUS_0   0x4528A03EU
# define VALIDUS_1   0xCABBB352U
# define VALIDUS_2   0x8147ED07U
# define VALIDUS_3   0xAC5567E6U
# define VALIDUS_4   0x244F0AE0U
# define VALIDUS_5   0x7AE6DC24U
# define VALIDUS_6   0x607FDE7AU
# define VALIDUS_7   0xED604141U
# define VALIDUS_8   0x8B601567U
# define VALIDUS_9   0x94994DDDU
# define VALIDUS_10  0x00910572U
# define VALIDUS_11  0x6C036810U
# define VALIDUS_12  0x74273753U
# define VALIDUS_13  0x11809AECU
# define VALIDUS_14  0x9EC28BE7U
# define VALIDUS_15  0x856B6A07U
# define VALIDUS_16  0xDCC772A0U
# define VALIDUS_17  0x54854204U
# define VALIDUS_18  0x7E8DDF77U
# define VALIDUS_19  0x343C0872U
# define VALIDUS_20  0xB9E401F3U
# define VALIDUS_21  0x8001489EU
# define VALIDUS_22  0xBBCC20ABU
# define VALIDUS_23  0x364B7F69U
# define VALIDUS_24  0x7EDFBBD5U
# define VALIDUS_25  0x52CE4099U
# define VALIDUS_26  0xDA9B320EU
# define VALIDUS_27  0x84C954BCU
# define VALIDUS_28  0xA03FA6A0U
# define VALIDUS_29  0x13BE82F2U
# define VALIDUS_30  0x7516470EU
# define VALIDUS_31  0x6AC7D4C0U
# define VALIDUS_32  0xFE4CF293U
# define VALIDUS_33  0x93620C9CU
# define VALIDUS_34  0x8C53D058U
# define VALIDUS_35  0xEF9EE2C6U
# define VALIDUS_36  0x61F9166FU
# define VALIDUS_37  0x7A3FDA68U
# define VALIDUS_38  0x22215B08U
# define VALIDUS_39  0xAAA1C60CU
# define VALIDUS_40  0x819EDDD8U
# define VALIDUS_41  0xCCCD47FEU
# define VALIDUS_42  0x470DF3E1U
# define VALIDUS_43  0x7FFADD92U
# define VALIDUS_44  0x433DC03CU
# define VALIDUS_45  0xC8AE64A3U
# define VALIDUS_46  0x80FB26C0U
# define VALIDUS_47  0xAE0FC014U
# define VALIDUS_48  0x2679D104U
# define VALIDUS_49  0x7B8401BEU
# define VALIDUS_50  0x5EFEE8A1U
# define VALIDUS_51  0xEB231E3CU
# define VALIDUS_52  0x8A75B5AEU
# define VALIDUS_53  0x95D92CE8U
# define VALIDUS_54  0x02D50CAFU
# define VALIDUS_55  0x6D365104U
# define VALIDUS_56  0x732ED60FU
# define VALIDUS_57  0x0F414B6FU
# define VALIDUS_58  0x9D4D3E47U
# define VALIDUS_59  0x861754DCU
# define VALIDUS_60  0xDEF6868EU
# define VALIDUS_61  0x56357B91U
# define VALIDUS_62  0x7E31DBF2U
# define VALIDUS_63  0x322860B2U
# define VALIDUS_64  0xB8018321U
# define VALIDUS_65  0x800B8D5CU
# define VALIDUS_66  0xBDB9B81FU
# define VALIDUS_67  0x38569B47U
# define VALIDUS_68  0x7F276A7BU
# define VALIDUS_69  0x51109A87U
# define VALIDUS_70  0xD871F179U
# define VALIDUS_71  0x843121FCU
# define VALIDUS_72  0xA1C46FE1U
# define VALIDUS_73  0x15FAD577U
# define VALIDUS_74  0x75FBF212U
# define VALIDUS_75  0x6983B065U
# define VALIDUS_76  0xFC09029AU
# define VALIDUS_77  0x9233821DU
# define VALIDUS_78  0x8D50D2F4U
# define VALIDUS_79  0xF1DED4B0U
# define VALIDUS_80  0x636A723DU
# define VALIDUS_81  0x798F09EEU
# define VALIDUS_82  0x1FF0EE3AU
# define VALIDUS_83  0xA8F4FD78U
# define VALIDUS_84  0x81FFF238U
# define VALIDUS_85  0xCEE2F829U
# define VALIDUS_86  0x48ED9435U
# define VALIDUS_87  0x7FEB76B7U
# define VALIDUS_88  0x414D7B3DU
# define VALIDUS_89  0xC6A58615U
# define VALIDUS_90  0x80B8912BU
# define VALIDUS_91  0xAFD0AB19U
# define VALIDUS_92  0x28A180F1U
# define VALIDUS_93  0x7C173E98U
# define VALIDUS_94  0x5D7653C5U
# define VALIDUS_95  0xE8E7A7B1U
# define VALIDUS_96  0x8994C3FBU
# define VALIDUS_97  0x97219014U
# define VALIDUS_98  0x0518D9C1U
# define VALIDUS_99  0x6E6076FCU
# define VALIDUS_100 0x722D372EU
# define VALIDUS_101 0x0D00C2A5U
# define VALIDUS_102 0x9BDFDBB4U
# define VALIDUS_103 0x86CD0770U
# define VALIDUS_104 0xE12840FFU
# define VALIDUS_105 0x57DECA91U
# define VALIDUS_106 0x7DCBB8A9U
# define VALIDUS_107 0x3010B2D0U
# define VALIDUS_108 0xB624CAE9U
# define VALIDUS_109 0x80201605U
# define VALIDUS_110 0xBFACA0B5U
# define VALIDUS_111 0x3A5D3213U
# define VALIDUS_112 0x7F64E5A9U
# define VALIDUS_113 0x4F4C7390U
# define VALIDUS_114 0xD64BDD44U
# define VALIDUS_115 0x83A2DDFCU
# define VALIDUS_116 0xA350C878U
# define VALIDUS_117 0x18356491U
# define VALIDUS_118 0x76D825F1U
# define VALIDUS_119 0x683714FFU
# define VALIDUS_120 0xF9C56410U
# define VALIDUS_121 0x910DC6A7U
# define VALIDUS_122 0x8E5708EEU
# define VALIDUS_123 0xF41FE8CAU
# define VALIDUS_124 0x64D3D441U
# define VALIDUS_125 0x78D478E5U
# define VALIDUS_126 0x1DBDF16BU
# define VALIDUS_127 0xA74F3092U
# define VALIDUS_128 0x826B225FU
# define VALIDUS_129 0xD0FC9903U
# define VALIDUS_130 0x4AC75AC1U
# define VALIDUS_131 0x7FD1CCAAU
# define VALIDUS_132 0x3F57F910U
# define VALIDUS_133 0xC4A14172U
# define VALIDUS_134 0x808031A0U
# define VALIDUS_135 0xB19804F1U
# define VALIDUS_136 0x2AC5EE66U
# define VALIDUS_137 0x7CA086E3U
# define VALIDUS_138 0x5BE63F66U
# define VALIDUS_139 0xE6AE0B79U
# define VALIDUS_140 0x88BD5259U
# define VALIDUS_141 0x98725D07U
# define VALIDUS_142 0x075C3E24U
# define VALIDUS_143 0x6F81C20DU
# define VALIDUS_144 0x71226F5DU
# define VALIDUS_145 0x0ABF2ECDU
# define VALIDUS_146 0x9A7A817EU
# define VALIDUS_147 0x878C732EU
# define VALIDUS_148 0xE35C74E1U
# define VALIDUS_149 0x59810CE8U
# define VALIDUS_150 0x7D5B7DCCU
# define VALIDUS_151 0x2DF529C6U
# define VALIDUS_152 0xB44DFF8AU
# define VALIDUS_153 0x803EE0F3U
# define VALIDUS_154 0xC1A4B268U
# define VALIDUS_155 0x3C5F1A33U
# define VALIDUS_156 0x7F982871U
# define VALIDUS_157 0x4D81EFF9U
# define VALIDUS_158 0xD429218FU
# define VALIDUS_159 0x831E9426U
# define VALIDUS_160 0xA4E49098U
# define VALIDUS_161 0x1A6E027AU
# define VALIDUS_162 0x77AAD103U
# define VALIDUS_163 0x66E21D3FU
# define VALIDUS_164 0xF7824573U
# define VALIDUS_165 0x8FF0F1C9U
# define VALIDUS_166 0x8F665D3EU
# define VALIDUS_167 0xF661F0CAU
# define VALIDUS_168 0x66351F7DU
# define VALIDUS_169 0x78103647U
# define VALIDUS_170 0x1B8891C7U
# define VALIDUS_171 0xA5B08130U
# define VALIDUS_172 0x82E065B1U
# define VALIDUS_173 0xD319FF6AU
# define VALIDUS_174 0x4C9B2181U
# define VALIDUS_175 0x7FADE179U
# define VALIDUS_176 0x3D5D61F3U
# define VALIDUS_177 0xC2A1C024U
# define VALIDUS_178 0x80520CA3U
# define VALIDUS_179 0xB365A915U
# define VALIDUS_180 0x2CE6ED63U
# define VALIDUS_181 0x7D1FCF9CU
# define VALIDUS_182 0x5A4ECB9DU
# define VALIDUS_183 0xE4767747U
# define VALIDUS_184 0x87EF7212U
# define VALIDUS_185 0x99CB78BDU
# define VALIDUS_186 0x099F0B5DU
# define VALIDUS_187 0x709A1B02U
# define VALIDUS_188 0x700E9401U
# define VALIDUS_189 0x087CBE3DU
# define VALIDUS_190 0x991D4C51U
# define VALIDUS_191 0x885588BDU

/**
 * @}
 * @}
 */

#endif /* !_VALIDUS_H_INCLUDED */
