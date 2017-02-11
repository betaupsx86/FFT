#pragma once

#include <QtWidgets/QWidget>
#include "ui_FFTPlot.h"

class FFTPlot : public QWidget
{
	Q_OBJECT

public:
	FFTPlot(QWidget *parent = Q_NULLPTR);

private:
	Ui::FFTPlotClass ui;
};
