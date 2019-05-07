cmake_minimum_required(VERSION 3.11)

project(model)

set(SOURCES
  src/dummy.cpp
)

set(INCLUDES
  inc/lobby_character.hpp
  inc/account_data.hpp
)

SOURCE_GROUP(${PROJECT_NAME} FILES ${SOURCES} ${INCLUDES})

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${INCLUDES})
target_include_directories(${PROJECT_NAME} PUBLIC inc)

#test