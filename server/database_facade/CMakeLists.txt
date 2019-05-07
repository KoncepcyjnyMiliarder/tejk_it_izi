cmake_minimum_required(VERSION 3.11)

project(database_facade)

set(SOURCES
  src/postgresql_db.cpp
  src/psql_facade.cpp
)

set(INCLUDES
  inc/database_facade.hpp
  inc/postgresql_db.hpp
  inc/psql_facade.hpp
)

SOURCE_GROUP(${PROJECT_NAME} FILES ${SOURCES} ${INCLUDES})

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${INCLUDES})
target_include_directories(${PROJECT_NAME} PUBLIC inc)
target_link_libraries(${PROJECT_NAME} PUBLIC model pqxx_static)

if(BUILD_TEJK_IT_IZI_TESTS)
add_subdirectory(ut)
endif()