#ifndef TROWEL_JOYSTICK_HPP_INCLUDED
#define TROWEL_JOYSTICK_HPP_INCLUDED

#include <cstdint>
#include <utility>
#include "sm64/ultra_types.hpp"
namespace trowel {

  inline constexpr int16_t hau_round(int16_t theta) {
    return theta & ~UINT16_C(0x000F);
  }

  // Checks if two angles are HAU-equivalent.
  inline constexpr bool is_hau_equal(int16_t p, int16_t q) {
    return ((uint16_t) p >> 4) == ((uint16_t) q >> 4);
  }

  // For find_input_by_ym_*, the direction(s) to search for a usable stick angle.
  enum class ym_search_dir {
    // Searches in both directions from the desired yaw.
    both,
    // Searches only clockwise from the desired yaw.
    cw,
    // Searches only counterclockwise from the desired yaw.
    ccw
  };

  // Computes the resulting yaw and magnitude from the given input.
  std::pair<int16_t, float> input_to_ym(int8_t x, int8_t y, int16_t camera_yaw);

  // Finds the closest input stick angle for the given yaw (to HAU precision) and magnitude, based on the camera yaw.
  std::pair<int8_t, int8_t> find_input_by_ym_hau(int16_t intended_yaw, float intended_mag, int16_t camera_yaw, ym_search_dir search_dir = ym_search_dir::both);
  // Finds the closest input stick angle for the given yaw and magnitude, based on the camera yaw.
  std::pair<int8_t, int8_t> find_input_by_ym_exact(int16_t intended_yaw, float intended_mag, int16_t camera_yaw, ym_search_dir search_dir = ym_search_dir::both);

  
}

#endif