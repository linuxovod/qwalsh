# -------------------------------------------------
# Project created by QtCreator 2010-05-01T23:19:31
# -------------------------------------------------
TARGET = QWalsh
TEMPLATE = app
include(lib/qwt.prf)
SOURCES += src/main.cpp \
    src/ui/plot.cpp \
    src/logic/calculation.cpp \
    src/logic/signal.cpp \
    src/logic/signalparams.cpp \
    src/ui/plotwindow.cpp \
    src/ui/signalparamsdialog.cpp \
    src/ui/startwindow.cpp \
    src/logic/experiment.cpp \
    src/logic/experimentparams.cpp \
    src/logic/window.cpp \
    src/ui/plotwidget.cpp
HEADERS += \
    src/ui/plot.h \
    src/logic/calculation.h \
    src/logic/signal.h \
    src/logic/signalparams.h \
    src/ui/plotwindow.h \
    src/ui/signalparamsdialog.h \
    src/ui/startwindow.h \
    src/logic/experiment.h \
    src/logic/experimentparams.h \
    src/logic/window.h \
    src/ui/plotwidget.h
FORMS += \
    src/ui/plotwindow.ui \
    src/ui/signalparamsdialog.ui \
    src/ui/startwindow.ui
unix {
    INCLUDEPATH += /usr/include
    LIBS += -lfftw3
}
win32 {
    INCLUDEPATH += D:\\QtSDK\\mingw\\include
    INCLUDEPATH += D:\\QtSDK\\Desktop\\Qt\\4.8.0\\mingw\\include
    INCLUDEPATH += D:\\Work\\fftw-3.3-dll32
    LIBS +=  -L"D:\\Work\\fftw-3.3-dll32" -lfftw3-3
}






























