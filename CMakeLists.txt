cmake_minimum_required(VERSION 4.0)
project(terminal_rpg)

set(CMAKE_CXX_STANDARD 17)

add_executable(terminal_rpg src/main.cpp
        src/items/item.hpp
        src/enemies/enemy.hpp
        src/chest/chest.hpp
        src/player/player.hpp
        src/items/itemdatabase.hpp
        src/enemies/enemy.cpp
        src/player/player.cpp
        src/chest/chest.cpp
        src/items/item.cpp
        src/items/itemdatabase.cpp
        src/items/itemdatabase.cpp
        src/enemies/enemydatabase.cpp
        src/levels/leveldatabase.hpp
        src/levels/leveldatabase.cpp
)
