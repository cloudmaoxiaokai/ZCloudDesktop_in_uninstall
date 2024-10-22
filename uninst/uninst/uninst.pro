# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = uninst
DESTDIR = ../Win32/Release
QT += core network widgets gui
CONFIG += release console
DEFINES += WIN64 QT_NETWORK_LIB QT_WIDGETS_LIB
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Release
LIBS += -L"$(QTDIR)/plugins/platforms" \
    -limm32 \
    -lwinmm \
    -lWs2_32 \
    -lopengl32 \
    -lqwindows \
    -lkernel32 \
    -luser32 \
    -lgdi32 \
    -lcomdlg32 \
    -ladvapi32 \
    -lshell32 \
    -lole32 \
    -loleaut32 \
    -luuid \
    -lodbc32 \
    -lodbccp32 \
    -lqminimal
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/release
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(uninst.pri)
win32:RC_FILE = uninst.rc
Release:QMAKE_POST_LINK += upx -9 $(DESTDIR_TARGET)
