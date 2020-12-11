cmake_minimum_required(VERSION 3.10)
enable_language(C) # what???

set(RAYLIB_FOLDER "raylib-3.0.0")

# add the raylib project
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games
add_subdirectory(${RAYLIB_FOLDER}) # defines the raylib library object

# set the project name
project(Raylib_Particles)

# add the executable
add_executable(Particles main.c particle_world.c)
target_include_directories(Particles PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${RAYLIB_FOLDER}/src)
target_link_libraries(Particles raylib)
