cmake_minimum_required(VERSION 3.14)

project(StarShooter C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Create executable
add_executable(StarShooter
    StarShooterGame.c
    src/enemyManager/enemyBehavior.c
    src/bulletManager/bulletManager.c
)

# Include raylib headers
target_include_directories(StarShooter PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
)

# Link raylib library
target_link_directories(StarShooter PRIVATE
    "${CMAKE_SOURCE_DIR}/lib"
)

target_link_libraries(StarShooter PRIVATE
    raylib
    winmm
    gdi32
    opengl32
)
target_link_options(StarShooter PRIVATE
    "-Wl,--defsym,stat64i32=_stat64i32"
)
