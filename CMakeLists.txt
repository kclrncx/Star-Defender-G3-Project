cmake_minimum_required(VERSION 3.16)

project(StarShooter C)

set(CMAKE_C_STANDARD 11)

find_package(raylib REQUIRED)

add_executable(StarShooter StarShooterGame.c ./src/enemyManager/enemyBehavior.c ./src/bulletManager/bulletManager.c)

target_link_libraries(StarShooter raylib)
