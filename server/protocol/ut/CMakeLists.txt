cmake_minimum_required(VERSION 3.11)

project(protocol)

set(TESTS
  join_chat_tests.cpp
  leave_chat_tests.cpp
  log_off_to_lobby_tests.cpp
  request_character_create_tests.cpp
  request_character_delete_tests.cpp
  request_character_select_tests.cpp
  request_lobby_log_off_tests.cpp
  send_chat_msg_tests.cpp
)

add_executable(${PROJECT_NAME}_ut ${TESTS})
target_link_libraries(${PROJECT_NAME}_ut PRIVATE ${PROJECT_NAME} gtest gmock gmock_main
  database_facade_mocks
  net_core_mocks
  utils_mocks
)