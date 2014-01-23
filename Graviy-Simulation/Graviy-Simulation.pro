TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp \
    Body.cpp

win32 {
    INCLUDEPATH += "C:\Libraries\include"
    CONFIG(release, debug|release): LIBS += -L"C:\Libraries\lib" -lsfml-graphics -lsfml-window -lsfml-system
    CONFIG(debug, debug|release):   LIBS += -lsfml-graphics-d -lsfml-window-d -lsfml-system-d
}
unix {
    CONFIG(release, debug|release): LIBS += -lsfml-graphics -lsfml-window -lsfml-system
    CONFIG(debug, debug|release):   LIBS += -lsfml-graphics-d -lsfml-window-d -lsfml-system-d
}

HEADERS += \
    Body.hpp
