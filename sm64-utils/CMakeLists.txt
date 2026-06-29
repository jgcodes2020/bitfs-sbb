
add_library(sm64-utils STATIC
    # headers
    "include/sm64/constants.hpp"
    "include/sm64/library.hpp"
    "include/sm64/object_fields.hpp"
    "include/sm64/types.hpp"
    "include/sm64/ultra_types.hpp"
    # sources
    "src/library.cpp"
)
target_include_directories(sm64-utils PUBLIC "include")
target_link_libraries(sm64-utils PUBLIC
    decan-hpp
)