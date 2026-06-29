
add_library(decan-hpp STATIC
    "include/decan.hpp"
)
# platform-specific stuffs
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    target_sources(decan-hpp PRIVATE "src/decan_win32.cpp")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    target_sources(decan-hpp PRIVATE "src/decan_linux.cpp")
endif()

target_include_directories(decan-hpp PUBLIC "include")
global_target_settings(decan-hpp)
