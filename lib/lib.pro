TEMPLATE = lib
TARGET = skedimageprovider
CONFIG += staticlib
CONFIG += c++11
#CONFIG += debug
QT += quick

# Build directory
OBJECTS_DIR = ../build
MOC_DIR     = ../build
RCC_DIR     = ../build
UI_DIR      = ../build
DESTDIR     = ../build

SOURCES += skedimageprovider.cpp
HEADERS += skedimageprovider.h

LIBS += -laui

headers.files = skedimageprovider.h
headers.path  = /usr/include
target.path = /usr/lib
INSTALLS += target headers
