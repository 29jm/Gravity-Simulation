TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp \
    Body.cpp

CONFIG(release, debug|release): LIBS += -lsfml-graphics -lsfml-window -lsfml-system
CONFIG(debug, debug|release):   LIBS += -lsfml-graphics-d -lsfml-window-d -lsfml-system-d

HEADERS += \
    Body.hpp
