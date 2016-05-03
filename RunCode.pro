QT += core network
QT -= gui

lessThan(QT_MINOR_VERSION, 6):greaterThan(QT_MAJOR_VERSION, 4): CONFIG += --std=c++11

CONFIG += c++11

TARGET = RunCode
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    runprocess.cpp \
    memorymonitor.cpp \
    utils/logsystem.cpp \
    maincontroller.cpp \
    inputhandle.cpp \
    helper/About.cpp \
    network.cpp

HEADERS += \
    runprocess.h \
    memorymonitor.h \
    utils/logsystem.h \
    maincontroller.h \
    inputhandle.h \
    helper/About.h \
    network.h

OTHER_FILES += \
    test/alloc.c \
    test/a+b_problem.c

MOC_DIR = $$PWD/moc
RCC_DIR = $$PWD/rc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lMemoryInfo
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lMemoryInfo

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
