#include "chacha20.h"
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
  memcpy(state->words, "expand 32-byte k", 16);
  for (int i = 0; i < 8; i++) {
    state->words[i + 4] = get_little_endian_word(&key[i * 4]);
  }
  for (int i = 0; i < 3; i++) {
    state->words[i + 13] = get_little_endian_word(&nonce[i * 4]);
  }
  state->words[12] = block_counter;
}
