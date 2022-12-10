QT       += core gui
QT       += sql
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# ----- SOURCES & HEADERS -----
# 1. main
SOURCES += \
    ./code/main.cpp \

# 2. core
SOURCES += \
    ./code/core/board.cpp \
    ./code/core/stone.cpp
HEADERS += \
    ./code/core/board.h \
    ./code/core/stone.h

# 3. window
SOURCES += \
    ./code/window/aboutwindow.cpp \
    ./code/window/configwindow.cpp \
    ./code/window/gamewindow.cpp \
    ./code/window/mainwindow.cpp \
    ./code/window/rankwindow.cpp \
    ./code/window/framelesswindow.cpp
HEADERS += \
    ./code/window/aboutwindow.h \
    ./code/window/configwindow.h \
    ./code/window/gamewindow.h \
    ./code/window/mainwindow.h \
    ./code/window/rankwindow.h \
    ./code/window/framelesswindow.h
FORMS += \
    ./code/window/aboutwindow.ui \
    ./code/window/configwindow.ui \
    ./code/window/gamewindow.ui \
    ./code/window/mainwindow.ui \
    ./code/window/rankwindow.ui \
    ./code/window/framelesswindow.ui

# 4. database
SOURCES += \
    ./code/database/sqliteDb.cpp \
    ./code/database/rank.cpp
HEADERS += \
    ./code/database/sqliteDb.h \
    ./code/database/rank.h
    
# 5. Audio
SOURCES += \
    ./code/audio/BGM.cpp
HEADERS += \
    ./code/audio/BGM.h

# 6. Hypercube
SOURCES += \
    code/hypercube/gemmodelmanager.cpp \
    code/hypercube/hstone.cpp \
    code/hypercube/hypercube.cpp \
    code/hypercube/mesh.cpp \
    code/hypercube/model.cpp \
    code/hypercube/stonemanager.cpp \
    code/hypercube/animation/animation.cpp \
    code/hypercube/animation/animationfactory.cpp \
    code/hypercube/animation/animationfall.cpp \
    code/hypercube/animation/animationremove.cpp \
    code/hypercube/animation/animationswap.cpp \
    code/hypercube/animation/animationwait.cpp \
    code/hypercube/hypercubethread.cpp
HEADERS += \
    code/hypercube/camera.h \
    code/hypercube/gemmodelmanager.h \
    code/hypercube/hstone.h \
    code/hypercube/hypercube.h \
    code/hypercube/mesh.h \
    code/hypercube/model.h \
    code/hypercube/stonemanager.h \
    code/hypercube/DEBUG.h \
    code/hypercube/animation/animation.h \
    code/hypercube/animation/animationfactory.h \
    code/hypercube/animation/animationfall.h \
    code/hypercube/animation/animationremove.h \
    code/hypercube/animation/animationswap.h \
    code/hypercube/animation/animationwait.h \
    code/hypercube/hypercubethread.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource/res.qrc \
    resource/shaders.qrc

# assimp
win32: LIBS += -L$$PWD/library/assimp/lib/ -llibassimp.dll

INCLUDEPATH += $$PWD/library/assimp/include
DEPENDPATH += $$PWD/library/assimp/include
