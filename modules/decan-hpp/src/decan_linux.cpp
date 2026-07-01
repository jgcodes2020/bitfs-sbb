
#if defined(__linux__)
  #include "decan.hpp"

  #include <exception>
  #include <fstream>
  #include <iostream>
  #include <memory>
  #include <stdexcept>
  #include <string>
  #include <unordered_map>

  #include <dlfcn.h>
  #include <elf.h>
  #include <link.h>

namespace decan {
  library::library(const std::filesystem::path& fileName) :
    m_filename(fileName.string()), m_handle([&]() -> void* {
      void* res = dlopen(fileName.c_str(), RTLD_NOW);
      if (res == nullptr) {
        throw std::runtime_error(dlerror());
      }
      return res;
    }()) {}
  library::~library() {
    int res = dlclose(m_handle);
    if (res != 0) {
      std::cerr << "dlclose error: " << dlerror() << '\n';
      std::cerr << "terminating...\n";
      std::terminate();
    }
  }
  void* library::get(const char* symbol) const {
    dlerror();
    void* res = dlsym(m_handle, symbol);
    char* err = dlerror();
    if (err != nullptr) {
      throw std::runtime_error(err);
    }
    return res;
  }
  std::unordered_map<std::string, section_info> library::read_sections() {
    using std::ios_base;
    
    std::unordered_map<std::string, section_info> sectionMap;

    // find the loaded base address
    intptr_t base_address;
    {
      link_map* link_map;
      int returnCode = dlinfo(m_handle, RTLD_DI_LINKMAP, &link_map);
      if (returnCode == -1) {
        throw std::runtime_error(dlerror());
      }
      base_address = link_map->l_addr;
    }

    // ld.so may not load the section table, so open the file separately to locate it.
    std::ifstream file(m_filename);
    file.seekg(0, ios_base::beg);
    std::unique_ptr<Elf64_Shdr[]> sections;
    std::unique_ptr<char[]> str_table;
    uint16_t num_sections;
    {
      // read header
      Elf64_Ehdr header;
      file.read(reinterpret_cast<char*>(&header), sizeof(header));
      if (header.e_shoff == 0) {
        throw std::runtime_error("No section table");
      }
      num_sections = header.e_shnum;

      // read section headers
      file.seekg(header.e_shoff, ios_base::beg);
      sections.reset(new Elf64_Shdr[header.e_shnum]);
      file.read(
        reinterpret_cast<char*>(sections.get()),
        header.e_shnum * sizeof(Elf64_Shdr));
      
      // locate .shstrtab section for section names
      const Elf64_Shdr& strtab_sect = sections[header.e_shstrndx];

      // read section names
      file.seekg(strtab_sect.sh_offset, ios_base::beg);
      str_table.reset(new char[strtab_sect.sh_size]);
      file.read(str_table.get(), strtab_sect.sh_size);
    }

    // prepare output map
    for (uint16_t i = 0; i < num_sections; i++) {
      const auto& sect                    = sections[i];
      sectionMap[&str_table[sect.sh_name]] = section_info {
        reinterpret_cast<void*>(base_address + sect.sh_addr), sect.sh_size};
    }
    return sectionMap;
  }
}  // namespace decan

#endif