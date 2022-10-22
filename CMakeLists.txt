cmake_minimum_required(VERSION 3.15)
project(cpuRE VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(thirdparty)

file(GLOB_RECURSE SRCS src/*.cpp src/*.c src/*.h)
add_executable(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL)

build_command(BUILD_COMMAND_LINE CONFIGURATION ${CMAKE_BUILD_TYPE} TARGET ALL_BUILD)
message(STATUS "Using compiler ${CMAKE_CXX_COMPILER}")
message(STATUS "Build command line: ${BUILD_COMMAND_LINE}")