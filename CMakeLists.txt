cmake_minimum_required(VERSION 3.15)
project(cpuRE VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

if (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
  set(architecture "x64")
else()
  set(architecture "x86")
endif()

message(STATUS "${CMAKE_SYSTEM} ${architecture}")

if (CMAKE_HOST_UNIX)
    add_compile_options(-g)
    add_compile_options(-fpic)
    # add_compile_options(-Wall)
    # add_compile_options(-Wextra)
endif()

set(library_dir ${CMAKE_SOURCE_DIR}/lib)
set(runtime_dir ${CMAKE_SOURCE_DIR}/bin)
set(assets_dir ${CMAKE_SOURCE_DIR}/assets)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${library_dir})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${library_dir})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${runtime_dir})

configure_file(config.in config.h)

include_directories(thirdparty)

file(GLOB_RECURSE SRCS src/*.cpp src/*.c src/*.h)
add_executable(${PROJECT_NAME} ${SRCS})

target_link_directories(${PROJECT_NAME} PRIVATE thirdparty/lib/${architecture})
target_link_libraries(${PROJECT_NAME} glfw3)

build_command(BUILD_COMMAND_LINE CONFIGURATION ${CMAKE_BUILD_TYPE} TARGET ALL_BUILD)
message(STATUS "Using compiler ${CMAKE_CXX_COMPILER}")
message(STATUS "Build command line: ${BUILD_COMMAND_LINE}")