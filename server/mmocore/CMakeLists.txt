cmake_minimum_required(VERSION 3.11)

project(mmocore)

set(SOURCES
  src/ingame_state.cpp
  src/lobby_state.cpp
  src/mmoclient.cpp
  src/preauth_state.cpp
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
)

SOURCE_GROUP(${PROJECT_NAME} FILES ${SOURCES} ${INCLUDES})

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${INCLUDES})
target_include_directories(${PROJECT_NAME} PUBLIC inc)
target_link_libraries(${PROJECT_NAME} PUBLIC protocol net_core utils database_facade)

if(BUILD_TEJK_IT_IZI_TESTS)
add_subdirectory(ut)
endif()