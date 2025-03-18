#include "chacha20.h"

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
