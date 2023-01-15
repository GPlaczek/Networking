cmake_minimum_required(VERSION 3.12)

project(Networking)

file(GLOB SERVER_SRC ${PROJECT_SOURCE_DIR}/src/server/*.cpp)
file(GLOB CLIENT_SRC ${PROJECT_SOURCE_DIR}/src/client/*.cpp)

include_directories(${PROJECT_SOURCE_DIR}/src/server)
include_directories(${PROJECT_SOURCE_DIR}/src/client)

add_executable(server ${SERVER_SRC})
add_executable(client ${CLIENT_SRC})

target_compile_options(client PRIVATE -fPIC)

find_package(Qt5Widgets REQUIRED)
find_package(QT NAMES Qt6 Qt5      COMPONENTS Core Widgets Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets Network REQUIRED)
find_package(PkgConfig)
pkg_check_modules(QT REQUIRED Qt5Widgets)

target_link_libraries(server pthread)
target_link_libraries(client ${QT_LIBRARIES})
target_link_libraries(client Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)

target_include_directories(client PRIVATE ${QT_INCLUDE_DIRS})
target_compile_options(client PRIVATE ${QT_CFLAGS_OTHER})
