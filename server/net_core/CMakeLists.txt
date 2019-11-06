cmake_minimum_required(VERSION 3.11)

include(${PROJECT_SOURCE_DIR}/set_warning_level.cmake)

project(net_core)

find_package(Boost REQUIRED COMPONENTS system date_time regex)

set(SOURCES
  src/boost_socket.cpp
  src/boost_acceptor.cpp
  src/net_session.cpp
)

set(INCLUDES
  inc/client_acceptor.hpp
  inc/net_session.hpp
  inc/net_socket.hpp
  inc/boost_acceptor.hpp
  inc/boost_socket.hpp
  inc/null_socket.hpp
)

SOURCE_GROUP(${PROJECT_NAME} FILES ${SOURCES} ${INCLUDES})

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${INCLUDES})
target_include_directories(${PROJECT_NAME} PUBLIC inc ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})
set_warning_level(${PROJECT_NAME})

if(BUILD_TEJK_IT_IZI_TESTS)
add_subdirectory(ut)
endif()
