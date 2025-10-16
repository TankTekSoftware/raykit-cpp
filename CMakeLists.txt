cmake_minimum_required(VERSION 3.25)
project(raylib-cpp LANGUAGES CXX)

# -- Compile Commands -- #
set(COMPILE_COMMANDS_TOP ${CMAKE_SOURCE_DIR}/compile_commands.json)

add_custom_target(export_compile_commands_top ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${COMPILE_COMMANDS_TOP}
    COMMENT "Exporting compile_commands.json to top level"
    VERBATIM
)

# -- Configure C++ -- #
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()

# -- Fetch Raylib -- #
include(FetchContent)

message(STATUS "Fetching raylib...")
FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "5.5"
)

FetchContent_MakeAvailable(raylib)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)

# -- Add Executable -- #
file(GLOB_RECURSE SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

# -- Include Directories & Link Libraries -- #
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        "${CMAKE_CURRENT_SOURCE_DIR}/src/"
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        raylib
        imgui
)

# -- Compile Definitions -- #
target_compile_definitions(${PROJECT_NAME}
    PRIVATE 
        ASSET_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
)
