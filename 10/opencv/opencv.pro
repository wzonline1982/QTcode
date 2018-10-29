TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

INCLUDEPATH+=C:\opencv\opencv-2.4.13\include\opencv \
C:\opencv\opencv-2.4.13\include\opencv2 \
C:\opencv\opencv-2.4.13\include

//LIBS +=-L C:\opencv\opencv-2.4.13\x86\mingw\lib\libopencv_*.a
LIBS += -L C:\opencv\opencv-2.4.13\x86\mingw\bin\libopencv_*.dll
