cmake_minimum_required(VERSION 3.15)
project(terminal_rpg LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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