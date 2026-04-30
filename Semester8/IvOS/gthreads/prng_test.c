#include <stdio.h>
#include <stdint.h>

static uint32_t g_prng_state = 0x12345678u;

static uint32_t gt_random_u32(void) {
  uint32_t value = g_prng_state;

  value ^= value << 13;
  value ^= value >> 17;
  value ^= value << 5;
  if (value == 0)
    value = 0x12345678u;
  g_prng_state = value;
  return value;
}

int main() {
    for (int i = 0; i < 20; i++) {
        uint32_t val = gt_random_u32();
        printf("%u mod 31 = %u\n", val, val % 31);
    }
    return 0;
}
