cmake_minimum_required(VERSION 3.11)

project(mmocore)

set(SOURCES
  src/ingame_state.cpp
  src/lobby_state.cpp
  src/mmoclient.cpp
  src/preauth_state.cpp
  src/lobby_state_recv_packet_factory.cpp
  src/ingame_state_recv_packet_factory.cpp
  src/null_received_packet.cpp
  src/world.cpp
  src/tii_entity_representative.cpp
)

set(INCLUDES
  inc/null_state.hpp
  inc/client_state.hpp
  inc/ingame_state.hpp
  inc/lobby_state.hpp
  inc/authenticator.hpp
  inc/mmoclient.hpp
  inc/preauth_state.hpp
  inc/state_transitioner.hpp
  inc/login_validator.hpp
  inc/lobby_state_recv_packet_factory.hpp
  inc/preauth_state_protocol.hpp
  inc/received_packet.hpp
  inc/null_received_packet.hpp
  inc/lobby_state_protocol.hpp
  inc/ingame_state_recv_packet_factory.hpp
  inc/ingame_state_protocol.hpp
  inc/friend_system_error_codes.hpp
  inc/world_registrar.hpp
  inc/world.hpp
  inc/task_scheduler.hpp
  inc/user_environment.hpp
  inc/tii_entity_representative.hpp
  inc/asynchronous_database_adapter.hpp
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
  src/s2c/friend_online_while_you_login.cpp
  inc/s2c/friend_online_while_you_login.hpp
  src/s2c/friend_login.cpp
  inc/s2c/friend_login.hpp
  src/s2c/friend_leave.cpp
  inc/s2c/friend_leave.hpp
  src/s2c/new_friendship.cpp
  inc/s2c/new_friendship.hpp
  src/s2c/broken_friendship.cpp
  inc/s2c/broken_friendship.hpp
  src/s2c/friendship_request.cpp
  inc/s2c/friendship_request.hpp
  src/s2c/friend_add_fail.cpp
  inc/s2c/friend_add_fail.hpp
  src/s2c/friend_removal_fail.cpp
  inc/s2c/friend_removal_fail.hpp
  src/s2c/friend_add_result.cpp
  inc/s2c/friend_add_result.hpp  
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
  src/c2s/friend_add.cpp
  inc/c2s/friend_add.hpp
  src/c2s/friend_remove.cpp
  inc/c2s/friend_remove.hpp
)

set(SOURCES_FRIEND_SYSTEM
  inc/friend_system/friend_system.hpp
  src/friend_system/friend_system.cpp
  inc/friend_system/friend_backend.hpp
  inc/friend_system/default_friend_backend.hpp
  src/friend_system/default_friend_backend.cpp
  inc/friend_system/friend_controller.hpp
  src/friend_system/friend_controller.cpp
  inc/friend_system/single_friend_request.hpp
  inc/friend_system/pending_friend_requests.hpp
  src/friend_system/pending_friend_requests.cpp
)

set(SOURCES_CHAT_SYSTEM
  inc/chat_system/chatroom.hpp
  src/chat_system/chatroom.cpp
  inc/chat_system/chat_controller.hpp
  src/chat_system/chat_controller.cpp
  inc/chat_system/chat_system.hpp
  src/chat_system/chat_system.cpp
  inc/chat_system/chat_backend.hpp
  inc/chat_system/default_chat_backend.hpp
  src/chat_system/default_chat_backend.cpp
)

SOURCE_GROUP(${PROJECT_NAME} FILES ${SOURCES} ${INCLUDES})
SOURCE_GROUP(s2c FILES ${PACKETS_S2C})
SOURCE_GROUP(c2s FILES ${PACKETS_C2S})
SOURCE_GROUP(friend_system FILES ${SOURCES_FRIEND_SYSTEM})
SOURCE_GROUP(chat_system FILES ${SOURCES_CHAT_SYSTEM})

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${INCLUDES} ${PACKETS_S2C} ${PACKETS_C2S} ${SOURCES_FRIEND_SYSTEM} ${SOURCES_CHAT_SYSTEM})
target_include_directories(${PROJECT_NAME} PUBLIC inc)
target_link_libraries(${PROJECT_NAME} PUBLIC net_core utils database_facade)

if(BUILD_TEJK_IT_IZI_TESTS)
add_subdirectory(ut)
endif()