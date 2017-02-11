#include "SignalPlot.h"
#include <QtWidgets/QApplication>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>

#include <cmath>
#include <complex>
#include <iostream>
#include <thread> 
#include <chrono>

#define TWIDLE(k, angle) std::polar(1.0, k * angle)
#include "SignalPlot.h"
#include <QtWidgets/QApplication>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>

#include <cmath>
#include <complex>
#include <iostream>
#include <thread> 
#include <chrono>

#define TWIDLE(k, angle) std::polar(1.0, k * angle)

QwtPlot* plotSignal(std::complex<double>* signal, size_t samples, char* title);

void DFT(std::complex<double>* x, std::complex<double>* X, int N, int s);
void FFT_2_thread(std::complex<double>* in, std::complex<double>* out, size_t N, size_t Ns, size_t thread_id, size_t num_threads);
void FFT_2(std::complex<double>* in, std::complex<double>* out, size_t N, size_t Ns, size_t thread_id);
void FFT_4_thread(std::complex<double>* in, std::complex<double>* out, size_t N, size_t Ns, size_t thread_id, size_t num_threads);
void FFT_4(std::complex<double>* in, std::complex<double>* out, size_t N, size_t Ns, size_t thread_id);
void FFT_8_thread(std::complex<double>* in, std::complex<double>* out, size_t N, size_t Ns, size_t thread_id, size_t num_threads);
void FFT_8(std::complex<double>* in, std::complex<double>* out, size_t N, size_t Ns, size_t thread_id);
void threaded_DFT(std::complex<double>*x, std::complex<double>* X, size_t N);
void silly_threaded_DFT(std::complex<double>*x, std::complex<double>* X, size_t N);

