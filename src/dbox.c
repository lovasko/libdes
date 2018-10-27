// Copyright (c) 2018 Daniel Lovasko.
// All Rights Reserved
//
// Distributed under the terms of the 2-clause BSD License. The full
// license is in the file LICENSE, distributed as part of this software.

#include "impl.h"


static const uint8_t initial_perm[64] = {
  57, 49, 41, 33, 25, 17,  9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
  61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7,
  56, 48, 40, 32, 24, 16,  8, 0, 58, 50, 42, 34, 26, 18, 10, 2,
  60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6
};

static const uint8_t final_perm[64] = {
  39, 7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30,
  37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12, 52, 20, 60, 28,
  35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 26,
  33, 1, 41,  9, 49, 17, 57, 25, 32, 0, 40,  8, 48, 16, 56, 24
};

/** Perform a dispersion on a data block.
 *
 * @param[in] blk data block
 * @param[in] box dispersion definition
 * @param[in] len length of the definition
 *
 * @return block after dispersion
**/
static uint64_t
dbox(const uint64_t blk, const uint8_t* box, const uint8_t len)
{
  uint64_t res;
  uint64_t bit;
  uint8_t i;

  res = 0;
  for (i = 0; i < len; i++) {
    bit = blk & (1 << (box[i]));
    res |= bit >> (box[i] - i);
  }

  return res;
}

/** Perform the initial permutation on a data block.
 *
 * Composing this function with "final_perm" must yield identity.
 *
 * @param[in] blk data block
 * @return permutated data block
**/
uint64_t
apply_initial_perm(const uint64_t blk)
{
  return dbox(blk, initial_perm, sizeof(initial_perm));
}

/** Perform the final permutation on a data block.
 *
 * Composing this function with "initial_perm" must yield identity.
 *
 * @param[in] blk data block
 * @return permutated data block
**/
uint64_t
apply_final_perm(const uint64_t blk)
{
  return dbox(blk, final_perm, 64);
}
