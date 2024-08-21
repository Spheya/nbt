cmake_minimum_required (VERSION 3.12)

project ("nbt")
add_library(nbt INTERFACE)
target_include_directories(nbt INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    add_executable(nbt_example "example.cpp")
    set_property(TARGET nbt_example PROPERTY CXX_STANDARD 20)
    target_link_libraries(nbt_example PUBLIC nbt)
endif()