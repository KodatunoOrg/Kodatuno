#-------------------------------------------------
#
# Project created by QtCreator 2013-03-13T21:23:55
#
#-------------------------------------------------

QT       += core gui \
		opengl

TARGET = KOD_TEST
TEMPLATE = app

INCLUDEPATH += \
    ../../GE/ \
    ../../Sys/ \
    ../../UA/ \

SOURCES += \
    QtMain.cpp \
    StdAfxQt.cpp \
    QtUserStatDialog.cpp \
    QtSweepSurfDialog.cpp \
    QtRotSurfDialog.cpp \
    QtNurbsSurfDialog.cpp \
    QtNurbsCurveDialog.cpp \
    QtMainWindow.cpp \
    QtDescribeView.cpp \
    QtConsole.cpp \
    ../../GE/VRML_Parser.cpp \
    ../../GE/STL_Parser.cpp \
    ../../GE/StdAfx.cpp \
    ../../GE/SFQuant.cpp \
    ../../GE/Quaternion.cpp \
    ../../GE/NURBS_Func.cpp \
    ../../GE/MESH.cpp \
    ../../GE/KodListFunc.cpp \
    ../../GE/IGES_Parser.cpp \
    ../../GE/DXF_Parser.cpp \
    ../../GE/Describe_BODY.cpp \
    ../../GE/BODY.cpp \
    ../../Sys/Kodatuno_Main.cpp \
    ../../Sys/Kodatuno_CMD.cpp \
    ../../Sys/Command.cpp \
    ../../UA/UserFunc.cpp \
    ../../UA/SampleFunc2.cpp \
    ../../UA/SampleFunc1.cpp

HEADERS  += \
    StdAfxQt.h \
    QtUserStatDialog.h \
    QtSweepSurfDialog.h \
    QtRotSurfDialog.h \
    QtNurbsSurfDialog.h \
    QtNurbsCurveDialog.h \
    QtMainWindow.h \
    QtDescribeView.h \
    QtConsole.h \
    ../../GE/VRML_Parser.h \
    ../../GE/STL_Parser.h \
    ../../GE/StdAfx.h \
    ../../GE/SFQuant.h \
    ../../GE/Quaternion.h \
    ../../GE/NURBS_Func.h \
    ../../GE/MESH.h \
    ../../GE/KodListFunc.h \
    ../../GE/IGES_Parser.h \
    ../../GE/DXF_Parser.h \
    ../../GE/Describe_BODY.h \
    ../../GE/BODY.h \
    ../../Sys/Kutil.h \
    ../../Sys/Kodatuno.h \
    ../../Sys/Command.h \
    ../../UA/UserFunc.h \
    ../../UA/SampleFunc.h

FORMS    += \
    QtUserStatDialog.ui \
    QtSweepSurfDialog.ui \
    QtRotSurfDialog.ui \
    QtNurbsSurfDialog.ui \
    QtNurbsCurveDialog.ui \
    QtMainWindow.ui

LIBS        += -lfreeglut

RESOURCES += \
    icons.qrc

QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp


