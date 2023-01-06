cmake_minimum_required(VERSION 3.12)

project(Networking)

file(GLOB SERVER_SRC ${PROJECT_SOURCE_DIR}/src/server/*.cpp)

include_directories(${PROJECT_SOURCE_DIR}/src/server)

add_executable(server ${SERVER_SRC})
add_executable(client src/bin/client.cpp)

target_compile_options(client PRIVATE -fPIC)

find_package(PkgConfig)
pkg_check_modules(QT REQUIRED Qt5Widgets)

target_link_libraries(server pthread)

target_link_libraries(client ${QT_LIBRARIES})
target_include_directories(client PRIVATE ${QT_INCLUDE_DIRS})
target_compile_options(client PRIVATE ${QT_CFLAGS_OTHER})
