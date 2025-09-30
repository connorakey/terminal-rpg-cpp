cmake_minimum_required(VERSION 3.15)
project(terminal_rpg LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# Fetch Catch2 (testing tool for C++)
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.11.0  # Use latest stable release or adjust as needed
)
FetchContent_MakeAvailable(catch2)

set(SOURCES
    src/main.cpp
    src/enemies/enemy.cpp
    src/enemies/enemydatabase.cpp
    src/player/player.cpp
    src/chest/chest.cpp
    src/items/item.cpp
    src/items/itemdatabase.cpp
    src/levels/leveldatabase.cpp
)

set(HEADERS
    src/items/item.hpp
    src/items/itemdatabase.hpp
    src/enemies/enemy.hpp
    src/chest/chest.hpp
    src/player/player.hpp
    src/levels/leveldatabase.hpp
)

add_executable(terminal_rpg
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(terminal_rpg PRIVATE
    src/
)

# Add test executables
add_executable(tests
    tests/test_chest.cpp
    tests/test_enemy.cpp
    src/chest/chest.cpp
    src/items/item.cpp
    src/enemies/enemy.cpp
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

target_include_directories(tests PRIVATE
    src/
    ${catch2_SOURCE_DIR}/include
)

enable_testing()

include(CTest)
include(Catch)

catch_discover_tests(tests)
