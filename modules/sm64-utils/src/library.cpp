#include "sm64/library.hpp"
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <utility>
#include "sm64/types.hpp"
#include "vcr.hpp"

namespace sm64 {
  libsm64::libsm64(const std::filesystem::path& path) : m_lib(path) {
    // retrieve section info (for saving)
    auto section_info = m_lib.read_sections();
    m_data_sect       = section_info[".data"];
    m_bss_sect        = section_info[".bss"];

    // get needed function pointers
    mfp_sm64_init   = (p_sm64_init) m_lib.get("sm64_init");
    mfp_sm64_update = (p_sm64_update) m_lib.get("sm64_update");

    // get needed globals
    m_gControllerPads = (OSContPad(*)[4]) m_lib.get("gControllerPads");

    // call sm64_init
    mfp_sm64_init();
  }

  void libsm64::advance() {
    // sm64_update advances the game 1 frame
    mfp_sm64_update();
  }

  vcr::frame libsm64::input(uint8_t port) const {
    if (port > 3)
      throw std::out_of_range("Port must range between 0 and 3");

    const auto& pad_obj = (*m_gControllerPads)[port];
    return vcr::frame {
      .buttons = (vcr::button) pad_obj.button,
      .stick_x = pad_obj.stick_x,
      .stick_y = pad_obj.stick_y};
  }

  void libsm64::set_input(vcr::frame frame, uint8_t port) {
    if (port > 3)
      throw std::out_of_range("Port must range between 0 and 3");

    // inputs are set by directly modifying gControllerPads; these are reset
    // after each frame advance
    auto& pad_obj   = (*m_gControllerPads)[port];
    pad_obj.button  = (u16) frame.buttons;
    pad_obj.stick_x = frame.stick_x;
    pad_obj.stick_y = frame.stick_y;
  }

  void libsm64::save_to(state& state) const {
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

  libsm64::state::state() :
    m_data_base(nullptr), m_data_len(0), m_bss_base(nullptr), m_bss_len(0) {}

  libsm64::state::state(const state& rhs) : state() {
    allocate_for(rhs);
    memcpy(m_data_base.get(), rhs.m_data_base.get(), m_data_len);
    memcpy(m_bss_base.get(), rhs.m_bss_base.get(), m_bss_len);
  }

  libsm64::state& libsm64::state::operator=(const state& rhs) {
    if (!is_valid_for(rhs))
      allocate_for(rhs);

    memcpy(m_data_base.get(), rhs.m_data_base.get(), m_data_len);
    memcpy(m_bss_base.get(), rhs.m_bss_base.get(), m_bss_len);
    return *this;
  }

  libsm64::state::state(state&& rhs) :
    m_data_base(std::move(rhs.m_data_base)),
    m_data_len(rhs.m_data_len),
    m_bss_base(std::move(rhs.m_bss_base)),
    m_bss_len(rhs.m_bss_len) {
    // set rhs buffer sizes to 0 (to indicate invalid)
    rhs.m_data_len = rhs.m_bss_len = 0;
  }

  libsm64::state& libsm64::state::operator=(state&& rhs) {
    m_data_base = std::move(rhs.m_data_base);
    m_data_len  = rhs.m_data_len;
    m_bss_base  = std::move(rhs.m_bss_base);
    m_bss_len   = rhs.m_bss_len;

    // set rhs buffer sizes to 0 (to indicate invalid)
    rhs.m_data_len = rhs.m_bss_len = 0;

    return *this;
  }

  bool libsm64::state::is_valid_for(const libsm64& lib) const {
    // ensure this isn't moved-out
    if (m_data_base == nullptr || m_bss_base == nullptr)
      return false;

    // ensure allocations are correctly sized
    return m_data_len == lib.m_data_sect.length &&
      m_bss_len == lib.m_bss_sect.length;
  }

  bool libsm64::state::is_valid_for(const state& rhs) const {
    // ensure this isn't moved-out
    if (m_data_base == nullptr || m_bss_base == nullptr)
      return false;

    // ensure allocations are correctly sized
    return m_data_len == rhs.m_data_len && m_bss_len == rhs.m_bss_len;
  }

  void libsm64::state::allocate_for(const libsm64& lib) {
    m_data_base.reset(new std::byte[lib.m_data_sect.length]);
    m_data_len = lib.m_data_sect.length;
    m_bss_base.reset(new std::byte[lib.m_bss_sect.length]);
    m_bss_len = lib.m_bss_sect.length;
  }

  void libsm64::state::allocate_for(const state& rhs) {
    m_data_base.reset(new std::byte[rhs.m_data_len]);
    m_data_len = rhs.m_data_len;
    m_bss_base.reset(new std::byte[rhs.m_bss_len]);
    m_bss_len = rhs.m_bss_len;
  }
}  // namespace sm64