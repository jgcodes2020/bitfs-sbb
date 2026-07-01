#ifndef TROWEL_JOYSTICK_HPP_INCLUDED
#define TROWEL_JOYSTICK_HPP_INCLUDED

#include <cstdint>
#include <utility>
#include "sm64/ultra_types.hpp"
namespace trowel {
  // Finds the closest input stick angle (within the same HAU) for the given yaw and magnitude, based on the camera yaw.
  std::pair<int8_t, int8_t> find_input_by_ym_hau(int16_t intended_yaw, float intended_mag, int16_t camera_yaw);
  // Finds the closest input stick angle for the given yaw and magnitude, based on the camera yaw.
  std::pair<int8_t, int8_t> find_input_by_ym_exact(int16_t intended_yaw, float intended_mag, int16_t camera_yaw);

  // Computes the resulting yaw and magnitude from the given input.
  std::pair<int16_t, float> input_to_ym(int8_t x, int8_t y, int16_t camera_yaw);

  // Checks if the two angles are equal by HAU.
  inline constexpr bool is_hau_equal(int16_t p, int16_t q) {
    return ((uint16_t) p >> 4) == ((uint16_t) q >> 4);
  }

  
}

#endif