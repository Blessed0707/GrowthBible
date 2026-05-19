cmake_minimum_required(VERSION 3.15)
project(MultiFileProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =================================================================
# GLOBAL FILE COLLECTIONS
# =================================================================
# Collect your core logic files (VerseFinder.cpp, etc.)
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

# Collect your test logic files (test.cpp, etc.)
file(GLOB_RECURSE TEST_FILES "src_tests/*.cpp")


# =================================================================
# TARGET 1: THE MAIN APP
# =================================================================
# This target uses main.cpp and your core logic
add_executable(MyExecutable main.cpp ${SRC_FILES})

# Give it access to its headers folder
target_include_directories(MyExecutable PRIVATE includes)


# =================================================================
# TARGET 2: THE TEST SUITE
# =================================================================
# This target DOES NOT use main.cpp. It uses test.cpp and your core logic.
add_executable(TestExecutable ${TEST_FILES} ${SRC_FILES})

# Give tests access to BOTH normal headers and test headers
target_include_directories(TestExecutable PRIVATE includes test_includes)


# =================================================================
# FILE COPYING (The Bulletproof Fix)
# =================================================================
# This copies your 'text' folder right next to MyExecutable.exe, wherever it builds
add_custom_command(TARGET MyExecutable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/text"
    "$<TARGET_FILE_DIR:MyExecutable>/text"
)

# This does the same for your TestExecutable.exe
add_custom_command(TARGET TestExecutable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/text"
    "$<TARGET_FILE_DIR:TestExecutable>/text"
)