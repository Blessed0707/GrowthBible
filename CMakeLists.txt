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
    "src/features/chapter/*.cpp"
    "src/features/wordsearch/*.cpp"
)

list(APPEND SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/sqlite_engine/sqlite3.c")

file(GLOB_RECURSE TEST_FILES "src_tests/*.cpp")

# =================================================================
# TARGET 1: THE MAIN APP (CLI)
# =================================================================
add_executable(MyExecutable main.cpp ${SRC_FILES})

target_include_directories(MyExecutable PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/includes"
    "${CMAKE_CURRENT_SOURCE_DIR}/sqlite_engine"
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/sqlite"
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/verse" 
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/navigation" 
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/chapter" 
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/wordsearch" 
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
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/chapter" 
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/wordsearch" 
)

# =================================================================
# TARGET 3: THE GUI APP (wxWidgets)
# =================================================================
set(wxWidgets_ROOT_DIR C:/wxWidgets)
set(wxWidgets_LIB_DIR C:/wxWidgets/lib/gcc_lib)
set(wxWidgets_CONFIGURATION mswu)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

add_executable(GrowthBibleGUI WIN32 
    main_gui.cpp
    ${SRC_FILES}
)

target_include_directories(GrowthBibleGUI PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/includes"
    "${CMAKE_CURRENT_SOURCE_DIR}/sqlite_engine"
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/sqlite"
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/verse" 
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/navigation" 
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/chapter" 
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/features/wordsearch" 
)

target_link_libraries(GrowthBibleGUI PRIVATE ${wxWidgets_LIBRARIES})

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

add_custom_command(TARGET GrowthBibleGUI POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/src/features/sqlite/holybible.db"
    "$<TARGET_FILE_DIR:GrowthBibleGUI>/holybible.db"
)

add_custom_target(run
    COMMAND GrowthBibleGUI
    DEPENDS GrowthBibleGUI
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# =================================================================
# SYSTEM LINKING
# =================================================================
find_package(Threads REQUIRED)
target_link_libraries(MyExecutable PRIVATE Threads::Threads)
target_link_libraries(TestExecutable PRIVATE Threads::Threads)
