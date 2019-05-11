#-------------------------------------------------
#
# Project created by QtCreator 2019-04-06T21:52:53
#
#-------------------------------------------------
include(../qmake-target-platform.pri)
include(../qmake-destination-path.pri)

#Silinecek!
DEFINES += UNICODE

DESTDIR = $$PWD/../binaries/$$DESTINATION_PATH
OBJECTS_DIR = $$PWD/build/$$DESTINATION_PATH/.obj
MOC_DIR = $$PWD/build/$$DESTINATION_PATH/.moc
RCC_DIR = $$PWD/build/$$DESTINATION_PATH/.qrc
UI_DIR = $$PWD/build/$$DESTINATION_PATH/.ui

QT       -= gui

TARGET = fb-lib
TEMPLATE = lib

CONFIG += c++14

DEFINES += FBLIB_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += source \
    C:/Libraries/libssh-0.8.7/include
LIBS += -LC:/Libraries/libssh-0.8.7/build/src/Debug/ -lssh
DEPENDPATH += C:/Libraries/libssh-0.8.7/include

INCLUDEPATH += C:/Libraries/boost_1_70_0
LIBS += -LC:/Libraries/boost_1_70_0/stage/msvc/x64/
DEPENDPATH += C:/Libraries/boost_1_70_0
DEFINES += BOOST_LOG_DYN_LINK

SOURCES += source/logger/fb-logger.cpp \
    source/controllers/master-controller.cpp \
    source/framework/command.cpp \
    source/controllers/command-controller.cpp \
    source/SFTP/sftpconnector.cpp \
    source/models/ConnectionController.cpp \
    source/models/Connection.cpp \
    source/data/DataDecorator.cpp \
    source/data/StringDecorator.cpp \
    source/data/IPDecorator.cpp \
    source/data/EnumeratorDecorator.cpp \
    source/data/Entity.cpp \
    source/data/IntDecorator.cpp \
    source/data/deneme.cpp

HEADERS += source/fb-lib_global.h  \
    source/fb-lib_global.h  \
    source/logger/fb-logger.h \
    source/controllers/master-controller.h \
    source/controllers/navigation-controller.h \
    source/framework/command.h \
    source/controllers/command-controller.h \
    source/SFTP/sftpconnector.h \
    source/models/ConnectionController.h \
    source/models/ConnectionSMImpl.h \
    source/models/Connection.h \
    source/data/DataDecorator.h \
    source/data/StringDecorator.h \
    source/data/IPDecorator.h \
    source/data/EnumeratorDecorator.h \
    source/data/Entity.h \
    source/data/EntityCollection.h \
    source/data/IntDecorator.h \
    source/data/deneme.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
