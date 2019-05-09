cmake_minimum_required(VERSION 3.11)

project(world)

set(SOURCES
  src/world.cpp
  src/chat_participant.cpp
)

set(INCLUDES
  inc/world.hpp
  inc/chat_system.hpp
  inc/chat_participant.hpp
  inc/user_environment.hpp
)

SOURCE_GROUP(${PROJECT_NAME} FILES ${SOURCES} ${INCLUDES})

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${INCLUDES})
target_include_directories(${PROJECT_NAME} PUBLIC inc)
target_link_libraries(${PROJECT_NAME} PUBLIC net_core protocol)

if(BUILD_TEJK_IT_IZI_TESTS)
add_subdirectory(ut)
endif()