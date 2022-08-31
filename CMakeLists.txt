cmake_minimum_required (VERSION 3.13...3.21)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE
        "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

project(
    tcod-engine-2022  # Project name, change this as needed.
    LANGUAGES C CXX
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")  # Keep all runtime files in one directory.

file(
    GLOB_RECURSE SOURCE_FILES
    CONFIGURE_DEPENDS  # Automatically reconfigure if source files are added/removed.
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.hpp
)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Ensure the C++17 standard is available.
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# Enforce UTF-8 encoding on MSVC.
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
endif()

# Enable warnings recommended for new projects.
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

if (EMSCRIPTEN)
    # Attach data folder to Emscripten builds.
    target_link_options(${PROJECT_NAME} PRIVATE --preload-file "${CMAKE_CURRENT_SOURCE_DIR}/data@data")
    target_link_options(${PROJECT_NAME} PRIVATE -lidbfs.js)  # Enable IDBFS (browser file system.)
    configure_file(
        ${PROJECT_SOURCE_DIR}/emscripten/index.html
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/index.html
    )
endif()

# I might switch between the libtcod submodule and the stable Vcpkg version depending on my current focus.
set(USE_LIBTCOD_SUBMODULE OFF CACHE BOOL "Build libtcod from the submodule instead of from Vcpkg.")
if(USE_LIBTCOD_SUBMODULE)
    set(CMAKE_DISABLE_FIND_PACKAGE_ZLIB ON)  # Not using zlib features.
    set(BUILD_SHARED_LIBS ${WIN32})  # Builds libtcod as a shared library.
    add_subdirectory("libtcod")  # This will error if you haven't initialized libtcod the submodule!
else()
    find_package(libtcod CONFIG REQUIRED)  # Get from Vcpkg.
endif()

find_package(fmt CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main libtcod::libtcod fmt::fmt)
