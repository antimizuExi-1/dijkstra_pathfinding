cmake_minimum_required(VERSION 4.0)
project(DataStructure C)

set(CMAKE_C_STANDARD 17)

add_subdirectory(raylib)

add_compile_options(-mwindows)
add_executable(DataStructure
        WIN32
        main.c
        graph.c
)
target_link_libraries(DataStructure raylib_static)