cmake_minimum_required(VERSION 3.11)

project(controllers)

set(SOURCES
  src/chat_controller.cpp
)

set(INCLUDES
  inc/chat_controller.hpp
)

SOURCE_GROUP(${PROJECT_NAME} FILES ${SOURCES} ${INCLUDES})

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${INCLUDES})
target_include_directories(${PROJECT_NAME} PUBLIC inc)
target_link_libraries(${PROJECT_NAME} PUBLIC model world)

if(BUILD_TEJK_IT_IZI_TESTS)
add_subdirectory(ut)
endif()