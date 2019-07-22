cmake_minimum_required(VERSION 3.11)

project(tejk_it_izi)

find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME} src/tejk_it_izi.cpp src/main.cpp inc/tejk_it_izi.hpp)
target_include_directories(${PROJECT_NAME} PRIVATE inc)
target_link_libraries(${PROJECT_NAME} PUBLIC mmocore Threads::Threads)
