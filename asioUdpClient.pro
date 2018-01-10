QT += core
QT -= gui

CONFIG += c++11

TARGET = asioUdpClient
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    casioasyncclient.cpp

#INCLUDEPATH += /usr/include
#LIBS += /usr/lib/x86_64-linux-gnu/libboost_system.so /usr/lib/x86_64-linux-gnu/libboost_thread.so

HEADERS += \
    casioasyncclient.h

LIBS +=  -lboost_system-mt.dll \
         -lboost_thread-mt.dll \
         -lwsock32 \
         -lws2_32
