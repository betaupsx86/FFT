/********************************************************************************
** Form generated from reading UI file 'FFTPlot.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FFTPLOT_H
#define UI_FFTPLOT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FFTPlotClass
{
public:

    void setupUi(QWidget *FFTPlotClass)
    {
        if (FFTPlotClass->objectName().isEmpty())
            FFTPlotClass->setObjectName(QStringLiteral("FFTPlotClass"));
        FFTPlotClass->resize(600, 400);

        retranslateUi(FFTPlotClass);

        QMetaObject::connectSlotsByName(FFTPlotClass);
    } // setupUi

    void retranslateUi(QWidget *FFTPlotClass)
    {
        FFTPlotClass->setWindowTitle(QApplication::translate("FFTPlotClass", "FFTPlot", 0));
    } // retranslateUi

};

namespace Ui {
    class FFTPlotClass: public Ui_FFTPlotClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FFTPLOT_H
