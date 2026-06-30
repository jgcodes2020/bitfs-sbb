
add_library(sm64-utils STATIC
    # headers
    "include/sm64/camera.hpp"
    "include/sm64/constants.hpp"
    "include/sm64/globals.hpp"
    "include/sm64/library.hpp"
    "include/sm64/object_fields.hpp"
    "include/sm64/types.hpp"
    "include/sm64/ultra_types.hpp"
    # sources
    "src/library.cpp"
)
set_target_properties(sm64-utils PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(sm64-utils PUBLIC "include")
target_link_libraries(sm64-utils PUBLIC
    decan-hpp
)