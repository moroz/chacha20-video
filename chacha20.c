#include "chacha20.h"
#include <stdio.h>
#include <string.h>

static inline uint32_t ROTL32(uint32_t n, int offset) {
  return (n << offset) | (n >> (32 - offset));
}

void chacha20_qround(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {
  *a += *b;
  *d ^= *a;
  *d = ROTL32(*d, 16);
  *c += *d;
  *b ^= *c;
  *b = ROTL32(*b, 12);
  *a += *b;
  *d ^= *a;
  *d = ROTL32(*d, 8);
  *c += *d;
  *b ^= *c;
  *b = ROTL32(*b, 7);
}

inline void QROUND(Chacha20 *state, int a, int b, int c, int d) {
  chacha20_qround(&state->words[a], &state->words[b], &state->words[c],
                  &state->words[d]);
}

static uint32_t get_little_endian_word(const uint8_t *vec) {
  return (vec[3] << 24) | (vec[2] << 16) | (vec[1] << 8) | vec[0];
}

void chacha20_setup_block(Chacha20 *state, const uint8_t *key,
                          const uint8_t *nonce, uint32_t block_counter) {
  memcpy(state->bytes, "expand 32-byte k", 16);
  for (int i = 0; i < 8; i++) {
    state->words[i + 4] = get_little_endian_word(&key[i * 4]);
  }
  for (int i = 0; i < 3; i++) {
    state->words[i + 13] = get_little_endian_word(&nonce[i * 4]);
  }
  state->words[12] = block_counter;
}

static void chacha20_inner_block(Chacha20 *state) {
  QROUND(state, 0, 4, 8, 12);
  QROUND(state, 1, 5, 9, 13);
  QROUND(state, 2, 6, 10, 14);
  QROUND(state, 3, 7, 11, 15);
  QROUND(state, 0, 5, 10, 15);
  QROUND(state, 1, 6, 11, 12);
  QROUND(state, 2, 7, 8, 13);
  QROUND(state, 3, 4, 9, 14);
}

void chacha20_block(Chacha20 *state, const uint8_t *key, const uint8_t *nonce,
                    uint32_t counter) {
  chacha20_setup_block(state, key, nonce, counter);

  uint32_t initialState[16];
  memcpy(initialState, state->words, 16 * sizeof(uint32_t));

  for (int i = 0; i < 10; i++) {
    chacha20_inner_block(state);
  }

  for (int i = 0; i < 16; i++) {
    state->words[i] += initialState[i];
  }
}

void chacha20_encrypt(const uint8_t *key, const uint32_t counter,
                      const uint8_t *nonce, const uint8_t *plaintext,
                      const size_t len, uint8_t *dst) {
  Chacha20 state;

  for (int j = 0, n = len / 64; j < n; j++) {
    chacha20_block(&state, key, nonce, counter + j);
    for (int k = 0; k < 64; k++) {
      dst[j * 64 + k] = plaintext[j * 64 + k] ^ state.bytes[k];
    }
  }

  if (len % 64 != 0) {
    int j = len / 64;
    chacha20_block(&state, key, nonce, counter + j);

    for (int k = 0, n = len - j * 64; k < n; k++) {
      dst[j * 64 + k] = plaintext[j * 64 + k] ^ state.bytes[k];
    }
  }
}
