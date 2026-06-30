
add_library(decan-hpp STATIC
    "include/decan.hpp"
)

# platform-specific stuffs
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    target_sources(decan-hpp PRIVATE "src/decan_win32.cpp")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    target_sources(decan-hpp PRIVATE "src/decan_linux.cpp")
endif()

set_target_properties(decan-hpp PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(decan-hpp PUBLIC "include")
target_link_libraries(decan-hpp PUBLIC
    ${CMAKE_DL_LIBS}
)
