#ifndef CHACHA20_H
#define CHACHA20_H

#include <stdint.h>

// chacha20_qround is the concrete implementation of the Chacha20 qround
void chacha20_qround(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d);

// Chacha20 defines the Chacha20 "vector" or "matrix", with 16 32-bit
// unsigned integers.
typedef struct {
  uint32_t words[16];
} Chacha20;

void QROUND(Chacha20 *state, int a, int b, int c, int d);

#endif
