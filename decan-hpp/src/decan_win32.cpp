
#if defined(_WIN32)
  #include "decan.hpp"

  #include <exception>
  #include <fstream>
  #include <iostream>
  #include <memory>
  #include <string>
  #include <system_error>
  #include <unordered_map>

  #define NOMINMAX
  #include <windows.h>

namespace decan {

  library::library(const std::filesystem::path& file_name) :
    m_filename(file_name.string()), m_handle([&]() -> HMODULE {
      HMODULE res = LoadLibraryW(file_name.c_str());
      if (res == nullptr) {
        DWORD last_error = GetLastError();
        throw std::system_error(last_error, std::system_category());
      }
      return res;
    }()) {}

  library::~library() {
    bool good = FreeLibrary(m_handle);
    if (!good) {
      DWORD last_error = GetLastError();
      std::cerr << "FreeLibrary error: "
                << std::system_error(last_error, std::system_category()).what()
                << '\n';
      std::cerr << "terminating...\n";
      std::terminate();
    }
  }

  void* library::get(const char* symbol) const {
    FARPROC res = GetProcAddress(m_handle, symbol);
    if (res == nullptr) {
      DWORD last_error = GetLastError();
      throw std::system_error(last_error, std::system_category());
    }

    return reinterpret_cast<void*>(res);
  }

  std::unordered_map<std::string, section_info> library::read_sections() {
    using std::ios_base;
    std::unordered_map<std::string, section_info> section_map;

    std::ifstream file(m_filename, std::ios::binary);
    uint16_t num_sections;
    std::unique_ptr<IMAGE_SECTION_HEADER[]> sections;
    std::unique_ptr<char[]> str_table;
    // Unlike Linux ELF, the PE format is incredibly convoluted.
    // I could use a library, but all the ones I've found are humongous.
    {
      // Locate PE signature offset
      file.seekg(0x3C, ios_base::beg);
      uint32_t next_offset;
      file.read(reinterpret_cast<char*>(&next_offset), sizeof(uint32_t));
      // PE signature is 4 bytes, so skip those
      file.seekg(next_offset + 4, ios_base::beg);
      IMAGE_FILE_HEADER fileHeader;
      // Read out file header
      file.read(
        reinterpret_cast<char*>(&fileHeader), sizeof(IMAGE_FILE_HEADER));
      num_sections = fileHeader.NumberOfSections;
      // calculate string table offset
      // According to MS it's deprecated, but the DLLs still use it
      next_offset = fileHeader.PointerToSymbolTable +
        sizeof(IMAGE_SYMBOL) * fileHeader.NumberOfSymbols;
      // Jump past optional header
      file.seekg(fileHeader.SizeOfOptionalHeader, ios_base::cur);
      // Read out sections
      sections.reset(new IMAGE_SECTION_HEADER[fileHeader.NumberOfSections]);
      file.read(
        reinterpret_cast<char*>(sections.get()),
        fileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER));
      // Read string table size
      uint32_t str_table_size;
      file.seekg(next_offset, ios_base::beg);
      file.read(reinterpret_cast<char*>(&str_table_size), sizeof(uint32_t));
      // Read out string table
      file.seekg(next_offset, ios_base::beg);
      str_table.reset(new char[str_table_size]);
      file.read(str_table.get(), str_table_size);
    }
    std::string name;
    for (size_t i = 0; i < num_sections; i++) {
      // Copy the name into a std::string
      for (size_t j = 0; j < 8; j++) {
        if (sections[i].Name[j] == '\0') {
          name = std::string(
            reinterpret_cast<char*>(static_cast<BYTE*>(sections[i].Name)), j);
          break;
        }
      }
      // If the name begins with /, it's an offset into the string table, so
      // find it
      if (name[0] == '/') {
        uint32_t off = strtoul(name.c_str() + 1, nullptr, 10);
        name         = std::string(&str_table[off]);
      }
      // MS also put the section size in a union
      section_map[name] = section_info {
        reinterpret_cast<char*>(m_handle) + sections[i].VirtualAddress,
        sections[i].Misc.VirtualSize};
    }
    return section_map;
  }
}  // namespace decan

#endif