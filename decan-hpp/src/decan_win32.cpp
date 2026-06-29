
#if defined(_WIN32)
#include "decan.hpp"

#include <codecvt>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <system_error>
#include <unordered_map>

#define NOMINMAX
	#include <windows.h>

library::library(const std::filesystem::path& fileName) :
	m_filename(fileName.string()),
	m_handle(
		[&]() -> HMODULE
		{
	HMODULE res = LoadLibraryW(fileName.c_str());
	if (res == nullptr)
	{
		DWORD lastError = GetLastError();
		throw std::system_error(lastError, std::system_category());
	}
	return res;
		}()) { }

library::~library()
{
	bool good = FreeLibrary(m_handle);
	if (!good)
	{
		DWORD lastError = GetLastError();
		std::cerr << "FreeLibrary error: "
							<< std::system_error(lastError, std::system_category()).what()
							<< '\n';
		std::cerr << "terminating...\n";
		std::terminate();
	}
}

void* library::get(const char* symbol) const
{
	FARPROC res = GetProcAddress(m_handle, symbol);
	if (res == nullptr)
	{
		DWORD lastError = GetLastError();
		throw std::system_error(lastError, std::system_category());
	}

	return reinterpret_cast<void*>(res);
}

std::unordered_map<std::string, section_info> library::read_sections()
{
	using std::ios_base;
	std::unordered_map<std::string, section_info> sectionMap;

	std::ifstream file(m_filename, std::ios::binary);
	uint16_t numSections;
	std::unique_ptr<IMAGE_SECTION_HEADER[]> sections;
	std::unique_ptr<char[]> strTable;
	// Unlike Linux ELF, the PE format is incredibly convoluted.
	// I could use a library, but all the ones I've found are humongous.
	{
		// Locate PE signature offset
		file.seekg(0x3C, ios_base::beg);
		uint32_t nextOffset;
		file.read(reinterpret_cast<char*>(&nextOffset), sizeof(uint32_t));
		// PE signature is 4 bytes, so skip those
		file.seekg(nextOffset + 4, ios_base::beg);
		IMAGE_FILE_HEADER fileHeader;
		// Read out file header
		file.read(reinterpret_cast<char*>(&fileHeader), sizeof(IMAGE_FILE_HEADER));
		numSections = fileHeader.NumberOfSections;
		// calculate string table offset
		// According to MS it's deprecated, but the DLLs still use it
		nextOffset = fileHeader.PointerToSymbolTable +
			sizeof(IMAGE_SYMBOL) * fileHeader.NumberOfSymbols;
		// Jump past optional header
		file.seekg(fileHeader.SizeOfOptionalHeader, ios_base::cur);
		// Read out sections
		sections.reset(new IMAGE_SECTION_HEADER[fileHeader.NumberOfSections]);
		file.read(
			reinterpret_cast<char*>(sections.get()),
			fileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER));
		// Read string table size
		uint32_t strTableSize;
		file.seekg(nextOffset, ios_base::beg);
		file.read(reinterpret_cast<char*>(&strTableSize), sizeof(uint32_t));
		// Read out string table
		file.seekg(nextOffset, ios_base::beg);
		strTable.reset(new char[strTableSize]);
		file.read(strTable.get(), strTableSize);
	}
	std::string name;
	for (size_t i = 0; i < numSections; i++)
	{
		// Copy the name into a std::string
		for (size_t j = 0; j < 8; j++)
		{
			if (sections[i].Name[j] == '\0')
			{
				name = std::string(
					reinterpret_cast<char*>(static_cast<BYTE*>(sections[i].Name)), j);
				break;
			}
		}
		// If the name begins with /, it's an offset into the string table, so
		// find it
		if (name[0] == '/')
		{
			uint32_t off = strtoul(name.c_str() + 1, nullptr, 10);
			name				 = std::string(&strTable[off]);
		}
		// MS also put the section size in a union
		sectionMap[name] = section_info {
			reinterpret_cast<char*>(m_handle) + sections[i].VirtualAddress,
			sections[i].Misc.VirtualSize};
	}
	return sectionMap;
}

#endif