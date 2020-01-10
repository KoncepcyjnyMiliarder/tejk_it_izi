cmake_minimum_required(VERSION 3.11)

project(mmocore)

find_package(GTest CONFIG REQUIRED)

set(MOCKS
  fake_user_environment.hpp
  mock_friend_component.hpp
  fake_user_environment.cpp
  fake_database_facade.cpp
  fake_database_facade.hpp
)

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

add_library(${PROJECT_NAME}_mocks STATIC ${MOCKS})
target_include_directories(${PROJECT_NAME}_mocks PUBLIC .)
target_link_libraries(${PROJECT_NAME}_mocks PUBLIC ${PROJECT_NAME} database_facade_mocks utils_mocks net_core_mocks)

add_executable(${PROJECT_NAME}_ut ${TESTS})
target_link_libraries(${PROJECT_NAME}_ut PRIVATE ${PROJECT_NAME} ${PROJECT_NAME}_mocks GTest::gmock_main)