cmake_minimum_required(VERSION 3.11)

set (CMAKE_CXX_STANDARD 14)

project(tejk_it_izi)
#build app test
set(BUILD_TEJK_IT_IZI_TESTS ON CACHE BOOL "build tests of the application modules")

#variables for gtest
set(BUILD_GTEST ON CACHE BOOL "force build gtest" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "force shared crt for windows" FORCE)

#variables for postgresql connector
set(SKIP_BUILD_TEST TRUE)
set(BUILD_TEST FALSE)
set(SKIP_PQXX_SHARED TRUE)

include(FetchContent)

#not entirely sure if it works this way?
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "dont update 3rd party libs" FORCE)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.8.0
)

FetchContent_Declare(
  libpqxx
  GIT_REPOSITORY https://github.com/jtv/libpqxx.git
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

FetchContent_GetProperties(libpqxx)
if(NOT libpqxx_POPULATED)
  FetchContent_Populate(libpqxx)
  add_subdirectory(${libpqxx_SOURCE_DIR} ${libpqxx_BINARY_DIR})
endif()

add_subdirectory(server)
add_subdirectory(client)
