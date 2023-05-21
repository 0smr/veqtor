cmake_minimum_required(VERSION 3.26.0)
project(veqtor VERSION 0.1.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Quick)
if(QT_VERSION_MAJOR EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS Core Quick Xml Qml OpenGL OpenGLWidgets)
    set(QT_LIBS Qt::Core Qt::Quick Qt6::Xml Qt::Qml Qt::OpenGL)
else()
    find_package(Qt5 REQUIRED COMPONENTS Core Quick Xml Qml Script)
    set(QT_LIBS Qt5::Core Qt5::Quick Qt5::Xml Qt5::Qml Qt5::Script)
endif()

file(GLOB veqtor_files "*/*.cpp" "*/*.h" "*.cpp" "*.h")

add_library(veqtor veqtor.qrc ${veqtor_files})
target_include_directories(veqtor PUBLIC .)

target_link_libraries(${PROJECT_NAME} ${QT_LIBS})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../qnanopainter/libqnanopainter/ ${CMAKE_CURRENT_BINARY_DIR}/libqnanopainter)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../qnanopainter/libqnanopainter/nanovg ${CMAKE_CURRENT_BINARY_DIR}/libqnanopainter/nanovg)

target_link_libraries(${PROJECT_NAME} qnanopainter nanovg)
target_compile_options(qnanopainter PUBLIC -UQNANO_DEBUG)
