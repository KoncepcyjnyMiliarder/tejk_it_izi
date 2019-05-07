cmake_minimum_required(VERSION 3.11)

project(net_core)

set(MOCKS
  fake_client_acceptor.hpp
  fake_client_acceptor.cpp
  fake_net_session.hpp
  fake_net_session.cpp
  fake_net_socket.hpp
  fake_net_socket.cpp
)

#set(TESTS
#  binary_deserializer_tests.cpp
#  binary_serializer_tests.cpp
#)

add_library(${PROJECT_NAME}_mocks STATIC ${MOCKS})
target_include_directories(${PROJECT_NAME}_mocks PUBLIC .)
target_link_libraries(${PROJECT_NAME}_mocks PUBLIC ${PROJECT_NAME})

#add_executable(${PROJECT_NAME}_ut ${TESTS})
#target_link_libraries(${PROJECT_NAME}_ut PRIVATE ${PROJECT_NAME} gtest gmock gmock_main)