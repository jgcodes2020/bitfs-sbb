add_executable(onekey-base "onekey-base.cpp")
target_compile_features(onekey-base PRIVATE cxx_std_23)
target_link_libraries(onekey-base PRIVATE sm64-utils)

add_executable(onekey-tracker "onekey-tracker.cpp")
target_compile_features(onekey-tracker PRIVATE cxx_std_23)
target_link_libraries(onekey-tracker PRIVATE trowel sm64-utils)