QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
DEFINES += YMW_APP
ymw += board util resource render # canvasgl.h include texturedraw
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32 {
    HEADERS += $$system(for /r $$PWD %i in (*.h,*.hpp) do @echo %i)
    SOURCES += $$system(for /r $$PWD %i in (*.c,*.cpp) do @echo %i)
    FORMS += $$system(for /r $$PWD %i in (*.ui) do @echo %i)
}

unix {
    HEADERS += $$system(find $$PWD -name \'*.h\') \
               $$system(find $$PWD -name \'*.hpp\')

    SOURCES += $$system(find $$PWD -name \'*.c\') \
               $$system(find $$PWD -name \'*.cpp\') \
               $$system(find $$PWD -name \'*.mm\')

    FORMS += $$system(find $$PWD -name \'*.ui\')
}

RESOURCES += \
    resource.qrc

include(../../framework.pri)
include(../module.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
