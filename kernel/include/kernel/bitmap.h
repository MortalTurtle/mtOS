#pragma once
#include <stdint.h>
#include <string.h>

template <uint64_t SIZE>
class bitmap {
  uint8_t bitmap_[SIZE];
  const uint64_t bit_size = SIZE * 8;

 public:
  bitmap() { memset(bitmap_, 0, SIZE); }
  void bitmap_clear(uint64_t index) {
    if (index >= bit_size) return;
    uint64_t byte = index / 8;
    uint64_t bit = index % 8;
    bitmap_[byte] &= ~(1 << bit);
  }

  void bitmap_set(uint64_t index) {
    if (index >= bit_size) return;
    uint64_t byte = index / 8;
    uint64_t bit = index % 8;
    bitmap_[byte] |= (1 << bit);
  }

  int bitmap_test(uint64_t index) const {
    if (index >= bit_size) return 1;
    uint64_t byte = index / 8;
    uint64_t bit = index % 8;
    return bitmap_[byte] & (1 << bit);
  }
};
