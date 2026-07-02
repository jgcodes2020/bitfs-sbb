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

    // Combined state information for both libsm64 and the input frames.
    class state {
      friend class tracker;
    public:
      state() = default;

      // Returns a span containing the saved input frames.
      std::span<const vcr::frame> frames() { return m_frames; }

      // Returns the index at which this frame was saved.
      size_t index() { return m_index; }

    private:
      sm64::libsm64::state m_state;
      std::vector<vcr::frame> m_frames;
      size_t m_index;
    };

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
    std::span<const vcr::frame> frames() { return m_frames; }

#pragma region Basic timekeeping
    // Returns the current frame index.
    size_t index() const { return m_index; }
    // Returns the number of frames in the sequence.
    size_t num_frames() const { return m_frames.size(); }

    // Tries to advances `count` frames forward, up to the end of the current
    // inputs. Returns the number of frames advanced.
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
    // Saves the current state to a savestate.
    void save_to(state& state) const;
    
    // Loads from a savestate.
    void load_from(const state& state);
#pragma endregion

  private:

    sm64::libsm64 m_sm64;
    std::vector<vcr::frame> m_frames;
    size_t m_index = 0;

    sm64::libsm64::state m_reset_state;
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