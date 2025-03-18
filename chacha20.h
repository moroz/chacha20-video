#ifndef CHACHA20_H
#define CHACHA20_H

#include <stdint.h>

void chacha20_qround(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d);

#endif
