cmake_minimum_required(VERSION 3.11)

project(database_facade)

set(MOCKS
  fake_database_facade.cpp
  fake_database_facade.hpp
)

add_library(${PROJECT_NAME}_mocks STATIC ${MOCKS})
target_include_directories(${PROJECT_NAME}_mocks PUBLIC .)
target_link_libraries(${PROJECT_NAME}_mocks PUBLIC ${PROJECT_NAME})
