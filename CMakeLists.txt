cmake_minimum_required(VERSION 3.13)
project(chip8)

# Add cmake modules path
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")

include(CTest)
enable_testing()

# Create main library
add_library(emulator "")
add_subdirectory(src)

# Create executable
add_executable(chip8
    src/main.cpp
)

target_link_libraries(chip8
    PRIVATE
        emulator
)

target_compile_options(chip8
    PRIVATE
        -Wall -Wextra
)

# Create lib for executic Catch tests
add_library(Catch INTERFACE)

target_include_directories(Catch
    INTERFACE
        include
)

# Create target for executing Catch tests
add_executable(unit_tests "")
add_subdirectory(tests/src)

# Load the .cmake file to make tests available to CTest
include(ParseAndAddCatchTests)
ParseAndAddCatchTests(unit_tests)

install(
    TARGETS unit_tests
    RUNTIME DESTINATION bin
)

# This must be last
include(CPack)