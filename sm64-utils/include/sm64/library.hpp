#ifndef SM64_LIBRARY_HPP_INCLUDED
#define SM64_LIBRARY_HPP_INCLUDED

#include <filesystem>
#include <memory>
#include "sm64/globals.hpp"
#include <decan.hpp>

namespace sm64 {
  using p_sm64_init = void (DECAN_STDCALL*)();
  using p_sm64_update = void (DECAN_STDCALL*)();

  class libsm64 {
  public:
    class state {
      // allow access to the private bits
      friend class libsm64;
    public:
      state(const state& rhs) = delete;
      state& operator=(const state& rhs) = delete;

      state(state&& rhs) = default;
      state& operator=(state&& rhs) = default;

      ~state() = default;
      
    private:
      state(const libsm64& lib);

      bool is_valid_for(const libsm64& lib) const;
      // 
      void allocate_for(const libsm64& lib);

      std::unique_ptr<std::byte[]> m_data_base;
      size_t m_data_len;
      std::unique_ptr<std::byte[]> m_bss_base;
      size_t m_bss_len;
    };
    
    // Loads and initializes libsm64.
    libsm64(const std::filesystem::path& path);
    
    libsm64(const libsm64&) = delete;
    libsm64& operator=(const libsm64&) = delete;
    
    libsm64(libsm64&&) = default;
    libsm64& operator=(libsm64&&) = default;

    ~libsm64() = default;

    // Advances the game state by 1 frame.
    void advance();

    // Retrieves a reference to a global.
    template <class T>
    T& operator[](global_tag<T> global);

    // Creates a new, empty savestate.
    state blank_state();
    // Saves the current state to a savestate.
    void save_to(state& state);
    // Loads a savestate.
    void load_from(const state& state);

  private:
    // library
    decan::library m_lib;
    // info about data and bss sections for saving
    decan::section_info m_data_sect;
    decan::section_info m_bss_sect;

    // useful functions
    p_sm64_init mfp_sm64_init;
    p_sm64_update mfp_sm64_update;
  };

  template <class T>
  T& libsm64::operator[](global_tag<T> global) {
    void* ptr = m_lib.get(global.name());
    return *reinterpret_cast<T*>(ptr);
  }
}

#endif