TEMPLATE = lib
QT += qml quick quickcontrols2
CONFIG += qt plugin

include(veqtor.pri)

qmltypes.target = qmltypes
qmltypes.commands = $$[QT_INSTALL_BINS]/qmlplugindump Hive 1.0 $$QMAKE_RESOLVED_TARGET > $$PWD/hive.qmltypes
qmltypes.depends = $$QMAKE_RESOLVED_TARGET

QMAKE_EXTRA_TARGETS += qmltypes
