cmake_minimum_required(VERSION 3.26)

project(example-2)

set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON" FORCE)

set(CMAKE_AUTOGEN_PARALLEL AUTO)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 CONFIG REQUIRED COMPONENTS gui qml quick script)
qt5_add_resources(resource_file qml.qrc)

add_executable(example-2
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${resource_file}
)

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Qml)

# Add Veqtor as a subdirectory.
# Note: If the vector directory is a subset of your project, skip the second part of the add_subdirectory command and instead use:
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/veqtor)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../veqtor/ ${CMAKE_CURRENT_BINARY_DIR}/veqtor)
# Here, we link the vector to our project.
target_link_libraries(${PROJECT_NAME} veqtor)
