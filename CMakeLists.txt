cmake_minimum_required(VERSION 3.5)
project(libactor)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -march=morello -mabi=purecap -Xclang -cheri-bounds=subobject-safe")

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(tests)
