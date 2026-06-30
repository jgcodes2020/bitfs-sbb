add_executable(onekey-test "main.cpp")
target_compile_features(onekey-test PRIVATE cxx_std_20)
target_link_libraries(onekey-test PRIVATE sm64-utils)