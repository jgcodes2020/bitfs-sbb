
add_library(trowel STATIC
    # headers
    "include/trowel/tracker.hpp"
    # sources
    "src/tracker.cpp"
)
target_compile_features(trowel PUBLIC cxx_std_20)
target_include_directories(trowel PUBLIC "include")
target_link_libraries(trowel PUBLIC
    sm64-utils
    vcr
)