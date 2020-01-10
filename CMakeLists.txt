cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(tejk_it_izi)
#build app test
set(BUILD_TEJK_IT_IZI_TESTS OFF CACHE BOOL "build tests of the application modules")

add_subdirectory(server)
add_subdirectory(client)
