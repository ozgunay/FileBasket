include(../qmake-target-platform.pri)
include(../qmake-destination-path.pri)

include(gtest_dependency.pri)

DESTDIR = $$PWD/../binaries/$$DESTINATION_PATH
OBJECTS_DIR = $$PWD/build/$$DESTINATION_PATH/.obj
MOC_DIR = $$PWD/build/$$DESTINATION_PATH/.moc
RCC_DIR = $$PWD/build/$$DESTINATION_PATH/.qrc
UI_DIR = $$PWD/build/$$DESTINATION_PATH/.ui

TEMPLATE = app
CONFIG += c++14
CONFIG += console
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

INCLUDEPATH += source

HEADERS += \
        source/models/tst_connectiontest.h

SOURCES += \
        main.cpp
