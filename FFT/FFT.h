#define _USE_MATH_DEFINES

#include <math.h>
#include <cmath>
#include <complex>

#define TWIDLE(k, angle) std::polar(1.0, k * angle)

void DFT(std::complex<double>* x, std::complex<double>* X, size_t N, size_t	 s);
void FFT_2(std::complex<double>* in, std::complex<double>* out, size_t N, size_t Ns, size_t group_id);
void FFT_4(std::complex<double>* in, std::complex<double>* out, size_t N, size_t Ns, size_t group_id);
void FFT_8(std::complex<double>* in, std::complex<double>* out, size_t N, size_t Ns, size_t group_id);
