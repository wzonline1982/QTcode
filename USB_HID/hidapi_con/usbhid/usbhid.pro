TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.c

HEADERS += \
    hidapi.h

LIBS += -L$$_PRO_FILE_PWD_/LIB/ \
            -lhidapi

#win32 {

#LIBS += -lhid -lsetupapi

#}



