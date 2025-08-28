cmake_minimum_required(VERSION 3.12)
project(2048 VERSION 1.0.3 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add compile options
if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include directories
include_directories(include)

# Source files
set(GAME_SOURCES
    src/game.c
    src/ui.c
)

# Create library
add_library(game_lib STATIC ${GAME_SOURCES})

# Main executable
add_executable(2048 src/main.c)
target_link_libraries(2048 game_lib)

# Test executable
add_executable(2048_test src/main.c)
target_link_libraries(2048_test game_lib)
target_compile_definitions(2048_test PRIVATE TEST_MODE)

# Enable testing
enable_testing()

# Add test
add_test(NAME unit_tests COMMAND 2048 test)

# Installation
install(TARGETS 2048 DESTINATION bin)

# Package configuration
set(CPACK_PACKAGE_NAME "2048")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Console version of the game 2048")
set(CPACK_PACKAGE_VENDOR "Maurits van der Schee")
include(CPack)