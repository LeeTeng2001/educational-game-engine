cmake_minimum_required(VERSION 3.21)
project(game_prog_in_cpp)
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_FIND_DEBUG_MODE 1)  # Debug find cmake

# Find dependencies ------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/external/cmake_modules/")

find_package(SDL2 REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(SDL2TTF REQUIRED)  # cmake find in cmake_modules

# Check dependencies -----------------------------------------------------
if(NOT ${SDL2})
    message(FATAL_ERROR "SDL2 library not found")
endif()
if(NOT ${RapidJSON})
    message(FATAL_ERROR "RapidJSON library not found")
endif()
if(NOT ${SDL2TTF})
    message(FATAL_ERROR "SDL2 font library not found")
endif()

# Include dependencies headers ----------------------------------------------
include_directories(
        ${SDL2_INCLUDE_DIRS}
        ${SDL2TTF_INCLUDE_DIR}
        ${RAPIDJSON_INCLUDE_DIRS}
        external/include
        )

# You'll probably want to link your FMOD library differently, use message to debug
file(GLOB FMOD_LIBRARIES "external/lib/fmod/*.dylib")
# message("FMOD_LIBRARIES = ${FMOD_LIBRARIES}")

# Add projects ------------------------------------------------------------
add_subdirectory("src")
