add_library(vcr STATIC
    "include/vcr.hpp"
    "src/vcr.cpp"
)
target_compile_features(vcr PUBLIC cxx_std_20)
target_include_directories(vcr PUBLIC "include")