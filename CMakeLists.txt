cmake_minimum_required(VERSION 3.10)

project(untitled LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

enable_testing()

# Измените эту строку
set(GTEST_ROOT "/usr/src/gtest")
find_package(GTest REQUIRED)
find_package(Qt5Core REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(untitled
  main.cpp
)

target_link_libraries(untitled Qt5::Core)

target_link_libraries(untitled ${GTEST_LIBRARIES} pthread)

add_test(NAME Test1 COMMAND untitled)
