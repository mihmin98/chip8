target_sources(unit_tests
    PRIVATE
        tests-main.cpp
        tests-chip8.cpp
)

target_include_directories(unit_tests
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries(unit_tests
    Catch
    emulator
)

target_compile_options(unit_tests
    PRIVATE    
        -Wall -Wextra
)