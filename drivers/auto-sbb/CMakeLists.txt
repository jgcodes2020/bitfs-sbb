add_executable(auto-sbb
    # headers
    # sources
    "src/main.cpp"
)
target_compile_features(auto-sbb PRIVATE cxx_std_23)
target_link_libraries(auto-sbb PRIVATE trowel sm64-utils vcr)