TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Composable.cpp \
        Demos.cpp \
        PersonaFluent.cpp \
        main.cpp

HEADERS += \
        Composable.h \
        Demos.h \
        Fluent.h \
        PersonaBase.h \
        PersonaFluent.h \
        StaticFluent.h
