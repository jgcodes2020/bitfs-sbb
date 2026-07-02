#include "trowel/joystick.hpp"
#include <array>
#include <cmath>
#include <cstdlib>
#include <iterator>
#include <limits>
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
  }  // namespace

  std::pair<int16_t, float> input_to_ym(
    int8_t x, int8_t y, int16_t camera_yaw) {
    // Note: Tyler had a hash-based LUT to do this, which I think is probably
    // worse for performance

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

    // Compute the intended magnitude. This coalesces all the powers of two into
    // a single factor. This should not affect the precision of the final
    // answer, as powers of two do not affect the significand.
    float intended_mag = stick_mag * stick_mag / 128.0f;
    // Compute the intended yaw by atan2s.
    int16_t base_intended_yaw =
      (intended_mag > 0.0f) ? sm64::atan2s(-adj_y, adj_x) : 0;

    // return the yaw magnitude (with camera)
    return {base_intended_yaw + camera_yaw, intended_mag};
  }

  namespace {
    // Tyler used a hash table for the outer layer, but I think it's pointless
    // to introduce that much overhead.
    using yaw_table_t =
      std::array<std::map<float, std::pair<int8_t, int8_t>>, (1 << 16)>;

    // initializes the inverse yaw-magnitude table.
    void init_yaw_table(yaw_table_t& yaw_table) {
      for (int16_t stick_x = -128; stick_x <= 127; ++stick_x) {
        for (int16_t stick_y = -128; stick_y <= 127; ++stick_y) {
          auto [intended_yaw, intended_mag] = input_to_ym(stick_x, stick_y, 0);

          yaw_table[(uint16_t) intended_yaw].try_emplace(
            intended_mag, stick_x, stick_y);
        }
      }
    }

    // Returns a table mapping {yaw angle + magnitude} to stick positions.
    const yaw_table_t& inv_ym_table() {
      // the actual table
      static yaw_table_t inv_yaw_table {};
      // static initialization hack: this ensures that inv_yaw_table is only
      // initialized once.
      static bool _init_fn = (init_yaw_table(inv_yaw_table), true);

      return inv_yaw_table;
    }

    enum class ym_match {
      // No candidates were found.
      none = 0,
      // Candidates matching the exact yaw were found, but the magnitude was
      // off. The best input may or may not have been updated.
      partial,
      // An exact yaw/magnitude match was found.
      exact,
    };

    /**
     * @brief Tries to exactly match a single yaw.
     *
     * @param yaw The yaw to match.
     * @param intended_mag The intended magnitude to match.
     * @param best_input Used to track the best match found.
     * @param best_mag_delta Used to track the smallest difference in magnitude.
     * @return the match result.
     */
    ym_match ym_check_single_yaw(
      int16_t yaw, float intended_mag, std::pair<int8_t, int8_t>& best_input,
      float& best_mag_delta) {
      const yaw_table_t& ym_table = inv_ym_table();

      // check if there are any candidates for this yaw
      const auto& mag_entries = ym_table[(uint16_t) yaw];
      if (mag_entries.empty())
        return ym_match::none;

      // try to lookup intended_mag, this is either an exact match or the first
      // thing larger
      auto upper_bound = mag_entries.lower_bound(intended_mag);

      // check for an exact match
      if (
        upper_bound != mag_entries.end() &&
        upper_bound->first == intended_mag) {
        best_input = upper_bound->second;
        return ym_match::exact;
      }
      // check if intended_mag is *less* than all candidates
      if (upper_bound == mag_entries.begin()) {
        float mag_delta = upper_bound->first - intended_mag;
        if (mag_delta < best_mag_delta) {
          best_mag_delta = mag_delta;
          best_input     = upper_bound->second;
        }
        return ym_match::partial;
      }
      // this is the first thing *less* than intended_mag
      auto lower_bound = std::prev(upper_bound);
      // check if intended_mag is *greater* than all candidates
      if (upper_bound == mag_entries.end()) {
        float mag_delta = intended_mag - lower_bound->first;
        if (mag_delta < best_mag_delta) {
          best_mag_delta = mag_delta;
          best_input     = lower_bound->second;
        }
        return ym_match::partial;
      }

      // upper and lower bound bracket intended_mag; compute both deltas
      float lower_mag_delta = intended_mag - lower_bound->first;
      float upper_mag_delta = upper_bound->first - intended_mag;

      // check the closer of the two and update
      if (lower_mag_delta <= upper_mag_delta) {
        if (lower_mag_delta < best_mag_delta) {
          best_mag_delta = lower_mag_delta;
          best_input     = lower_bound->second;
        }
      }
      else {
        if (upper_mag_delta < best_mag_delta) {
          best_mag_delta = upper_mag_delta;
          best_input     = upper_bound->second;
        }
      }

      return ym_match::partial;
    }

    // From 0, produces the next element in the sequence: 0, 1, -1, 2, -2, ...
    inline int16_t snake_offset(int16_t curr) {
      return (curr > 0)? -curr + 1 : -curr;
    }
  }  // namespace

  std::pair<int8_t, int8_t> find_input_by_ym_exact(
    int16_t intended_yaw, float intended_mag, int16_t camera_yaw) {
    constexpr size_t MAX_ATTEMPTS = 256;

    // TODO: THIS FUNCTION DOESN'T QUITE WORK YET
    abort();

    // special case: 0 mag
    if (intended_mag == 0.0f)
      return {0, 0};

    int16_t min_intended_yaw = hau_round(intended_yaw);
    int16_t max_intended_yaw = min_intended_yaw + 16;

    min_intended_yaw -= camera_yaw;
    max_intended_yaw -= camera_yaw;

    std::pair<int8_t, int8_t> best_input = {0, 0};
    float best_mag_delta = std::numeric_limits<float>::infinity();

    bool found_match = false;
    int16_t offset = 0;

    for (size_t attempts = 0; attempts < MAX_ATTEMPTS; ++attempts) {

    }
  }
}  // namespace trowel