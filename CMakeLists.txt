cmake_minimum_required(VERSION 4.2 FATAL_ERROR)
project(bitfs-sbb
    DESCRIPTION "The automation for SBB in BitFS."
    LANGUAGES C CXX
)
#[===[
Applies a bunch of common settings to a target.
]===]
function(global_target_settings target)
    set_target_properties("${target}" PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )
endfunction()

# shamelessly stolen from here; sets build type automagically
# https://www.kitware.com/cmake-and-the-default-build-type/
set(default_build_type "Release")
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  set(default_build_type "Debug")
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(decan-hpp)