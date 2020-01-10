cmake_minimum_required(VERSION 3.11)

project(utils)

find_package(GTest CONFIG REQUIRED)

set(MOCKS
  fake_logger.hpp
  fake_logger.cpp
)

set(TESTS
  binary_deserializer_tests.cpp
  binary_serializer_tests.cpp
)

add_library(${PROJECT_NAME}_mocks STATIC ${MOCKS})
target_include_directories(${PROJECT_NAME}_mocks PUBLIC .)
target_link_libraries(${PROJECT_NAME}_mocks PUBLIC ${PROJECT_NAME})

add_executable(${PROJECT_NAME}_ut ${TESTS})
target_link_libraries(${PROJECT_NAME}_ut PRIVATE ${PROJECT_NAME} GTest::gmock_main)