QwtPlot* plotSignal(std::complex<double>* signal, size_t samples, char* title) {

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

void DFT(std::complex<double>* x, std::complex<double>* X, int N, int s) {

	if (N == 1)
		X[0] = x[0];

	else {
		DFT(x, X, N / 2, 2 * s);
		DFT(x + s, X + N / 2, N / 2, 2 * s);

		for (size_t k = 0; k < N / 2; k++)
		{
			std::complex<double> tmp = X[k];
			X[k] = tmp + std::polar(1.0,double(-2 * M_PI *k / N)) * X[k + N / 2];
			X[k+N/2] = tmp - std::polar(1.0,double(-2 * M_PI *k / N)) * X[k + N / 2];	
		}
	}
}

void FFT_2_thread(std::complex<double>* in, std::complex<double>* out, size_t N, size_t Ns, size_t thread_id, size_t num_threads) {
	size_t num_groups = N / 2;
	size_t work_per_thread = num_groups / num_threads;

	for (size_t i = 0; i < work_per_thread; i++)
	{
		FFT_2(in, out, N, Ns, work_per_thread*thread_id + i);
	}
}
void FFT_2(std::complex<double>* in, std::complex<double>* out,size_t N, size_t Ns, size_t thread_id ){
	
	size_t num_threads = N / 2;
	std::complex<double>  in0, in1;
	in0 = in[(0 * num_threads) + thread_id];
	in1 = in[(1 * num_threads) + thread_id];

	if (Ns != 1)
	{
		double angle = -2 * M_PI*(thread_id % Ns) / (Ns * 2);
		in1 *= TWIDLE(1, angle);
	}

	std::complex<double>  tmp;
	tmp = in0;
	in0 = in0 + in1;
	in1 = tmp - in1;

	size_t Idout = (thread_id / Ns)*Ns*2 + (thread_id%Ns);
	out[(0 * Ns) + Idout] = in0;
	out[(1 * Ns) + Idout] = in1;
}

void FFT_4_thread(std::complex<double>* in, std::complex<double>* out, size_t N, size_t Ns, size_t thread_id, size_t num_threads) {
	size_t num_groups = N / 4;
	size_t work_per_thread = num_groups / num_threads;

	for (size_t i = 0; i < work_per_thread; i++)
	{
		FFT_4(in, out, N, Ns, work_per_thread*thread_id + i);
	}
}
void FFT_4(std::complex<double>* in, std::complex<double>* out, size_t N, size_t Ns, size_t thread_id)
{
	size_t num_threads = N / 4;
	std::complex<double>  in0, in1, in2, in3;
	in0 = in[(0 * num_threads) + thread_id];
	in1 = in[(1 * num_threads) + thread_id];
	in2 = in[(2 * num_threads) + thread_id];
	in3 = in[(3 * num_threads) + thread_id];

	if (Ns != 1)
	{
		double angle = -2 * M_PI*(thread_id % Ns) / (Ns * 4);
		in1 *= TWIDLE(1, angle);
		in2 *= TWIDLE(2, angle);
		in3 *= TWIDLE(3, angle);
	}

	std::complex<double>  v0, v1, v2, v3;
	v0 = in0 + in2;
	v2 = in0 - in2;
	v1 = in1 + in3;
	v3 = std::complex<double>(in1.imag()-in3.imag(), in3.real()-in1.real());
	in0 = v0 + v1;
	in2 = v0 - v1;
	in1 = v2 + v3;
	in3 = v2 - v3;

	int Idout = (thread_id / Ns)*Ns*4+ (thread_id%Ns);
	out[(0 * Ns) + Idout] = in0;
	out[(1 * Ns) + Idout] = in1;
	out[(2 * Ns) + Idout] = in2;
	out[(3 * Ns) + Idout] = in3;
}

void FFT_8_thread(std::complex<double>* in, std::complex<double>* out, size_t N, size_t Ns, size_t thread_id, size_t num_threads) {
	size_t num_groups = N / 8;
	size_t work_per_thread = num_groups / num_threads;

	for (size_t i = 0; i < work_per_thread; i++)
	{
		FFT_8(in, out, N, Ns, work_per_thread*thread_id + i);
	}
}
void FFT_8(std::complex<double>* in, std::complex<double>* out, size_t N, size_t Ns, size_t thread_id)
{
	size_t num_threads = N / 8;
	std::complex<double>  in0, in1, in2, in3, in4, in5, in6, in7;
	in0 = in[(0 * num_threads) + thread_id];
	in1 = in[(1 * num_threads) + thread_id];
	in2 = in[(2 * num_threads) + thread_id];
	in3 = in[(3 * num_threads) + thread_id];
	in4 = in[(4 * num_threads) + thread_id];
	in5 = in[(5 * num_threads) + thread_id];
	in6 = in[(6 * num_threads) + thread_id];
	in7 = in[(7 * num_threads) + thread_id];

	if (Ns != 1)
	{
		double angle = -2 * M_PI*(thread_id % Ns)/(Ns * 8);
		in1 *= TWIDLE(1, angle);
		in2 *= TWIDLE(2, angle);
		in3 *= TWIDLE(3, angle);
		in4 *= TWIDLE(4, angle);
		in5 *= TWIDLE(5, angle);
		in6 *= TWIDLE(6, angle);
		in7 *= TWIDLE(7, angle);
	}

	std::complex<double>  v0, v1, v2, v3, v4, v5, v6, v7;
	v0 = in0 + in4;
	v4 = in0 - in4;
	v1 = in1 + in5;
	v5 = in1 - in5;
	v2 = in2 + in6;
	v6 = std::complex<double>(in2.imag() - in6.imag(), in6.real() - in2.real());
	v3 = in3 + in7;
	v7 = std::complex<double>(in3.imag() - in7.imag(), in7.real() - in3.real());

	std::complex<double> tmp;
	tmp = v0;
	v0 = v0 + v2;
	v2 = tmp - v2;
	tmp = v1;
	v1 = v1 + v3;
	v3 = std::complex<double>(tmp.imag() - v3.imag(), v3.real() - tmp.real());

	tmp = v4;
	v4 = v4 + v6;
	v6 = tmp - v6;
	tmp = v5;
	v5 = std::complex<double>(M_SQRT1_2* (v5.real()+v7.real()+v5.imag()+v7.imag()), M_SQRT1_2*(v5.imag()+v7.imag()-v5.real()- v7.real()));
	v7 = std::complex<double>(M_SQRT1_2* (v7.real() -tmp.real() -v7.imag() + tmp.imag()), M_SQRT1_2*(v7.imag()- tmp.imag()+v7.real()- tmp.real()));

	in0 = v0 + v1;
	in4 = v0 - v1;

	in1 = v4 + v5;
	in5 = v4 - v5;

	in2 = v2 + v3;
	in6 = v2 - v3;

	in3 = v6 + v7;
	in7 = v6 - v7;

	size_t Idout = (thread_id / Ns)*Ns*8 + (thread_id%Ns);
	out[(0 * Ns) + Idout] = in0;
	out[(1 * Ns) + Idout] = in1;
	out[(2 * Ns) + Idout] = in2;
	out[(3 * Ns) + Idout] = in3;
	out[(4 * Ns) + Idout] = in4;
	out[(5 * Ns) + Idout] = in5;
	out[(6 * Ns) + Idout] = in6;
	out[(7 * Ns) + Idout] = in7;
}

void FFT_thread(std::complex<double>* in, std::complex<double>* out, size_t N, size_t Ns, size_t thread_id, size_t work, void(*FFT_func)(std::complex<double>*, std::complex<double>*, size_t, size_t, size_t)) {
	for (size_t i = 0; i < work; i++)
	{
		FFT_func(in, out, N, Ns, work*thread_id + i);
	}
}

//Threaded multiple-stage implementation of the DFT. We spawn several threads and handle a fixed amount of work in each one.
void threaded_DFT(std::complex<double>*x, std::complex<double>* X, size_t N) {

	std::complex<double>* tmp = (std::complex<double>*)malloc(N * sizeof *tmp); //tmp array to hold the intermediate results between stages

	size_t stages = log2(N);								//Total number of stages
	size_t FFT_8_stages = stages / 3;						//Number of stages to compute using our largest FFT func (FFT_8)
	size_t remainder_stages = stages % 3;					//Remainding stages
	size_t Ns=1;											//Defined as pow (2 , number of completed stages). Used to calculate twiddle factors and index remapping
	size_t num_groups;										//Number of groups per stages (how many times the FFT funcs are called per stage)
	size_t num_threads;										//Number of threads to spawn (Spawning too many will decrease performance)
	size_t work_per_thread;									//Number of groups handled by each thread

	//Looping through the stages that use largest FFT func
	for (size_t i = 0; i < FFT_8_stages; i++){
		num_groups = N / 8;
		num_threads = (std::thread::hardware_concurrency() < num_groups) ? std::thread::hardware_concurrency() : num_groups;
		work_per_thread = num_groups / num_threads;
		std::vector<std::thread> t = std::vector<std::thread>(num_threads);

		for (size_t thread_id = 0; thread_id < num_threads; thread_id++) {
			t[thread_id] = std::thread(FFT_thread, x, X, N, Ns, thread_id, work_per_thread, &FFT_8);
		}

		for (size_t thread_id = 0; thread_id < num_threads; thread_id++) {
			t[thread_id].join();
		}
		
		//Remapping input and output array for next stage
		x = X;
		X = tmp;
		tmp = x;

		Ns = Ns << 3;										//Each FFT_8 stage completes 3 stages
	}
	
	//Dealing with the remainding stages. Same procedure as the FFT_8 stages.
	if (remainder_stages == 1) {
		num_groups = N / 2;
		num_threads = (std::thread::hardware_concurrency() < num_groups) ? std::thread::hardware_concurrency() : num_groups;
		work_per_thread = num_groups / num_threads;
		std::vector<std::thread> t = std::vector<std::thread>(num_threads);

		for (size_t thread_id = 0; thread_id < num_threads; thread_id++) {
			t[thread_id] = std::thread(FFT_thread, x, X, N, Ns, thread_id, work_per_thread, &FFT_2);
		}

		for (size_t thread_id = 0; thread_id < num_threads; thread_id++) {
			t[thread_id].join();
		}
		x = X;
		X = tmp;
		tmp = x;
	}

	else if (remainder_stages == 2) {
		num_groups = N / 4;
		num_threads = (std::thread::hardware_concurrency() < num_groups) ? std::thread::hardware_concurrency() : num_groups;
		work_per_thread = num_groups / num_threads;
		std::vector<std::thread> t = std::vector<std::thread>(num_threads);

		for (size_t thread_id = 0; thread_id < num_threads; thread_id++) {
			t[thread_id] = std::thread(FFT_thread, x, X, N, Ns, thread_id, work_per_thread, &FFT_4);
		}

		for (size_t thread_id = 0; thread_id < num_threads; thread_id++) {
			t[thread_id].join();
		}
		x = X;
		X = tmp;
		tmp = x;
	}

	//Making sure the result is in the correct memory location expected by the caller. Freeing tmp array.
	if ((FFT_8_stages + (remainder_stages>0)) % 2)	{
		tmp = X;
		free(tmp);
		tmp = NULL;
		x = NULL;
		X = NULL;
	}
	else {
		std::memcpy(X, x, N * sizeof *X);
		free(tmp);
		tmp = NULL;
		x = NULL;
		X = NULL;
	}
}

//Silly implementation where we spawn as many threads as groups in each stage.
void silly_threaded_DFT(std::complex<double>* x, std::complex<double>* X, size_t N) {
	std::complex<double>* tmp=(std::complex<double>*)malloc(N * sizeof *tmp);

	size_t num_threads;
	size_t stages = log2(N);
	size_t FFT_8_stages = stages / 3;
	size_t remainder_stages = stages % 3;

	size_t Ns = 1;
	
	for (size_t i=0; i < FFT_8_stages; i++){
		num_threads = N / 8;
		std::vector<std::thread> t = std::vector<std::thread>(num_threads);

		for (size_t thread_id = 0; thread_id < num_threads; thread_id++) {
			t[thread_id] = std::thread(FFT_8, x, X, N, Ns, thread_id);
		}
		for (size_t thread_id = 0; thread_id < num_threads; thread_id++) {
			t[thread_id].join();
		}
		x = X;
		X = tmp;
		tmp = x;

		Ns = Ns << 3;
	}

	if (remainder_stages == 1) {
		num_threads = N / 2;
		std::vector<std::thread> t = std::vector<std::thread>(num_threads);

		for (size_t thread_id = 0; thread_id < num_threads; thread_id++) {
			t[thread_id] = std::thread(FFT_2, x, X, N, Ns, thread_id);
		}

		for (size_t thread_id = 0; thread_id < num_threads; thread_id++) {
			t[thread_id].join();
		}
		x = X;
		X = tmp;
		tmp = x;
	}
	else if (remainder_stages==2){
		num_threads = N / 4;
		std::vector<std::thread> t = std::vector<std::thread>(num_threads);

		for (size_t thread_id = 0; thread_id < num_threads; thread_id++) {
			t[thread_id] = std::thread(FFT_4, x, X, N, Ns, thread_id);
		}
		for (size_t thread_id = 0; thread_id < num_threads; thread_id++) {
			t[thread_id].join();
		}
		x = X;
		X = tmp;
		tmp = x;
	}

	if ((FFT_8_stages + (remainder_stages>0)) % 2)	{
		tmp = X;
		free(tmp);
		tmp = NULL;
		x = NULL;
		X = NULL;
	}
	else {
		std::memcpy(X, x, N * sizeof *X);
		free(tmp);
		tmp = NULL;
		x = NULL;
		X = NULL;
	}
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	size_t size = 1 << 15;
	std::complex<double>* input = (std::complex<double>*)malloc(size * sizeof *input);
	std::complex<double>* output = (std::complex<double>*)malloc(size * sizeof *output);
	std::complex<double>* output_2= (std::complex<double>*)malloc(size * sizeof *output_2);

	for (size_t i = 0; i < size; i++)
	{
		input[i] = cos(i*M_PI/8);
		printf("ARRAY IN %u is %d", i, input[i]);
	}

	QwtPlot* in_plot = plotSignal(input, size, "input");

	std::chrono::high_resolution_clock::time_point t1; 
	std::chrono::high_resolution_clock::time_point t2;

	t1 = std::chrono::high_resolution_clock::now();
	DFT(input, output, size, 1);
	t2 = std::chrono::high_resolution_clock::now();
	double duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

	t1 = std::chrono::high_resolution_clock::now();
	threaded_DFT(input, output_2, size);
	t2 = std::chrono::high_resolution_clock::now();
	double duration2 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

	QwtPlot* out_plot = plotSignal(output, size, "output");
	QwtPlot* out_plot_2 = plotSignal(output_2, size, "output_2");

	return a.exec();
}
