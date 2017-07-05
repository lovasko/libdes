#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "../src/des.h"


/** Compute the parity of a 8-bit unsigned integer.
 *
 * @param[in] byte 8-bit unsigned integer
 *
 * @return parity
 * @retval 0 even
 * @retval 1 odd
**/
static int
uint8_parity(uint8_t byte)
{
  int res;
  uint8_t x;

  res = 0;
  x = byte;

  while (x) {
    res = !res;
    x = x & (x - 1);
  }

  return res;
}


/** Add the parity bits for a selected key.
 *
 * @param[in] key 56-bit key without parity bits
 * @return 64-bit key with parity bits
**/
static uint64_t
add_parity(uint64_t key)
{
  uint64_t res;
  uint64_t mask;
  uint8_t byte;
  uint8_t i;
  uint8_t k;

  res = 0;
  mask = 0x7f; /* 0b1111111 */

  for (i = 0; i < 8; i++) {
    byte = key & (mask << (i * 7));
   
    if (uint8_parity(byte) == 0)
      byte |= 0x80;

    res |= (uint64_t)byte << (i * 8);
  }

  return res;
}

/** Test the assumption that the encryption and decryption functions form an
 * identity function under composition. */
int
main(int argc, char* argv[])
{
  uint64_t pln1;
  uint64_t pln2;
  uint64_t cip;
  long long i;
  long long max;
  int dr;

  /* Verify the command-line arguments. */
  if (argc != 2) {
    fprintf(stderr, "ERROR: expected exactly one argument\n");
    return EXIT_FAILURE;
  }

  /* Parse the number of test rounds. */
  max = strtoll(argv[1]);
  if (max == 0 && errno != 0) {
    perror("strtoll");
    return EXIT_FAILURE;
  }

  /* Create the source of randomness. */
  dr = open("/dev/random", O_RDONLY);
  if (dr < 0) {
    perror("open");
    return EXIT_FAILURE;
  }

  /* Run tests. */
  for (i = 0; i < max; i++) {
    read(dr, &key, 7);
    read(dr, &pln, 8);
    key = add_parity(key);

    des_encrypt(pln, key, &cip);
    des_decrypt(cip, key, &pln2);

    if (pln != pln2) {
      printf("MISMATCH: key %llu, pln1 %llu, pln2 %llu\n", key, pln1, pln2);
      return EXIT_FAILURE;
    }
  }

  close(dr);

  return EXIT_SUCCESS;
}
