cmake_minimum_required(VERSION 3.29)
project(NetworkedConnect4)

set(CMAKE_CXX_STANDARD 20)

add_executable(NetworkedConnect4 main.cpp
        
        game.h
        game.cpp
        serialization.h
        serialization.cpp
        endianness.cpp
 "GameLogic/board.h" "GameLogic/board.cpp")

add_subdirectory(raylib)

add_subdirectory(simplesock)
target_link_libraries(NetworkedConnect4 raylib simplesocklib)

if(WIN32)
  target_link_libraries(NetworkedConnect4 wsock32 ws2_32)
endif()