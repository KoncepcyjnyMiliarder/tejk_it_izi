cmake_minimum_required(VERSION 3.11)

project(commandline_client)

find_package (Threads)

add_executable(${PROJECT_NAME} commandline_client.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC mmocore Threads::Threads)
