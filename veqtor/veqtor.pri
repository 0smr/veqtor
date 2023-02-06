QML_IMPORT_PATH += $${PWD}/..
QML2_IMPORT_PATH += $${PWD}/..
QML_DESIGNER_IMPORT_PATH += $${PWD}/..

RESOURCES += $${PWD}/veqtor.qrc

include($$PWD/../qnanopainter/libqnanopainter/include.pri)

QT += xml

CONFIG -= c++11
CONFIG += c++17 qmltypes

QML_IMPORT_NAME = veqtor
QML_IMPORT_MAJOR_VERSION = 1

#PRECOMPILED_HEADER = $$PWD/pch.h
#HEADERS += $$PWD/pch.h

HEADERS += \
    $$PWD/elements/container.h \
    $$PWD/elements/element.h \
    $$PWD/elements/link.h \
    $$PWD/elements/svg.h \
    $$PWD/elements/graphic.h \
    $$PWD/elements/eline.h \
    $$PWD/elements/epath.h \
    $$PWD/elements/unknown.h \
    $$PWD/shapes/apoint.h \
    $$PWD/shapes/ellipse.h \
    $$PWD/shapes/line.h \
    $$PWD/shapes/path.h \
    $$PWD/shapes/rectangle.h \
    $$PWD/shapes/shape.h \
    $$PWD/shapes/shapes.h \
    $$PWD/utils/svgtools.h \
    $$PWD/utils/tools.h \
    $$PWD/veqtor.h \
    $$PWD/nanopen.h \
    $$PWD/painthelper.h \
    $$PWD/nanopainter.h

SOURCES += \
    $$PWD/elements/element.cpp \
    $$PWD/elements/epath.cpp \
    $$PWD/elements/graphic.cpp \
    $$PWD/shapes/apoint.cpp \
    $$PWD/shapes/ellipse.cpp \
    $$PWD/shapes/line.cpp \
    $$PWD/shapes/path.cpp \
    $$PWD/shapes/rectangle.cpp \
    $$PWD/shapes/shape.cpp \
    $$PWD/utils/svgtools.cpp \
    $$PWD/utils/tools.cpp \
    $$PWD/veqtor.cpp \
    $$PWD/painthelper.cpp \
    $$PWD/nanopainter.cpp
