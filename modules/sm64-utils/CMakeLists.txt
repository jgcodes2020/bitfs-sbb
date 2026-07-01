
add_library(sm64-utils STATIC
    # headers
    "include/sm64/camera.hpp"
    "include/sm64/constants.hpp"
    "include/sm64/globals.hpp"
    "include/sm64/library.hpp"
    "include/sm64/math_util.hpp"
    "include/sm64/object_fields.hpp"
    "include/sm64/types.hpp"
    "include/sm64/ultra_types.hpp"
    # sources
    "src/library.cpp"
    "src/math_util.cpp"
    "src/trig_tables.cpp"
)
target_compile_features(sm64-utils PUBLIC cxx_std_20)
target_include_directories(sm64-utils PUBLIC "include")
target_link_libraries(sm64-utils PUBLIC
    decan-hpp
    vcr
)