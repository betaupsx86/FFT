/********************************************************************************
** Form generated from reading UI file 'SignalPlot.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNALPLOT_H
#define UI_SIGNALPLOT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SignalPlotClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SignalPlotClass)
    {
        if (SignalPlotClass->objectName().isEmpty())
            SignalPlotClass->setObjectName(QStringLiteral("SignalPlotClass"));
        SignalPlotClass->resize(600, 400);
        menuBar = new QMenuBar(SignalPlotClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        SignalPlotClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SignalPlotClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SignalPlotClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(SignalPlotClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        SignalPlotClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(SignalPlotClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SignalPlotClass->setStatusBar(statusBar);

        retranslateUi(SignalPlotClass);

        QMetaObject::connectSlotsByName(SignalPlotClass);
    } // setupUi

    void retranslateUi(QMainWindow *SignalPlotClass)
    {
        SignalPlotClass->setWindowTitle(QApplication::translate("SignalPlotClass", "SignalPlot", 0));
    } // retranslateUi

};

namespace Ui {
    class SignalPlotClass: public Ui_SignalPlotClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNALPLOT_H
