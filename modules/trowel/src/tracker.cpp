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

  void tracker::ensure_states(size_t n) {
    m_num_states = n;
    // resize if more slots are needed
    if (m_num_states > m_state_pool.size())
      m_state_pool.resize(m_num_states);
  }
  void tracker::trim_states() {
    // resize if less slots are needed
    if (m_num_states < m_state_pool.size())
      m_state_pool.resize(m_num_states);
  }

  void tracker::save_slot(size_t n) {
    slot_range_check(n);

    m_sm64.save_to(m_state_pool[n].state);
    m_state_pool[n].inputs = m_frames;
    m_state_pool[n].index = m_index;
  }
  void tracker::load_slot(size_t n) {
    slot_range_check(n);

    m_sm64.load_from(m_state_pool[n].state);
    m_frames = m_state_pool[n].inputs;
    m_index = m_state_pool[n].index;
  }

  void tracker::copy_slot(size_t src, size_t dst) {
    slot_range_check(src);
    slot_range_check(dst);

    m_state_pool[dst] = m_state_pool[src];
  }

  void tracker::slot_range_check(size_t index) {
    if (index >= m_num_states)
      throw std::out_of_range(
        std::format(
          "tracker::slot_range_check(): invalid state slot index {} (m_num_states = {})",
          index, m_num_states));
  }
}  // namespace trowel