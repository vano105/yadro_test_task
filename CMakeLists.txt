cmake_minimum_required(VERSION 3.5)

set(SOURCES 
  src/club/club.cpp
  src/event/event.cpp
  src/time/time.cpp
  src/utils/utils.cpp
  src/main.cpp
)

add_executable(yadro ${SOURCES})

set_target_properties(
  yadro PROPERTIES
  CXX_STANDART 20
  CXX_STANDART_REQUIRED ON
)

project(yadro VERSION 0.0.1 LANGUAGES C CXX)
