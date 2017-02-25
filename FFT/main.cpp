#include "FFTPlot.h"
#include <QtWidgets/QApplication>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>

#include <complex>
#include <iostream>
#include <chrono>

QwtPlot* plotSignal(std::complex<float>* signal, size_t samples, char* title);
extern void thread_DFT(std::complex<float>*x, std::complex<float>* X, size_t N);
extern void parallel_DFT(std::complex<float>* x, std::complex<float>* X, size_t N);
extern void DFT(std::complex<float>* x, std::complex<float>* X, size_t N, size_t s);

QwtPlot* plotSignal(std::complex<float>* signal, size_t samples, char* title) {

	QwtPlot* plot = new QwtPlot();
	plot->setTitle(title);
	plot->setCanvasBackground(Qt::white);
	plot->setAxisScale(QwtPlot::yLeft, -10.0, 10.0);
	plot->insertLegend(new QwtLegend());

	QwtPlotGrid *grid = new QwtPlotGrid();
	grid->attach(plot);

	QwtPlotCurve *curve = new QwtPlotCurve();
	curve->setTitle("Some Points");
	curve->setPen(Qt::blue, 0),
		curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
	curve->setStyle(QwtPlotCurve::CurveStyle::Sticks);

	QwtSymbol *symbol = new QwtSymbol(QwtSymbol::Ellipse,
		QBrush(Qt::yellow), QPen(Qt::red, 2), QSize(8, 8));
	curve->setSymbol(symbol);

	curve->setStyle(QwtPlotCurve::CurveStyle::Sticks);
	//curve->setStyle(QwtPlotCurve::CurveStyle::Lines);

	QPolygonF points;

	for (size_t i = 0; i < samples; i++)
	{
		points << QPointF(i, std::abs(signal[i]));
	}

	curve->setSamples(points);
	curve->attach(plot);

	plot->resize(1024, 768);
	plot->show();
	return plot;
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	size_t size = 1 << 27;
	std::complex<float>* input = new std::complex<float>[size]; //tmp array to hold the intermediate results between stages
	std::complex<float>* output_0 = new std::complex<float>[size]; 
	std::complex<float>* output_1 = new std::complex<float>[size]; 
	//std::complex<float>* output_2 = new std::complex<float>[size];

	for (size_t i = 0; i < size; i++)
	{
		input[i] = std::polar(1.0F, float(-2 * M_PI * 3* i /64 ));
		//input[i] = cos(i*M_PI / 8) + sin(i*M_PI / 8);
		//input[i] = cos(0);
	}

//	QwtPlot* in_plot = plotSignal(input, size, "input");

	std::chrono::high_resolution_clock::time_point t1;
	std::chrono::high_resolution_clock::time_point t2;

	t1 = std::chrono::high_resolution_clock::now();
	parallel_DFT(input, output_0, size);
	t2 = std::chrono::high_resolution_clock::now();
	double duration0 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

	t1 = std::chrono::high_resolution_clock::now();
	thread_DFT(input, output_1, size);
	t2 = std::chrono::high_resolution_clock::now();
	double duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

	//t1 = std::chrono::high_resolution_clock::now();
	//DFT(input, output_2, size,1);
	//t2 = std::chrono::high_resolution_clock::now();
	//double duration2 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

	//QwtPlot* out_plot_0 = plotSignal(output_0, size, "output_0");
	//QwtPlot* out_plot_1 = plotSignal(output_1, size, "output_1");
	//QwtPlot* out_plot_2 = plotSignal(output_2, size, "output_2");
	
	return a.exec();
}

