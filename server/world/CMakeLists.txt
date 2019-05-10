cmake_minimum_required(VERSION 3.11)

project(world)

set(SOURCES
  src/world.cpp
  src/chat_participant.cpp
  src/friend_list.cpp
  src/friend_system.cpp
  src/tii_player_character.cpp
  src/friendlist_backend.cpp
)

set(INCLUDES
  inc/world_registrar.hpp
  inc/world.hpp
  inc/chat_system.hpp
  inc/chat_participant.hpp
  inc/user_environment.hpp
  inc/friend_list.hpp
  inc/friend_system.hpp
  inc/tii_player_character.hpp
  inc/friendlist_backend.hpp
)

SOURCE_GROUP(${PROJECT_NAME} FILES ${SOURCES} ${INCLUDES})

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${INCLUDES})
target_include_directories(${PROJECT_NAME} PUBLIC inc)
target_link_libraries(${PROJECT_NAME} PUBLIC net_core protocol)

if(BUILD_TEJK_IT_IZI_TESTS)
add_subdirectory(ut)
endif()