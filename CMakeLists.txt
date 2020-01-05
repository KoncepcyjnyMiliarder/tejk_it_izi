cmake_minimum_required(VERSION 3.11)

set (CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(tejk_it_izi)
#build app test
set(BUILD_TEJK_IT_IZI_TESTS OFF CACHE BOOL "build tests of the application modules")

#variables for gtest
set(BUILD_GTEST ON CACHE BOOL "force build gtest" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "force shared crt for windows" FORCE)

#variables for postgresql connector
set(SKIP_BUILD_TEST TRUE)
set(BUILD_TEST FALSE)

include(FetchContent)

#not entirely sure if it works this way?
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "dont update 3rd party libs" FORCE)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.8.0
)

if(BUILD_TEJK_IT_IZI_TESTS)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()
endif()

add_subdirectory(server)
add_subdirectory(client)
