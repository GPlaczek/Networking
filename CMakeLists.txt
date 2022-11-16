cmake_minimum_required(VERSION 3.12)

project(Networking)

add_executable(server src/server/server.cpp)
add_executable(client src/bin/client.cpp)

find_package(PkgConfig)
pkg_check_modules(QT REQUIRED Qt5Widgets)

target_link_libraries(client ${QT_LIBRARIES})
target_include_directories(client PRIVATE ${QT_INCLUDE_DIRS})
target_compile_options(client PRIVATE ${QT_CFLAGS_OTHER})
