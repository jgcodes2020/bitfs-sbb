
#include <cassert>
#include <filesystem>
#include <print>
#include "sm64/constants.hpp"
#include "sm64/globals.hpp"
#include "sm64/library.hpp"
#include "vcr.hpp"

const struct {
  std::filesystem::path self = __FILE__;
  std::filesystem::path libsm64_jp =
    self.parent_path().parent_path() / "libsm64/lib/sm64_jp" DECAN_LIB_EXT;
  std::filesystem::path onekey_m64 = self.parent_path() / "onekey.m64";
} main_paths;

int main() {
  using sm64::globals;
  sm64::libsm64 sm64(main_paths.libsm64_jp);
  vcr::m64 onekey(main_paths.onekey_m64);

  auto& gCurrLevelNum   = sm64[globals::gCurrLevelNum];

  auto bitfs_state = sm64.blank_state();
  uint32_t bitfs_frame_idx = 0;

  for (uint32_t i = 0; i < onekey.size(); i++) {
    if (gCurrLevelNum == sm64::LEVEL_BITFS && bitfs_frame_idx == 0) {
      std::println("Found BitFS before frame {}", i);
      bitfs_frame_idx = i;
      sm64.save_to(bitfs_state);
    }

    sm64.set_input(onekey[i]);
    sm64.advance();
  }
  // confirm that the .m64 reaches the end of the run
  assert(gCurrLevelNum == sm64::LEVEL_BOWSER_3);

  // reload BitFS
  sm64.load_from(bitfs_state);
  assert(gCurrLevelNum == sm64::LEVEL_BITFS);
}