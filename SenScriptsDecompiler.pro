QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

#QXlsx
QXLSX_PARENTPATH=./qxlsx/        # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=./qxlsx/headers/  # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=./qxlsx/sources/  # current QXlsx source path is ./source/
include(./QXlsx.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        sources/Builder.cpp \
        sources/functions.cpp \
        sources/instruction.cpp \
        sources/main.cpp \
        sources/decompiler.cpp \
        sources/translationfile.cpp \
        sources/utilities.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    headers/Builder.h \
    headers/CS1InstructionsSet.h \
    headers/CS2InstructionsSet.h \
    headers/CS3InstructionsSet.h \
    headers/CS4InstructionsSet.h \
    headers/TXInstructionsSet.h \
    headers/functions.h \
    headers/global_vars.h \
    headers/instruction.h \
    headers/operande.h \
    headers/decompiler.h \
    headers/translationfile.h \
    headers/utilities.h

win32-msvc* {
    QMAKE_CXXFLAGS += /bigobj
}

