#include <cstddef>
#include <filesystem>
#include <string>
#include <unordered_map>

#ifndef DECAN_HPP
  #define DECAN_HPP

  #if defined(_WIN32)
    #define NOMINMAX
    #include <windows.h>

    #define TAS_FW_STDCALL __stdcall
  #elif defined(__linux__)
    #define TAS_FW_STDCALL
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
    ~library();

		// Gets a symbol from a library.
		// This maps directly to GetProcAddress/dlsym.
    void* get(const char* symbol) const;

    // Loads the file and parses 
    std::unordered_map<std::string, section_info> read_sections();
  };

}  // namespace decan

#endif
