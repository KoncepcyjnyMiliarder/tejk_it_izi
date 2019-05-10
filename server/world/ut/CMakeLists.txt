cmake_minimum_required(VERSION 3.11)

project(world)

set(MOCKS
  fake_user_environment.hpp
  mock_friend_component.hpp
  fake_user_environment.cpp
)

#set(TESTS
#)

add_library(${PROJECT_NAME}_mocks STATIC ${MOCKS})
target_include_directories(${PROJECT_NAME}_mocks PUBLIC .)
target_link_libraries(${PROJECT_NAME}_mocks PUBLIC ${PROJECT_NAME}
  gtest
  gmock
  gmock_main
  database_facade_mocks
  net_core_mocks
  utils_mocks)

#later
#add_executable(${PROJECT_NAME}_ut ${TESTS})
#target_link_libraries(${PROJECT_NAME}_ut PRIVATE ${PROJECT_NAME} gtest gmock gmock_main)