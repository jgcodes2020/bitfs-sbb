#include <cassert>
#include <cstddef>
#include <filesystem>
#include <print>
#include "sm64/constants.hpp"
#include "sm64/object_fields.hpp"
#include "sm64/globals.hpp"
#include "sm64/library.hpp"
#include "sm64/types.hpp"
#include "vcr.hpp"

const struct {
  std::filesystem::path self = __FILE__;
  std::filesystem::path libsm64_jp =
    self.parent_path().parent_path().parent_path() / "libsm64/lib/sm64_jp" DECAN_LIB_EXT;
  std::filesystem::path onekey_m64 = self.parent_path() / "onekey.m64";
} main_paths;

int main() {
  using sm64::globals;
  sm64::libsm64 sm64(main_paths.libsm64_jp);
  vcr::m64 onekey(main_paths.onekey_m64);

  auto& gCurrLevelNum = sm64[globals::gCurrLevelNum];
  auto& gObjectPool   = sm64[globals::gObjectPool];

  const auto* bhvBully = sm64[globals::bhvSmallBully];
  const auto* bhvWiiVCPlat = sm64[globals::bhvBitFSSinkingPlatforms];
  const auto* bhvSpinHeart = sm64[globals::bhvRecoveryHeart];

  constexpr size_t BULLY_SLOT = 27;
  constexpr size_t HEART_SLOT = 29;
  constexpr size_t WIIVC_PLAT_SLOT = 81;

  constexpr float BULLY_HOME_Y = -2764;
  constexpr float WIIVC_PLAT_HOME_Y = -3065;
  constexpr float HEART_Y = -2700;

  auto bitfs_state         = sm64::libsm64::state();
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
  
  // confirm bully, heart, and wiivc platform are in their respective slots
  auto& bully = gObjectPool[BULLY_SLOT];
  auto& heart = gObjectPool[HEART_SLOT];
  auto& wiivc_plat = gObjectPool[WIIVC_PLAT_SLOT];

  assert(bully.behavior == bhvBully);
  assert(heart.behavior == bhvSpinHeart);
  assert(wiivc_plat.behavior == bhvWiiVCPlat);

  assert(bully.oHomeY == BULLY_HOME_Y);
  assert(heart.oPosY == HEART_Y);
  assert(wiivc_plat.oHomeY == WIIVC_PLAT_HOME_Y);
}