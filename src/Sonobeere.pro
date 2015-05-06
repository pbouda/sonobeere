TEMPLATE = app

QT += qml quick multimedia

SOURCES += main.cpp \
    beeresonotopy.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    beeresonotopy.h

unix|win32: LIBS += -lsonotopy

unix|win32: LIBS += -lfftw3
