#include "trowel/joystick.hpp"
#include <array>
#include <cmath>
#include <map>
#include <utility>
#include "sm64/math_util.hpp"

namespace trowel {
  namespace {
    inline int8_t adjust_stick(int8_t val) {
      float res = 0;
      if (val >= 8)
        res = val - 6;
      else if (val <= -8)
        res = val + 6;
      return res;
    }

    // Tyler used a hash table for the outer layer, but I think it's pointless
    // to introduce that much overhead.
    using yaw_table_t =
      std::array<std::map<float, std::pair<int8_t, int8_t>>, (1 << 16)>;

    void init_yaw_table(yaw_table_t& yaw_table) {
      for (int16_t stick_x = -128; stick_x <= 127; ++stick_x) {
        for (int16_t stick_y = -128; stick_y <= 127; ++stick_y) {
          auto [intended_yaw, intended_mag] = input_to_ym(stick_x, stick_y, 0);

          uint16_t yaw_idx = (uint16_t) intended_yaw;
          yaw_table[yaw_idx].try_emplace(intended_mag, stick_x, stick_y);
        }
      }
    }

    // Returns a table mapping {yaw angle + magnitude} to stick positions.
    const yaw_table_t& inv_yaw_table() {
      // the actual table
      static yaw_table_t inv_yaw_table {};
      // static initialization hack: this ensures that inv_yaw_table is only
      // initialized once.
      static bool _init_fn = (init_yaw_table(inv_yaw_table), true);

      return inv_yaw_table;
    }

  }  // namespace

  std::pair<int16_t, float> input_to_ym(
    int8_t x, int8_t y, int16_t camera_yaw) {
    
    // Perform adjustment for the controller input.
    // https://github.com/n64decomp/sm64/blob/9921382a68bb0c865e5e45eb594d9c64db59b1af/src/game/game_init.c#L433-L459

    // adjust input coordinates
    float adj_x = adjust_stick(x);
    float adj_y = adjust_stick(y);

    // compute norm, cap norm to 64 if needed
    float stick_mag = sqrtf(adj_x * adj_x + adj_y * adj_y);
    if (stick_mag > 64.0f) {
      adj_x *= 64.0f / stick_mag;
      adj_y *= 64.0f / stick_mag;
      stick_mag = 64.0f;
    }

    // Perform magnitude adjustments for Mario.
    // https://github.com/n64decomp/sm64/blob/9921382a68bb0c865e5e45eb594d9c64db59b1af/src/game/mario.c#L1295-L1308

    // Compute the intended magnitude. This coalesces all the powers of two into a single factor.
    // This should not affect the precision of the final answer, as powers of two do not affect the significand.
    float intended_mag = stick_mag * stick_mag / 128.0f;
    // Compute the intended yaw by atan2s.
    int16_t base_intended_yaw =
      (intended_mag > 0.0f) ? sm64::atan2s(-adj_y, adj_x) : 0;

    // return the yaw magnitude
    return {base_intended_yaw + camera_yaw, intended_mag};
  }
}  // namespace trowel