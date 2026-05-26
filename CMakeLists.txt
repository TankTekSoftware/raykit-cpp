cmake_minimum_required(VERSION 3.25)
project(raykit-cpp LANGUAGES CXX)


# --- Compile Commands Settings --- #
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(COMPILE_COMMANDS_TOP ${CMAKE_SOURCE_DIR}/compile_commands.json)

add_custom_target(export_compile_commands_top ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${COMPILE_COMMANDS_TOP}
    COMMENT "Exporting compile_commands.json to top level"
    VERBATIM
)

# --- Configure C++ --- #
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Configure Output Directory --- #
string(TOLOWER "${PLATFORM}" PLATFORM_DIR)
string(TOLOWER "${CMAKE_BUILD_TYPE}" BUILD_TYPE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PLATFORM_DIR}-${BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PLATFORM_DIR}-${BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PLATFORM_DIR}-${BUILD_TYPE})

if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()

# --- Fetch Raylib --- #
include(FetchContent)

message(STATUS "Fetching raylib...")
FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "6.0"
)

FetchContent_Declare(
    physfs
    GIT_REPOSITORY "https://github.com/icculus/physfs.git"
    GIT_TAG "main"
)

FetchContent_MakeAvailable(raylib physfs)

# --- Optional Steamworks Integration --- #
set(STEAMWORKS_SDK_ROOT
    ${CMAKE_SOURCE_DIR}/vendor/steamworks/
)

# Check if Steamworks SDK exists
if(EXISTS ${STEAMWORKS_SDK_ROOT})
    set(USE_STEAMWORKS ON)
    message(STATUS "Steamworks SDK found, enabling Steamworks integration")
    
    set(STEAMWORKS_INCLUDE_DIR
        ${STEAMWORKS_SDK_ROOT}/public
    )

    # Platform-specific library
    if (WIN32)
        set(STEAMWORKS_LIB
            ${STEAMWORKS_SDK_ROOT}/redistributable_bin/win64/steam_api64.lib
        )
    elseif (APPLE)
        set(STEAMWORKS_LIB
            ${STEAMWORKS_SDK_ROOT}/redistributable_bin/osx/libsteam_api.dylib
        )
    elseif (UNIX)
        set(STEAMWORKS_LIB
            ${STEAMWORKS_SDK_ROOT}/redistributable_bin/linux64/libsteam_api.so
        )
    endif()
else()
    set(USE_STEAMWORKS OFF)
    message(STATUS "Steamworks SDK not found, building without Steamworks integration")
endif()

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

# Add Steamworks if available
if(USE_STEAMWORKS)
    target_include_directories(${PROJECT_NAME} 
        PRIVATE 
            ${STEAMWORKS_INCLUDE_DIR}
    )
    
    target_link_libraries(${PROJECT_NAME} 
        PRIVATE 
            ${STEAMWORKS_LIB}
    )
    
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE 
            USE_STEAMWORKS
    )
    
    # Copy Steamworks DLLs/dylibs
    if (WIN32)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${STEAMWORKS_SDK_ROOT}/redistributable_bin/win64/steam_api64.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    endif()

    if (UNIX AND NOT APPLE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${STEAMWORKS_SDK_ROOT}/redistributable_bin/linux64/libsteam_api.so
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    endif()

    if(APPLE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${STEAMWORKS_SDK_ROOT}/redistributable_bin/osx/libsteam_api.dylib
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    endif()
endif()

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        ASSET_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
        PAK_NAME="${PROJECT_NAME}.pck"
        PROJECT_NAME="${PROJECT_NAME}"
)

set(PAK_OUTPUT "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.pck")

file(GLOB_RECURSE ASSET_FILES
    LIST_DIRECTORIES false
    CONFIGURE_DEPENDS
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/*
)
list(FILTER ASSET_FILES EXCLUDE REGEX "(^|/)\\.DS_Store$")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -f "${PAK_OUTPUT}"
    COMMAND ${CMAKE_COMMAND} -E tar "cf" "${PAK_OUTPUT}" --format=zip -- ${ASSET_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/assets
    COMMENT "Packaging assets into ${PROJECT_NAME}.pck"
    VERBATIM
)
