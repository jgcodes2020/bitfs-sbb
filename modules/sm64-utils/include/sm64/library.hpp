#ifndef SM64_LIBRARY_HPP_INCLUDED
#define SM64_LIBRARY_HPP_INCLUDED

#include <filesystem>
#include <memory>
#include <type_traits>
#include "sm64/globals.hpp"
#include "sm64/types.hpp"
#include "vcr.hpp"
#include <decan.hpp>

namespace sm64 {
  using p_sm64_init   = void(DECAN_STDCALL*)();
  using p_sm64_update = void(DECAN_STDCALL*)();

  // A loaded instance of libsm64.
  class libsm64 {
  public:
    // A savestate created from the global data segments of libsm64.
    // Is otherwise opaque besides containing save data.
    class state {
      // allow access to the private bits
      friend class libsm64;

    public:
      // Creates a new blank state with no allocation or data.
      state();

      state(const state& rhs);
      state& operator=(const state& rhs);

      state(state&& rhs);
      state& operator=(state&& rhs);

      ~state() = default;

    private:
      // Checks whether a given state's buffers can be reused (for a libsm64
      // instance).
      bool is_valid_for(const libsm64& lib) const;
      // Checks whether a given state's buffers can be reused (for copying).
      bool is_valid_for(const state& lib) const;
      // Allocates/reallocates buffers for the savestate (for a libsm64
      // instance).
      void allocate_for(const libsm64& lib);
      // allocates/reallocates buffers for the savestate (for copying).
      void allocate_for(const state& rhs);

      std::unique_ptr<std::byte[]> m_data_base;
      size_t m_data_len;
      std::unique_ptr<std::byte[]> m_bss_base;
      size_t m_bss_len;
    };

    // Loads and initializes libsm64.
    libsm64(const std::filesystem::path& path);

    libsm64(const libsm64&)            = delete;
    libsm64& operator=(const libsm64&) = delete;

    libsm64(libsm64&&)            = default;
    libsm64& operator=(libsm64&&) = default;

    ~libsm64() = default;

#pragma region Fundamental operations
    // Advances the game state by 1 frame.
    void advance();

    // Retrieves a reference to a global.
    template <class T>
    T& operator[](sized_global<T> global);
    // Retrieves a pointer to a global.
    template <class T>
    T* operator[](ptr_global<T> global);

    // Retrieves a const reference to a global.
    template <class T>
    const T& operator[](sized_global<T> global) const;
    // Retrieves a const pointer to a global.
    template <class T>
    const T* operator[](ptr_global<T> global) const;

    // Gets a given controller port's current input.
    vcr::frame input(uint8_t port = 0) const;
    // Sets a given controller port's input using a VCR frame.
    void set_input(vcr::frame frame, uint8_t port = 0);
#pragma endregion

#pragma region Savestates
    // Saves the current state to a savestate.
    void save_to(state& state) const;
    // Loads a savestate.
    void load_from(const state& state);
#pragma endregion

  private:
    // library
    decan::library m_lib;
    // info about data and bss sections for saving
    decan::section_info m_data_sect;
    decan::section_info m_bss_sect;

    // useful functions
    p_sm64_init mfp_sm64_init;
    p_sm64_update mfp_sm64_update;

    // internal pointers
    OSContPad (*m_gControllerPads)[4];
  };

  template <class T>
  T& libsm64::operator[](sized_global<T> global) {
    void* ptr = m_lib.get(global.name());
    return *reinterpret_cast<T*>(ptr);
  }

  template <class T>
  T* libsm64::operator[](ptr_global<T> global) {
    void* ptr = m_lib.get(global.name());
    return reinterpret_cast<T*>(ptr);
  }

  template <class T>
  const T& libsm64::operator[](sized_global<T> global) const {
    void* ptr = m_lib.get(global.name());
    return *reinterpret_cast<const T*>(ptr);
  }

  template <class T>
  const T* libsm64::operator[](ptr_global<T> global) const {
    void* ptr = m_lib.get(global.name());
    return reinterpret_cast<const T*>(ptr);
  }
}  // namespace sm64

#endif