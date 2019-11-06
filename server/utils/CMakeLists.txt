cmake_minimum_required(VERSION 3.11)

include(${PROJECT_SOURCE_DIR}/set_warning_level.cmake)

project(utils)

set(SOURCES
  src/console_logger.cpp
)

set(INCLUDES
  inc/logger.hpp
  inc/console_logger.hpp
  inc/binary_deserializer.hpp
  inc/binary_serializer.hpp
)

SOURCE_GROUP(${PROJECT_NAME} FILES ${SOURCES} ${INCLUDES})

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${INCLUDES})
target_include_directories(${PROJECT_NAME} PUBLIC inc)
set_warning_level(${PROJECT_NAME})

if(BUILD_TEJK_IT_IZI_TESTS)
add_subdirectory(ut)
endif()
