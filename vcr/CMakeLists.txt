add_library(vcr STATIC
    "include/vcr.hpp"
    "src/vcr.cpp"
)
set_target_properties(vcr PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(vcr PUBLIC "include")