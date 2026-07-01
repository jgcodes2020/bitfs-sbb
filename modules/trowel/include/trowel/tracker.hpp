#ifndef TROWEL_TRACKER_HPP_INCLUDED
#define TROWEL_TRACKER_HPP_INCLUDED

#include <limits>
#include <span>
#include <vector>
#include "sm64/globals.hpp"
#include "sm64/library.hpp"
#include "vcr.hpp"
namespace trowel {

  // Tracks both libsm64 state and a set of inputs.
  class tracker {
  public:
    static constexpr size_t npos = std::numeric_limits<size_t>::max();

    // Sets up the tracker using the provided path to libsm64 and input
    // sequence.
    tracker(
      const std::filesystem::path& sm64_path,
      std::vector<vcr::frame>&& input_frames);

    // Retrieves a reference to a global.
    template <class T>
    const T& operator[](sm64::sized_global<T> global) const;
    // Retrieves a pointer to a global.
    template <class T>
    const T* operator[](sm64::ptr_global<T> global) const;

    // Returns a span containing the current set of frames.
    std::span<const vcr::frame> frames() {
      return m_frames;
    }

#pragma region Basic timekeeping
    // Returns the current frame index.
    size_t index() const { return m_index; }
    // Returns the number of frames in the sequence.
    size_t num_frames() const { return m_frames.size(); }

    // Tries to advances `count` frames forward, up to the end of the current inputs.
    // Returns the number of frames advanced.
    size_t advance(size_t count = 1);

    // Jumps forward to the provided frame index.
    // If the index is past the last frame, jumps to the end instead.
    void skip_to(size_t target = npos);

    // Advances using the provided frame.
    // Inputs past this point will be clobbered.
    void advance_with(vcr::frame next);

    // Jumps back to frame 0 of the current input sequence.
    void restart();
#pragma endregion

#pragma region Savestates
    // Sets the number of states available in the state pool.
    // There may be more states present; though states past the end cannot be
    // used.
    void ensure_states(size_t n);

    // Returns the current size of the state pool.
    size_t num_states() const { return m_num_states; }

    // Trims the state pool's inner capacity to the current size.
    void trim_states();

    // Saves the game and input state to slot n.
    void save_slot(size_t n);

    // Loads the game and input state from slot n.
    void load_slot(size_t n);

    // Copies one slot to another.
    void copy_slot(size_t src, size_t dst);
#pragma endregion

  private:
    struct state_slot {
      sm64::libsm64::state state;
      std::vector<vcr::frame> inputs;
      size_t index;
    };

    // Ensures a slot index is valid, throwing if not.
    void slot_range_check(size_t index);

    sm64::libsm64 m_sm64;
    std::vector<vcr::frame> m_frames;
    size_t m_index = 0;

    sm64::libsm64::state m_reset_state;

    std::vector<state_slot> m_state_pool {};
    size_t m_num_states = 0;
  };

  template <class T>
  const T& tracker::operator[](sm64::sized_global<T> global) const {
    return m_sm64[global];
  }

  template <class T>
  const T* tracker::operator[](sm64::ptr_global<T> global) const {
    return m_sm64[global];
  }
};  // namespace trowel

#endif