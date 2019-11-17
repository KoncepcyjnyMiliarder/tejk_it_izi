cmake_minimum_required(VERSION 3.11)

include(${PROJECT_SOURCE_DIR}/set_warning_level.cmake)

project(loginserver)

find_package(Threads REQUIRED)

set(SOURCES
  src/loginserver.cpp
  src/client_session.cpp
  src/gameserver_session.cpp
)

set(INCLUDES
  inc/loginserver.hpp
  inc/client_session.hpp
  inc/gameserver_session.hpp
  inc/login_protocol.hpp
  inc/loginserver_data.hpp
  inc/gameserver.hpp
)

add_executable(${PROJECT_NAME} ${SOURCES} src/main.cpp ${INCLUDES})
target_include_directories(${PROJECT_NAME} PRIVATE inc)
target_link_libraries(${PROJECT_NAME} PUBLIC mmocore Threads::Threads)
set_warning_level(${PROJECT_NAME})
