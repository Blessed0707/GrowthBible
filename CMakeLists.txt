cmake_minimum_required(VERSION 3.15)

project(MultiFileProject VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =================================================================
# GLOBAL FILE COLLECTIONS
# =================================================================
file(GLOB_RECURSE SRC_FILES 
    "src/*.cpp" 
    "src/features/verse/*.cpp"
    "src/features/sqlite/*.cpp"
    "src/features/navigation/*.cpp"
<<<<<<< HEAD
    "src/features/console_ui/*.cpp"
=======
    "src/features/chapter/*.cpp"
    "src/features/wordsearch/*.cpp"
>>>>>>> d6f8b7530a3e1420d7316962cf45f95eeaf01bc6
)

list(APPEND SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/sqlite_engine/sqlite3.c")

file(GLOB_RECURSE TEST_FILES "src_tests/*.cpp")


# =================================================================
# TARGET 1: THE MAIN APP
# =================================================================
add_executable(MyExecutable main.cpp ${SRC_FILES}
    "${CMAKE_CURRENT_SOURCE_DIR}/src/features/verse/WordSearch.cpp"
)

target_include_directories(MyExecutable PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/includes"
    "${CMAKE_CURRENT_SOURCE_DIR}/sqlite_engine"
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/sqlite"
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/verse" 
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/navigation" 
<<<<<<< HEAD
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/console_ui"


=======
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/chapter" 
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/wordsearch" 
>>>>>>> d6f8b7530a3e1420d7316962cf45f95eeaf01bc6
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
<<<<<<< HEAD
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/console_ui"

=======
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/chapter" 
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/wordsearch" 
>>>>>>> d6f8b7530a3e1420d7316962cf45f95eeaf01bc6
)


# =================================================================
# FILE COPYING
# =================================================================
add_custom_command(TARGET MyExecutable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/text"
    "$<TARGET_FILE_DIR:MyExecutable>/text"
)

add_custom_command(TARGET MyExecutable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/src/features/sqlite/kjvbible.db"
    "$<TARGET_FILE_DIR:MyExecutable>/kjvbible.db"
)

add_custom_command(TARGET TestExecutable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/text"
    "$<TARGET_FILE_DIR:TestExecutable>/text"
)

add_custom_command(TARGET TestExecutable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/src/features/sqlite/kjvbible.db"
    "$<TARGET_FILE_DIR:TestExecutable>/kjvbible.db"
)


# =================================================================
# SYSTEM LINKING
# =================================================================
find_package(Threads REQUIRED)
target_link_libraries(MyExecutable PRIVATE Threads::Threads)
target_link_libraries(TestExecutable PRIVATE Threads::Threads)