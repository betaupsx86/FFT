#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SignalPlot.h"

class SignalPlot : public QMainWindow
{
	Q_OBJECT

public:
	SignalPlot(QWidget *parent = Q_NULLPTR);

private:
	Ui::SignalPlotClass ui;
};
