cmake_minimum_required(VERSION 3.15)

# CRITICAL FIX: Explicitly enable BOTH CXX (C++) and C languages at the project level
project(MultiFileProject VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =================================================================
# GLOBAL FILE COLLECTIONS
# =================================================================
# 1. Collect all your C++ files recursively
file(GLOB_RECURSE SRC_FILES 
    "src/*.cpp" 
    "src/features/verse/*.cpp"
    "src/features/sqlite/*.cpp"
    "src/features/navigation/*.cpp"
)

# 2. CRITICAL FIX: Explicitly append the sqlite3.c engine using its absolute path
list(APPEND SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/sqlite_engine/sqlite3.c")

# Collect your test logic files (test.cpp, etc.)
file(GLOB_RECURSE TEST_FILES "src_tests/*.cpp")


# =================================================================
# TARGET 1: THE MAIN APP
# =================================================================
add_executable(MyExecutable main.cpp ${SRC_FILES})

target_include_directories(MyExecutable PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/includes"
    "${CMAKE_CURRENT_SOURCE_DIR}/sqlite_engine"
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/sqlite"
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/verse" 
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/navigation" 


)


# =================================================================
# TARGET 2: THE TEST SUITE
# =================================================================
add_executable(TestExecutable ${TEST_FILES} ${SRC_FILES})

target_include_directories(TestExecutable PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/includes"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_includes"
    "${CMAKE_CURRENT_SOURCE_DIR}/sqlite_engine"
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/sqlite"
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/verse" 
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/navigation" 
)


# =================================================================
# FILE COPYING (This makes database paths generic)
# =================================================================
add_custom_command(TARGET MyExecutable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/text"
    "$<TARGET_FILE_DIR:MyExecutable>/text"
)

add_custom_command(TARGET MyExecutable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/src/features/sqlite/holybible.db"
    "$<TARGET_FILE_DIR:MyExecutable>/holybible.db"
)

add_custom_command(TARGET TestExecutable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/text"
    "$<TARGET_FILE_DIR:TestExecutable>/text"
)

add_custom_command(TARGET TestExecutable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/src/features/sqlite/holybible.db"
    "$<TARGET_FILE_DIR:TestExecutable>/holybible.db"
)


# =================================================================
# SYSTEM LINKING
# =================================================================
find_package(Threads REQUIRED)
target_link_libraries(MyExecutable PRIVATE Threads::Threads)
target_link_libraries(TestExecutable PRIVATE Threads::Threads)