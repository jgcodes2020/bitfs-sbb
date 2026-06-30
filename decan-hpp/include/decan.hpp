#include <cstddef>
#include <filesystem>
#include <string>
#include <unordered_map>

#ifndef DECAN_HPP
  #define DECAN_HPP

  #if defined(_WIN32)
    #define NOMINMAX
    #include <windows.h>

    // On Windows, applies the stdcall calling convention.
    #define DECAN_STDCALL __stdcall
  #elif defined(__linux__)
    // On Windows, applies the stdcall calling convention.
    #define DECAN_STDCALL
  #endif

namespace decan {

  #if defined(_WIN32)
  using handle_t = HMODULE;
  #elif defined(__linux__)
  using handle_t = void*;
  #endif

  struct section_info {
    void* address;
    size_t length;
  };

  class library {
    std::string m_filename;
		handle_t m_handle;

  public:
    library(const std::filesystem::path& path);
    
    library(const library&) = delete;
    library& operator=(const library&) = delete;

    library(library&& rhs) : m_filename(std::move(rhs.m_filename)), m_handle(rhs.m_handle) {
      // zero the RHS handle to avoid misuse
      rhs.m_handle = (handle_t) 0;
    }
    library& operator=(library&& rhs) {
      // destruct, then move-construct.
      this->~library();
      new(this) library(std::move(rhs));
      return *this;
    }
    
    ~library();
		// Gets a symbol from a library.
		// This maps directly to GetProcAddress/dlsym.
    void* get(const char* symbol) const;

    // Loads the file and parses section data.
    std::unordered_map<std::string, section_info> read_sections();
  };

}  // namespace decan

#endif
