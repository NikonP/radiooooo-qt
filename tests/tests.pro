include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread

# gcov
QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
QMAKE_CFLAGS += -fprofile-arcs -ftest-coverage
LIBS += -lgcov

TARGET = run-tests

INCLUDEPATH += ../app

HEADERS += \
        configurator_tests.h \
        tst_case1.h \
        ../app/configurator.h

SOURCES += \
        main.cpp \
        ../app/configurator.cpp
