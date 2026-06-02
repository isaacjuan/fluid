TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        BusinessRules.cpp \
        Composable.cpp \
        Demos.cpp \
        PersonaFluent.cpp \
        main.cpp

HEADERS += \
        BusinessRules.h \
        Composable.h \
        Demos.h \
        Fluent.h \
        PersonaBase.h \
        PersonaFluent.h \
        StaticFluent.h
