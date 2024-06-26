QT -= gui
QT += widgets

ymw += util

TEMPLATE = lib
CONFIG += c++17

DEFINES += BOARD_LIBRARY YMW_LIBRARY
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
}

include(../../../framework.pri)
include(../../module.pri)

# Default rules for deployment.
unix {
    target.path = /usr/lib
}

//LIBS += -l$$PWD/../glew32 -l$$PWD/../glew32s
!isEmpty(target.path): INSTALLS += target
