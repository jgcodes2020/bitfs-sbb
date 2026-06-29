#include "sm64/library.hpp"
#include <cstring>
#include <stdexcept>

namespace sm64 {
  libsm64::libsm64(const std::filesystem::path& path) : m_lib(path) {
    // retrieve section info (for saving)
    auto section_info = m_lib.read_sections();
    m_data_sect       = section_info[".data"];
    m_bss_sect        = section_info[".bss"];

    // get needed function pointers
    mfp_sm64_init   = (p_sm64_init) m_lib.get("sm64_init");
    mfp_sm64_update = (p_sm64_update) m_lib.get("sm64_update");

    // call sm64_init
    mfp_sm64_init();
  }

  void libsm64::advance() {
    // sm64_update advances the game 1 frame
    mfp_sm64_update();
  }

  libsm64::state libsm64::blank_state() {
    return state(*this);
  }

  void libsm64::save_to(state& state) {
    // if the data doesn't line up, reallocate
    if (!state.is_valid_for(*this))
      state.allocate_for(*this);

    // copy data/bss sections to the state
    std::memcpy(state.m_data_base.get(), m_data_sect.address, state.m_data_len);
    std::memcpy(state.m_bss_base.get(), m_bss_sect.address, state.m_bss_len);
  }

  void libsm64::load_from(const state& state) {
    if (!state.is_valid_for(*this))
      throw std::invalid_argument("state does not have data saved");

    // copy state to the data/bss sections
    std::memcpy(m_data_sect.address, state.m_data_base.get(), state.m_data_len);
    std::memcpy(m_bss_sect.address, state.m_bss_base.get(), state.m_bss_len);
  }

  libsm64::state::state(const libsm64& lib) :
    m_data_base(new std::byte[lib.m_data_sect.length]),
    m_data_len(lib.m_data_sect.length),
    m_bss_base(new std::byte[lib.m_bss_sect.length]),
    m_bss_len(lib.m_bss_sect.length) {}

  bool libsm64::state::is_valid_for(const libsm64& lib) const {
    // ensure this isn't moved-out
    if (m_data_base == nullptr || m_bss_base == nullptr)
      return false;

    // ensure allocations are correctly sized
    return m_data_len == lib.m_data_sect.length &&
      m_bss_len == lib.m_bss_sect.length;
  }

  void libsm64::state::allocate_for(const libsm64& lib) {
    m_data_base.reset(new std::byte[lib.m_data_sect.length]);
    m_data_len = lib.m_data_sect.length;
    m_bss_base.reset(new std::byte[lib.m_bss_sect.length]);
    m_bss_len = lib.m_bss_sect.length;
  }
}  // namespace sm64