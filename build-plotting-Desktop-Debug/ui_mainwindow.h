/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <qcgaugewidget.h>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QCustomPlot *SD1;
    QPushButton *ConBTN;
    QPushButton *StopBTN;
    QCustomPlot *SD2;
    QCustomPlot *F_SD;
    QCustomPlot *SD3;
    QLCDNumber *MODE;
    QProgressBar *BATTTERY;
    QcGaugeWidget *ROLL;
    QcGaugeWidget *PITCH;
    QcGaugeWidget *YAW;
    QcGaugeWidget *LIFT;
    QcGaugeWidget *AE0;
    QcGaugeWidget *AE1;
    QcGaugeWidget *AE2;
    QcGaugeWidget *AE3;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1665, 1284);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        SD1 = new QCustomPlot(centralWidget);
        SD1->setObjectName(QStringLiteral("SD1"));
        SD1->setGeometry(QRect(30, 10, 521, 251));
        ConBTN = new QPushButton(centralWidget);
        ConBTN->setObjectName(QStringLiteral("ConBTN"));
        ConBTN->setGeometry(QRect(1220, 160, 80, 23));
        StopBTN = new QPushButton(centralWidget);
        StopBTN->setObjectName(QStringLiteral("StopBTN"));
        StopBTN->setGeometry(QRect(1440, 160, 80, 23));
        SD2 = new QCustomPlot(centralWidget);
        SD2->setObjectName(QStringLiteral("SD2"));
        SD2->setGeometry(QRect(30, 530, 521, 251));
        F_SD = new QCustomPlot(centralWidget);
        F_SD->setObjectName(QStringLiteral("F_SD"));
        F_SD->setGeometry(QRect(30, 270, 521, 251));
        SD3 = new QCustomPlot(centralWidget);
        SD3->setObjectName(QStringLiteral("SD3"));
        SD3->setGeometry(QRect(30, 790, 521, 251));
        MODE = new QLCDNumber(centralWidget);
        MODE->setObjectName(QStringLiteral("MODE"));
        MODE->setGeometry(QRect(1220, 50, 301, 101));
        BATTTERY = new QProgressBar(centralWidget);
        BATTTERY->setObjectName(QStringLiteral("BATTTERY"));
        BATTTERY->setGeometry(QRect(1220, 10, 301, 21));
        BATTTERY->setValue(24);
        ROLL = new QcGaugeWidget(centralWidget);
        ROLL->setObjectName(QStringLiteral("ROLL"));
        ROLL->setGeometry(QRect(600, 10, 211, 181));
        PITCH = new QcGaugeWidget(centralWidget);
        PITCH->setObjectName(QStringLiteral("PITCH"));
        PITCH->setGeometry(QRect(900, 10, 211, 181));
        YAW = new QcGaugeWidget(centralWidget);
        YAW->setObjectName(QStringLiteral("YAW"));
        YAW->setGeometry(QRect(600, 260, 211, 181));
        LIFT = new QcGaugeWidget(centralWidget);
        LIFT->setObjectName(QStringLiteral("LIFT"));
        LIFT->setGeometry(QRect(900, 260, 211, 181));
        AE0 = new QcGaugeWidget(centralWidget);
        AE0->setObjectName(QStringLiteral("AE0"));
        AE0->setGeometry(QRect(600, 520, 211, 181));
        AE1 = new QcGaugeWidget(centralWidget);
        AE1->setObjectName(QStringLiteral("AE1"));
        AE1->setGeometry(QRect(900, 520, 211, 181));
        AE2 = new QcGaugeWidget(centralWidget);
        AE2->setObjectName(QStringLiteral("AE2"));
        AE2->setGeometry(QRect(600, 770, 211, 181));
        AE3 = new QcGaugeWidget(centralWidget);
        AE3->setObjectName(QStringLiteral("AE3"));
        AE3->setGeometry(QRect(900, 770, 211, 181));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1665, 20));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        ConBTN->setText(QApplication::translate("MainWindow", "Continue", Q_NULLPTR));
        StopBTN->setText(QApplication::translate("MainWindow", "Stop", Q_NULLPTR));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
