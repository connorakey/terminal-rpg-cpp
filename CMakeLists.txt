cmake_minimum_required(VERSION 4.0)
project(terminal_rpg)

set(CMAKE_CXX_STANDARD 17)

add_executable(terminal_rpg main.cpp
        item.hpp
        enemy.hpp
        chest.hpp
        player.hpp
        itemdatabase.hpp
        enemy.cpp
        player.cpp
        chest.cpp
        item.cpp
        itemdatabase.cpp
        itemdatabase.cpp
        enemydatabase.cpp
)
