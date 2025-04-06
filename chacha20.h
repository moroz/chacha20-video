#ifndef CHACHA20_H
#define CHACHA20_H

#include <stddef.h>
#include <stdint.h>

// chacha20_qround is the concrete implementation of the Chacha20 qround
void chacha20_qround(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d);

// Chacha20 defines the Chacha20 "vector" or "matrix", with 16 32-bit
// unsigned integers.
typedef union {
  uint32_t words[16];
  uint8_t bytes[64];
} Chacha20;

void QROUND(Chacha20 *state, int a, int b, int c, int d);

void chacha20_setup_block(Chacha20 *state, const uint8_t *key,
                          const uint8_t *nonce, uint32_t block_counter);

void chacha20_block(Chacha20 *state, const uint8_t *key, const uint8_t *nonce,
                    uint32_t counter);

// chacha20_encrypt encrypts `len` bytes of the the provided `plaintext` using
// the Chacha20 stream cipher. It starts counting blocks at `counter`, uses the
// given `key` for encryption, and the provided 96-bit `nonce` as the
// initialization vector.
//
// `dst` points to a byte array at least `len` bytes long, in which the
// ciphertext will be stored.
void chacha20_encrypt(const uint8_t *key, const uint32_t counter,
                      const uint8_t *nonce, const uint8_t *plaintext,
                      const size_t len, uint8_t *dst);

#endif
