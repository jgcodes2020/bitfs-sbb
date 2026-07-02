#include "trowel/tracker.hpp"
#include <algorithm>
#include <format>
#include <stdexcept>
#include "sm64/library.hpp"
#include "vcr.hpp"

namespace trowel {
  tracker::tracker(
    const std::filesystem::path& sm64_path,
    std::vector<vcr::frame>&& input_frames) :
    m_sm64(sm64_path),
    m_frames(std::move(input_frames)),
    m_reset_state(sm64::libsm64::state()) {
    m_sm64.save_to(m_reset_state);
  }

  size_t tracker::advance(size_t count) {
    size_t i = 0;
    for (; i < count && m_index < m_frames.size(); ++i, ++m_index) {
      m_sm64.set_input(m_frames[m_index]);
      m_sm64.advance();
    }
    return i;
  }

  void tracker::skip_to(size_t target) {
    if (m_index > target)
      throw std::out_of_range(
        std::format(
          "tracker::skip_to() cannot skip backwards (index: {} -> target: {})",
          m_index, target));

    size_t real_target = std::min(m_frames.size(), target);

    while (m_index < real_target) {
      m_sm64.set_input(m_frames[m_index]);
      m_sm64.advance();
      ++m_index;
    }
  }

  void tracker::advance_with(vcr::frame next) {
    m_sm64.set_input(next);
    m_sm64.advance();

    // if we're in the middle of the input file, truncate to the current
    // position
    if (m_index < m_frames.size())
      m_frames.resize(m_index);

    m_frames.emplace_back(next);
    ++m_index;
  }

  void tracker::restart() {
    m_sm64.load_from(m_reset_state);
    m_index = 0;
  }

  void tracker::save_to(state& state) const {
    m_sm64.save_to(state.m_state);
    state.m_frames = m_frames;
    state.m_index = m_index;
  }

  void tracker::load_from(const state& state) {
    m_sm64.load_from(state.m_state);
    m_frames = state.m_frames;
    m_index = state.m_index;
  }
}  // namespace trowel