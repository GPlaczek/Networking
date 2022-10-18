cmake_minimum_required(VERSION 3.12)

project(Networking)

add_executable(server src/bin/server.cpp)

add_executable(client src/bin/client.cpp)
