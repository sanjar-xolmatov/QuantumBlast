cmake_minimum_required(VERSION 4.4.4)

project(QuantumBlast LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

add_executable(quantumblast
  src/quantum.c
)

target_compile_options(quantumblast PRIVATE
  -Wall
  -Wextra
  -Werror
)

target_link_libraries(quantumblast PRIVATE raylib)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
