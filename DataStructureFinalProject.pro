QT       += core gui
QT       += sql
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

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
    ./code/main.cpp

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
    ./code/window/rankwindow.cpp
HEADERS += \
    ./code/window/aboutwindow.h \
    ./code/window/configwindow.h \
    ./code/window/gamewindow.h \
    ./code/window/mainwindow.h \
    ./code/window/rankwindow.h
FORMS += \
    ./code/window/aboutwindow.ui \
    ./code/window/configwindow.ui \
    ./code/window/gamewindow.ui \
    ./code/window/mainwindow.ui \
    ./code/window/rankwindow.ui

# 4. database
SOURCES += \
    ./code/database/sqliteDb.cpp \
    ./code/database/ranking.cpp
HEADERS += \
    ./code/database/sqliteDb.h \
    ./code/database/ranking.h
    
# 5. BGM and sounds
SOURCES += \
    ./code/audio/BGM.cpp
HEADERS += \
    ./code/audio/BGM.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

