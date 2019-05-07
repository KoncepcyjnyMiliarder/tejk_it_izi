cmake_minimum_required(VERSION 3.11)

project(protocol)

set(SOURCES  
  src/lobby_state_recv_packet_factory.cpp
  src/ingame_state_recv_packet_factory.cpp
  src/null_received_packet.cpp  
)

set(PACKETS_S2C
  src/s2c/character_created.cpp
  inc/s2c/character_created.hpp
  src/s2c/character_creation_fail.cpp
  inc/s2c/character_creation_fail.hpp
  src/s2c/character_list.cpp
  inc/s2c/character_list.hpp
  src/s2c/character_selected.cpp
  inc/s2c/character_selected.hpp
  src/s2c/chat_msg.cpp
  inc/s2c/chat_msg.hpp
  src/s2c/user_joined_chat.cpp
  inc/s2c/user_joined_chat.hpp
  src/s2c/user_left_chat.cpp
  inc/s2c/user_left_chat.hpp
  src/s2c/moved_to_lobby.cpp
  inc/s2c/moved_to_lobby.hpp
)

set(PACKETS_C2S
  src/c2s/request_character_create.cpp
  inc/c2s/request_character_create.hpp
  src/c2s/request_character_delete.cpp
  inc/c2s/request_character_delete.hpp
  src/c2s/request_character_select.cpp
  inc/c2s/request_character_select.hpp
  src/c2s/request_lobby_log_off.cpp
  inc/c2s/request_lobby_log_off.hpp
  src/c2s/join_chat.cpp
  inc/c2s/join_chat.hpp
  src/c2s/leave_chat.cpp
  inc/c2s/leave_chat.hpp
  src/c2s/send_chat_msg.cpp
  inc/c2s/send_chat_msg.hpp
  src/c2s/log_off_to_lobby.cpp
  inc/c2s/log_off_to_lobby.hpp
)

set(INCLUDES
  inc/lobby_state_recv_packet_factory.hpp
  inc/preauth_state_protocol.hpp
  inc/received_packet.hpp
  inc/null_received_packet.hpp
  inc/lobby_state_protocol.hpp
  inc/ingame_state_recv_packet_factory.hpp
  inc/ingame_state_protocol.hpp
)

SOURCE_GROUP(${PROJECT_NAME} FILES ${SOURCES} ${INCLUDES})
SOURCE_GROUP(s2c FILES ${PACKETS_S2C})
SOURCE_GROUP(c2s FILES ${PACKETS_C2S})

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${INCLUDES} ${PACKETS_S2C} ${PACKETS_C2S})
target_include_directories(${PROJECT_NAME} PUBLIC inc)
target_link_libraries(${PROJECT_NAME} PUBLIC net_core utils model mmocore world)

if(BUILD_TEJK_IT_IZI_TESTS)
add_subdirectory(ut)
endif()