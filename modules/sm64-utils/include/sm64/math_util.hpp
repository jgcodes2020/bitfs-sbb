#ifndef SM64_MATH_UTIL_HPP_INCLUDED
#define SM64_MATH_UTIL_HPP_INCLUDED

#include <cstdint>
namespace sm64 {
  // The full sine/cosine table, containing 5/4 turns of lookup values.
  extern const float g_sine_table[];
  // The full arctan table.
  extern const int16_t g_arctan_table[];

  // Computes the sine of an angle.
  // Should function identically to decomp.
  inline float sins(int16_t angle) {
    return g_sine_table[(uint16_t) angle >> 4];
  }
  // Computes the cosine of an angle.
  // Should function identically to decomp.
  inline float coss(int16_t angle) {
    return (g_sine_table + 1024)[((uint16_t) angle >> 4)];
  }

  // Computes the angle (clockwise of +y) of the ray from (0, 0) to (x, y). 
  // Should function identically to decomp.
  int16_t atan2s(float y, float x);
}

#endif