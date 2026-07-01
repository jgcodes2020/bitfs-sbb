#include "sm64/math_util.hpp"

namespace sm64 {
  static inline int16_t atan2_lookup(float y, float x) {
    return (x == 0) ? g_arctan_table[0]
                    : g_arctan_table[(int32_t) (y / x * 1024 + 0.5f)];
  }

  int16_t atan2s_game(float y, float x) {
    uint16_t ret;

    if (x >= 0) {
      if (y >= 0) {
        if (y >= x) {
          ret = atan2_lookup(x, y);
        }
        else {
          ret = 0x4000 - atan2_lookup(y, x);
        }
      }
      else {
        y = -y;
        if (y < x) {
          ret = 0x4000 + atan2_lookup(y, x);
        }
        else {
          ret = 0x8000 - atan2_lookup(x, y);
        }
      }
    }
    else {
      x = -x;
      if (y < 0) {
        y = -y;
        if (y >= x) {
          ret = 0x8000 + atan2_lookup(x, y);
        }
        else {
          ret = 0xC000 - atan2_lookup(y, x);
        }
      }
      else {
        if (y < x) {
          ret = 0xC000 + atan2_lookup(y, x);
        }
        else {
          ret = -atan2_lookup(x, y);
        }
      }
    }
    return (int16_t) ret;
  }

  int16_t atan2s(float y, float x) {
    const bool flip_x = x < 0;
    const bool flip_y = y < 0;

    // reduce coordinates to quadrant 1
    if (flip_x) x = -x;
    if (flip_y) y = -y;

    // lookup in quadrant 1
    uint16_t theta = (y >= x)? atan2_lookup(x, y) : 0x4000 - atan2_lookup(y, x);

    // unfold angle back out
    if (flip_y) theta = 0x8000 - theta;
    if (flip_x) theta = -theta;

    return (int16_t) theta;
  }
}  // namespace sm64