
#include <filesystem>
#include <ios>
#include <iostream>
#include "sm64/constants.hpp"
#include "sm64/globals.hpp"
#include "sm64/library.hpp"
#include "vcr.hpp"

const struct {
  std::filesystem::path self = __FILE__;
  std::filesystem::path libsm64_jp = self.parent_path().parent_path() / "libsm64/lib/sm64_jp.so";
  std::filesystem::path onekey_m64 = self.parent_path() / "onekey.m64";
} main_paths;

int main() {
  using sm64::globals;
  sm64::libsm64 sm64(main_paths.libsm64_jp);
  vcr::m64 onekey(main_paths.onekey_m64);

  auto& gControllerPads = sm64[globals::gControllerPads];
  auto& gCurrLevelNum = sm64[globals::gCurrLevelNum];

  bool reached_bitfs = false;

  for (uint32_t i = 0; i < onekey.size(); i++) {
    gControllerPads[0].button = (uint16_t) onekey[i].buttons;
    gControllerPads[0].stick_x = onekey[i].stick_x;
    gControllerPads[0].stick_y = onekey[i].stick_y;

    sm64.advance();

    if (i % 150 == 0) {
      std::cout << "gCurrLevelNum = " << std::hex << gCurrLevelNum << std::endl;
    }
  }
}