QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    canvas.cpp \
    circle.cpp \
    command.cpp \
    command/addshapecommand.cpp \
    command/moveshapecommand.cpp \
    command/removeshapecommand.cpp \
    command/rotateshapecommand.cpp \
    command/scaleshapecommand.cpp \
    ellipse.cpp \
    hexagon.cpp \
    layer.cpp \
    layermanager.cpp \
    main.cpp \
    mainwindow.cpp \
    parameterdialog.cpp \
    polygon.cpp \
    polyline.cpp \
    rectangle.cpp \
    rhomb.cpp \
    shape.cpp \
    square.cpp \
    stars.cpp \
    triangle.cpp\

HEADERS += \
    canvas.h \
    circle.h \
    command.h \
    command/addshapecommand.h \
    command/moveshapecommand.h \
    command/propertychangecmd.h \
    command/removeshapecommand.h \
    command/rotateshapecommand.h \
    command/scaleshapecommand.h \
    ellipse.h \
    hexagon.h \
    layer.h \
    layermanager.h \
    mainwindow.h \
    parameterdialog.h \
    polygon.h \
    polyline.h \
    rectangle.h \
    rhomb.h \
    shap.h \
    square.h \
    stars.h \
    triangle.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
