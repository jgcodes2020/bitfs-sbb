
if(NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/out")
endif()

add_subdirectory(auto-sbb)
add_subdirectory(onekey-test)