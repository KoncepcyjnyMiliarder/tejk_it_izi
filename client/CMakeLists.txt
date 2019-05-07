cmake_minimum_required(VERSION 3.11)

project(commandline_client)

add_executable(${PROJECT_NAME} commandline_client.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC net_core protocol)
