find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

target_sources(emulator
    PUBLIC
        Audio.cpp
        Chip8.cpp
        Emulator.cpp
)

target_include_directories(emulator
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)

target_compile_options(emulator
    PRIVATE
        -Wall -Wextra
)

target_link_libraries(emulator
    PRIVATE
        ${SDL2_LIBRARY}
)