/********************************************************************************
** Form generated from reading UI file 'QtMainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTMAINWINDOW_H
#define UI_QTMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "QtConsole.h"
#include "QtDescribeView.h"

QT_BEGIN_NAMESPACE

class Ui_QtMainWindow
{
public:
    QAction *actionViewOnX;
    QAction *actionViewOnY;
    QAction *actionViewOnZ;
    QAction *actionViewOnXYZ;
    QAction *actionViewFit;
    QAction *actionViewSolid;
    QAction *actionViewNoEdge;
    QAction *actionViewWireFlame;
    QAction *actionViewVoid;
    QAction *actionSelectAll;
    QAction *actionSelect_Cancel;
    QAction *actionViewErase;
    QAction *actionViewBlend;
    QAction *actionOpenBody;
    QAction *actionUserFunc1;
    QAction *actionUserFunc2;
    QAction *actionUserFunc3;
    QAction *actionUserFunc4;
    QAction *actionUserFunc5;
    QAction *actionAboutKodatuno;
    QAction *actionViewScale;
    QAction *actionTolerance;
    QAction *actionCPView;
    QAction *actionSurfaceInfo;
    QAction *actionScalingBODY;
    QAction *actionTranslation;
    QAction *actionRotationSurface;
    QAction *actionSweepSurface;
    QAction *actionNURBSCurve;
    QAction *actionNURBSSarface;
    QAction *actionNURBSRank;
    QAction *actionQuit;
    QAction *actionShowConsole;
    QAction *actionDeleteBody;
    QAction *actionShowBodyList;
    QAction *actionUserStatus;
    QAction *actionUserFunc6;
    QAction *actionUserFunc7;
    QAction *actionUserFunc8;
    QWidget *centralWidget;
    QGridLayout *gridLayout_3;
    QFrame *frame;
    QGridLayout *gridLayout_4;
    QtDescribeView *describeWidget;
    QMenuBar *menuBar;
    QMenu *menuFile_F;
    QMenu *menuUser_U;
    QMenu *menuHelp_H;
    QMenu *menuView_V;
    QMenu *menuEdit_E;
    QMenu *menuSurface;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *bodyListDock;
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout;
    QListWidget *bodyList;
    QDockWidget *consoleDock;
    QWidget *dockWidgetContents_2;
    QGridLayout *gridLayout_2;
    QtConsole *consoleText;

    void setupUi(QMainWindow *QtMainWindow)
    {
        if (QtMainWindow->objectName().isEmpty())
            QtMainWindow->setObjectName("QtMainWindow");
        QtMainWindow->setWindowModality(Qt::NonModal);
        QtMainWindow->resize(830, 660);
        QtMainWindow->setMinimumSize(QSize(830, 660));
        QtMainWindow->setContextMenuPolicy(Qt::DefaultContextMenu);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/icons/main.ico"), QSize(), QIcon::Normal, QIcon::Off);
        QtMainWindow->setWindowIcon(icon);
        QtMainWindow->setToolButtonStyle(Qt::ToolButtonIconOnly);
        QtMainWindow->setDockOptions(QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks|QMainWindow::ForceTabbedDocks|QMainWindow::VerticalTabs);
        QtMainWindow->setUnifiedTitleAndToolBarOnMac(false);
        actionViewOnX = new QAction(QtMainWindow);
        actionViewOnX->setObjectName("actionViewOnX");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/new/prefix1/icons/ViewOnX.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionViewOnX->setIcon(icon1);
        actionViewOnY = new QAction(QtMainWindow);
        actionViewOnY->setObjectName("actionViewOnY");
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/new/prefix1/icons/ViewOnY.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionViewOnY->setIcon(icon2);
        actionViewOnZ = new QAction(QtMainWindow);
        actionViewOnZ->setObjectName("actionViewOnZ");
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/new/prefix1/icons/ViewOnZ.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionViewOnZ->setIcon(icon3);
        actionViewOnXYZ = new QAction(QtMainWindow);
        actionViewOnXYZ->setObjectName("actionViewOnXYZ");
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/new/prefix1/icons/ViewOnXYZ.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionViewOnXYZ->setIcon(icon4);
        actionViewFit = new QAction(QtMainWindow);
        actionViewFit->setObjectName("actionViewFit");
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/new/prefix1/icons/ViewFit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionViewFit->setIcon(icon5);
        actionViewSolid = new QAction(QtMainWindow);
        actionViewSolid->setObjectName("actionViewSolid");
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/new/prefix1/icons/ViewSolid.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionViewSolid->setIcon(icon6);
        actionViewNoEdge = new QAction(QtMainWindow);
        actionViewNoEdge->setObjectName("actionViewNoEdge");
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/new/prefix1/icons/ViewSolid_NoEdge.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionViewNoEdge->setIcon(icon7);
        actionViewWireFlame = new QAction(QtMainWindow);
        actionViewWireFlame->setObjectName("actionViewWireFlame");
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/new/prefix1/icons/ViewWierFrame.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionViewWireFlame->setIcon(icon8);
        actionViewVoid = new QAction(QtMainWindow);
        actionViewVoid->setObjectName("actionViewVoid");
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/new/prefix1/icons/ViewVoid.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionViewVoid->setIcon(icon9);
        actionSelectAll = new QAction(QtMainWindow);
        actionSelectAll->setObjectName("actionSelectAll");
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/new/prefix1/icons/SelectAll.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSelectAll->setIcon(icon10);
        actionSelect_Cancel = new QAction(QtMainWindow);
        actionSelect_Cancel->setObjectName("actionSelect_Cancel");
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/new/prefix1/icons/SelectCancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSelect_Cancel->setIcon(icon11);
        actionViewErase = new QAction(QtMainWindow);
        actionViewErase->setObjectName("actionViewErase");
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/new/prefix1/icons/ViewErase.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionViewErase->setIcon(icon12);
        actionViewBlend = new QAction(QtMainWindow);
        actionViewBlend->setObjectName("actionViewBlend");
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/new/prefix1/icons/ViewBlend.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionViewBlend->setIcon(icon13);
        actionOpenBody = new QAction(QtMainWindow);
        actionOpenBody->setObjectName("actionOpenBody");
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/new/prefix1/icons/pload.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpenBody->setIcon(icon14);
        actionUserFunc1 = new QAction(QtMainWindow);
        actionUserFunc1->setObjectName("actionUserFunc1");
        actionUserFunc2 = new QAction(QtMainWindow);
        actionUserFunc2->setObjectName("actionUserFunc2");
        actionUserFunc3 = new QAction(QtMainWindow);
        actionUserFunc3->setObjectName("actionUserFunc3");
        actionUserFunc4 = new QAction(QtMainWindow);
        actionUserFunc4->setObjectName("actionUserFunc4");
        actionUserFunc5 = new QAction(QtMainWindow);
        actionUserFunc5->setObjectName("actionUserFunc5");
        actionAboutKodatuno = new QAction(QtMainWindow);
        actionAboutKodatuno->setObjectName("actionAboutKodatuno");
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/new/prefix1/icons/help.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionAboutKodatuno->setIcon(icon15);
        actionViewScale = new QAction(QtMainWindow);
        actionViewScale->setObjectName("actionViewScale");
        actionTolerance = new QAction(QtMainWindow);
        actionTolerance->setObjectName("actionTolerance");
        actionCPView = new QAction(QtMainWindow);
        actionCPView->setObjectName("actionCPView");
        actionSurfaceInfo = new QAction(QtMainWindow);
        actionSurfaceInfo->setObjectName("actionSurfaceInfo");
        actionScalingBODY = new QAction(QtMainWindow);
        actionScalingBODY->setObjectName("actionScalingBODY");
        actionTranslation = new QAction(QtMainWindow);
        actionTranslation->setObjectName("actionTranslation");
        actionRotationSurface = new QAction(QtMainWindow);
        actionRotationSurface->setObjectName("actionRotationSurface");
        actionSweepSurface = new QAction(QtMainWindow);
        actionSweepSurface->setObjectName("actionSweepSurface");
        actionNURBSCurve = new QAction(QtMainWindow);
        actionNURBSCurve->setObjectName("actionNURBSCurve");
        actionNURBSSarface = new QAction(QtMainWindow);
        actionNURBSSarface->setObjectName("actionNURBSSarface");
        actionNURBSRank = new QAction(QtMainWindow);
        actionNURBSRank->setObjectName("actionNURBSRank");
        actionQuit = new QAction(QtMainWindow);
        actionQuit->setObjectName("actionQuit");
        actionShowConsole = new QAction(QtMainWindow);
        actionShowConsole->setObjectName("actionShowConsole");
        actionDeleteBody = new QAction(QtMainWindow);
        actionDeleteBody->setObjectName("actionDeleteBody");
        actionShowBodyList = new QAction(QtMainWindow);
        actionShowBodyList->setObjectName("actionShowBodyList");
        actionUserStatus = new QAction(QtMainWindow);
        actionUserStatus->setObjectName("actionUserStatus");
        actionUserFunc6 = new QAction(QtMainWindow);
        actionUserFunc6->setObjectName("actionUserFunc6");
        actionUserFunc7 = new QAction(QtMainWindow);
        actionUserFunc7->setObjectName("actionUserFunc7");
        actionUserFunc7->setEnabled(true);
        actionUserFunc7->setVisible(true);
        actionUserFunc8 = new QAction(QtMainWindow);
        actionUserFunc8->setObjectName("actionUserFunc8");
        centralWidget = new QWidget(QtMainWindow);
        centralWidget->setObjectName("centralWidget");
        gridLayout_3 = new QGridLayout(centralWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName("gridLayout_3");
        gridLayout_3->setContentsMargins(2, 2, 2, 2);
        frame = new QFrame(centralWidget);
        frame->setObjectName("frame");
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Sunken);
        gridLayout_4 = new QGridLayout(frame);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName("gridLayout_4");
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        describeWidget = new QtDescribeView(frame);
        describeWidget->setObjectName("describeWidget");

        gridLayout_4->addWidget(describeWidget, 0, 0, 1, 1);


        gridLayout_3->addWidget(frame, 0, 0, 1, 1);

        QtMainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtMainWindow);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 830, 24));
        menuBar->setDefaultUp(false);
        menuBar->setNativeMenuBar(true);
        menuFile_F = new QMenu(menuBar);
        menuFile_F->setObjectName("menuFile_F");
        menuUser_U = new QMenu(menuBar);
        menuUser_U->setObjectName("menuUser_U");
        menuHelp_H = new QMenu(menuBar);
        menuHelp_H->setObjectName("menuHelp_H");
        menuView_V = new QMenu(menuBar);
        menuView_V->setObjectName("menuView_V");
        menuEdit_E = new QMenu(menuBar);
        menuEdit_E->setObjectName("menuEdit_E");
        menuSurface = new QMenu(menuBar);
        menuSurface->setObjectName("menuSurface");
        QtMainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtMainWindow);
        mainToolBar->setObjectName("mainToolBar");
        QtMainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QtMainWindow);
        statusBar->setObjectName("statusBar");
        statusBar->setEnabled(true);
        QtMainWindow->setStatusBar(statusBar);
        bodyListDock = new QDockWidget(QtMainWindow);
        bodyListDock->setObjectName("bodyListDock");
        bodyListDock->setAllowedAreas(Qt::AllDockWidgetAreas);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName("dockWidgetContents");
        gridLayout = new QGridLayout(dockWidgetContents);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        bodyList = new QListWidget(dockWidgetContents);
        bodyList->setObjectName("bodyList");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(bodyList->sizePolicy().hasHeightForWidth());
        bodyList->setSizePolicy(sizePolicy);
        bodyList->setMinimumSize(QSize(0, 0));
        bodyList->setSizeIncrement(QSize(40, 0));
        bodyList->setBaseSize(QSize(40, 0));

        gridLayout->addWidget(bodyList, 2, 0, 1, 1);

        bodyListDock->setWidget(dockWidgetContents);
        QtMainWindow->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, bodyListDock);
        consoleDock = new QDockWidget(QtMainWindow);
        consoleDock->setObjectName("consoleDock");
        consoleDock->setFloating(false);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName("dockWidgetContents_2");
        gridLayout_2 = new QGridLayout(dockWidgetContents_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        consoleText = new QtConsole(dockWidgetContents_2);
        consoleText->setObjectName("consoleText");
        consoleText->setFrameShape(QFrame::Panel);
        consoleText->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(consoleText, 0, 0, 1, 1);

        consoleDock->setWidget(dockWidgetContents_2);
        QtMainWindow->addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, consoleDock);

        menuBar->addAction(menuFile_F->menuAction());
        menuBar->addAction(menuEdit_E->menuAction());
        menuBar->addAction(menuSurface->menuAction());
        menuBar->addAction(menuView_V->menuAction());
        menuBar->addAction(menuUser_U->menuAction());
        menuBar->addAction(menuHelp_H->menuAction());
        menuFile_F->addAction(actionOpenBody);
        menuFile_F->addAction(actionQuit);
        menuUser_U->addAction(actionUserStatus);
        menuUser_U->addSeparator();
        menuUser_U->addAction(actionUserFunc1);
        menuUser_U->addAction(actionUserFunc2);
        menuUser_U->addAction(actionUserFunc3);
        menuUser_U->addAction(actionUserFunc4);
        menuUser_U->addAction(actionUserFunc5);
        menuUser_U->addAction(actionUserFunc6);
        menuUser_U->addAction(actionUserFunc7);
        menuUser_U->addAction(actionUserFunc8);
        menuHelp_H->addAction(actionAboutKodatuno);
        menuView_V->addAction(actionViewScale);
        menuView_V->addAction(actionTolerance);
        menuView_V->addAction(actionCPView);
        menuView_V->addAction(actionSurfaceInfo);
        menuView_V->addSeparator();
        menuView_V->addAction(actionShowConsole);
        menuView_V->addAction(actionShowBodyList);
        menuEdit_E->addAction(actionScalingBODY);
        menuEdit_E->addAction(actionTranslation);
        menuEdit_E->addAction(actionNURBSRank);
        menuEdit_E->addAction(actionDeleteBody);
        menuSurface->addAction(actionRotationSurface);
        menuSurface->addAction(actionSweepSurface);
        menuSurface->addAction(actionNURBSCurve);
        menuSurface->addAction(actionNURBSSarface);
        mainToolBar->addAction(actionViewOnX);
        mainToolBar->addAction(actionViewOnY);
        mainToolBar->addAction(actionViewOnZ);
        mainToolBar->addAction(actionViewOnXYZ);
        mainToolBar->addAction(actionViewFit);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionViewSolid);
        mainToolBar->addAction(actionViewNoEdge);
        mainToolBar->addAction(actionViewWireFlame);
        mainToolBar->addAction(actionViewVoid);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionSelectAll);
        mainToolBar->addAction(actionSelect_Cancel);
        mainToolBar->addAction(actionViewErase);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionViewBlend);

        retranslateUi(QtMainWindow);

        QMetaObject::connectSlotsByName(QtMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *QtMainWindow)
    {
        QtMainWindow->setWindowTitle(QCoreApplication::translate("QtMainWindow", "Kodatuno Qt", nullptr));
        actionViewOnX->setText(QCoreApplication::translate("QtMainWindow", "View On X", nullptr));
        actionViewOnY->setText(QCoreApplication::translate("QtMainWindow", "View On Y", nullptr));
#if QT_CONFIG(tooltip)
        actionViewOnY->setToolTip(QCoreApplication::translate("QtMainWindow", "View On Y", nullptr));
#endif // QT_CONFIG(tooltip)
        actionViewOnZ->setText(QCoreApplication::translate("QtMainWindow", "View On Z", nullptr));
#if QT_CONFIG(tooltip)
        actionViewOnZ->setToolTip(QCoreApplication::translate("QtMainWindow", "View On Z", nullptr));
#endif // QT_CONFIG(tooltip)
        actionViewOnXYZ->setText(QCoreApplication::translate("QtMainWindow", "View On XYZ", nullptr));
#if QT_CONFIG(tooltip)
        actionViewOnXYZ->setToolTip(QCoreApplication::translate("QtMainWindow", "View On XYZ", nullptr));
#endif // QT_CONFIG(tooltip)
        actionViewFit->setText(QCoreApplication::translate("QtMainWindow", "View Fit", nullptr));
#if QT_CONFIG(tooltip)
        actionViewFit->setToolTip(QCoreApplication::translate("QtMainWindow", "View Fit", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionViewFit->setShortcut(QCoreApplication::translate("QtMainWindow", "Ctrl+F", nullptr));
#endif // QT_CONFIG(shortcut)
        actionViewSolid->setText(QCoreApplication::translate("QtMainWindow", "View Solid", nullptr));
#if QT_CONFIG(tooltip)
        actionViewSolid->setToolTip(QCoreApplication::translate("QtMainWindow", "View Solid", nullptr));
#endif // QT_CONFIG(tooltip)
        actionViewNoEdge->setText(QCoreApplication::translate("QtMainWindow", "View No Edge", nullptr));
#if QT_CONFIG(tooltip)
        actionViewNoEdge->setToolTip(QCoreApplication::translate("QtMainWindow", "View No Edge", nullptr));
#endif // QT_CONFIG(tooltip)
        actionViewWireFlame->setText(QCoreApplication::translate("QtMainWindow", "View Wire Flame", nullptr));
#if QT_CONFIG(tooltip)
        actionViewWireFlame->setToolTip(QCoreApplication::translate("QtMainWindow", "View Wire Flame", nullptr));
#endif // QT_CONFIG(tooltip)
        actionViewVoid->setText(QCoreApplication::translate("QtMainWindow", "View Void", nullptr));
#if QT_CONFIG(tooltip)
        actionViewVoid->setToolTip(QCoreApplication::translate("QtMainWindow", "View Void", nullptr));
#endif // QT_CONFIG(tooltip)
        actionSelectAll->setText(QCoreApplication::translate("QtMainWindow", "Select All", nullptr));
#if QT_CONFIG(tooltip)
        actionSelectAll->setToolTip(QCoreApplication::translate("QtMainWindow", "Select All", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionSelectAll->setShortcut(QCoreApplication::translate("QtMainWindow", "Ctrl+A", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSelect_Cancel->setText(QCoreApplication::translate("QtMainWindow", "Select Cancel", nullptr));
#if QT_CONFIG(tooltip)
        actionSelect_Cancel->setToolTip(QCoreApplication::translate("QtMainWindow", "Select Cancel", nullptr));
#endif // QT_CONFIG(tooltip)
        actionViewErase->setText(QCoreApplication::translate("QtMainWindow", "View Erase", nullptr));
#if QT_CONFIG(tooltip)
        actionViewErase->setToolTip(QCoreApplication::translate("QtMainWindow", "View Erase", nullptr));
#endif // QT_CONFIG(tooltip)
        actionViewBlend->setText(QCoreApplication::translate("QtMainWindow", "View Blend", nullptr));
        actionOpenBody->setText(QCoreApplication::translate("QtMainWindow", "Open File (&O)", nullptr));
        actionUserFunc1->setText(QCoreApplication::translate("QtMainWindow", "User Func 1", nullptr));
        actionUserFunc2->setText(QCoreApplication::translate("QtMainWindow", "User Func 2", nullptr));
        actionUserFunc3->setText(QCoreApplication::translate("QtMainWindow", "User Func 3", nullptr));
        actionUserFunc4->setText(QCoreApplication::translate("QtMainWindow", "User Func 4", nullptr));
        actionUserFunc5->setText(QCoreApplication::translate("QtMainWindow", "User Func 5", nullptr));
        actionAboutKodatuno->setText(QCoreApplication::translate("QtMainWindow", "About Kodatuno(&K)", nullptr));
        actionViewScale->setText(QCoreApplication::translate("QtMainWindow", "View Scale (&S)", nullptr));
        actionTolerance->setText(QCoreApplication::translate("QtMainWindow", "Tolerance (&T)", nullptr));
        actionCPView->setText(QCoreApplication::translate("QtMainWindow", "Control Point View (&C)", nullptr));
        actionSurfaceInfo->setText(QCoreApplication::translate("QtMainWindow", "Surface Info (&S)", nullptr));
        actionScalingBODY->setText(QCoreApplication::translate("QtMainWindow", "Scaling (&S)", nullptr));
        actionTranslation->setText(QCoreApplication::translate("QtMainWindow", "Translation (&T)", nullptr));
        actionRotationSurface->setText(QCoreApplication::translate("QtMainWindow", "Rotation (&R)", nullptr));
        actionSweepSurface->setText(QCoreApplication::translate("QtMainWindow", "Sweep (&S)", nullptr));
        actionNURBSCurve->setText(QCoreApplication::translate("QtMainWindow", "NURBS Curve", nullptr));
        actionNURBSSarface->setText(QCoreApplication::translate("QtMainWindow", "NURBS Sarface", nullptr));
        actionNURBSRank->setText(QCoreApplication::translate("QtMainWindow", "NURBS Rank(&R)", nullptr));
        actionQuit->setText(QCoreApplication::translate("QtMainWindow", "Quit (&Q)", nullptr));
        actionShowConsole->setText(QCoreApplication::translate("QtMainWindow", "Show Console", nullptr));
        actionDeleteBody->setText(QCoreApplication::translate("QtMainWindow", "Delete Body (&D)", nullptr));
        actionShowBodyList->setText(QCoreApplication::translate("QtMainWindow", "Show BODY List", nullptr));
#if QT_CONFIG(tooltip)
        actionShowBodyList->setToolTip(QCoreApplication::translate("QtMainWindow", "Show BODY List", nullptr));
#endif // QT_CONFIG(tooltip)
        actionUserStatus->setText(QCoreApplication::translate("QtMainWindow", "User Status", nullptr));
#if QT_CONFIG(tooltip)
        actionUserStatus->setToolTip(QCoreApplication::translate("QtMainWindow", "User Status", nullptr));
#endif // QT_CONFIG(tooltip)
        actionUserFunc6->setText(QCoreApplication::translate("QtMainWindow", "User Func 6", nullptr));
        actionUserFunc7->setText(QCoreApplication::translate("QtMainWindow", "User Func 7", nullptr));
        actionUserFunc8->setText(QCoreApplication::translate("QtMainWindow", "User Func 8", nullptr));
        menuFile_F->setTitle(QCoreApplication::translate("QtMainWindow", "File (&F)", nullptr));
        menuUser_U->setTitle(QCoreApplication::translate("QtMainWindow", "User (&U)", nullptr));
        menuHelp_H->setTitle(QCoreApplication::translate("QtMainWindow", "Help (&H)", nullptr));
        menuView_V->setTitle(QCoreApplication::translate("QtMainWindow", "View (&V)", nullptr));
        menuEdit_E->setTitle(QCoreApplication::translate("QtMainWindow", "Edit (&E)", nullptr));
        menuSurface->setTitle(QCoreApplication::translate("QtMainWindow", "Create (&C)", nullptr));
        bodyListDock->setWindowTitle(QCoreApplication::translate("QtMainWindow", "BODY List", nullptr));
        consoleDock->setWindowTitle(QCoreApplication::translate("QtMainWindow", "Console", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtMainWindow: public Ui_QtMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTMAINWINDOW_H
