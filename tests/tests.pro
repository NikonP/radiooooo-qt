include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

TARGET = run-tests

HEADERS += \
        tst_case1.h

SOURCES += \
        main.cpp
