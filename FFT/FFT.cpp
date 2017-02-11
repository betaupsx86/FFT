#include <FFT.h>

void DFT(std::complex<double>* x, std::complex<double>* X, size_t N, size_t s) {

	if (N == 1)
		X[0] = x[0];

	else {
		DFT(x, X, N / 2, 2 * s);
		DFT(x + s, X + N / 2, N / 2, 2 * s);

		for (size_t k = 0; k < N / 2; k++)
		{
			std::complex<double> tmp = X[k];
			X[k] = tmp + std::polar(1.0, double(-2 * M_PI *k / N)) * X[k + N / 2];
			X[k + N / 2] = tmp - std::polar(1.0, double(-2 * M_PI *k / N)) * X[k + N / 2];
		}
	}
}
void FFT_2(std::complex<double>* in, std::complex<double>* out, size_t N, size_t Ns, size_t group_id) {

	size_t num_groups = N / 2;
	std::complex<double>  in0, in1;
	in0 = in[(0 * num_groups) + group_id];
	in1 = in[(1 * num_groups) + group_id];

	if (Ns != 1)
	{
		double angle = -2 * M_PI*(group_id % Ns) / (Ns * 2);
		in1 *= TWIDLE(1, angle);
	}

	std::complex<double>  tmp;
	tmp = in0;
	in0 = in0 + in1;
	in1 = tmp - in1;

	size_t Idout = (group_id / Ns)*Ns * 2 + (group_id%Ns);
	out[(0 * Ns) + Idout] = in0;
	out[(1 * Ns) + Idout] = in1;
}
void FFT_4(std::complex<double>* in, std::complex<double>* out, size_t N, size_t Ns, size_t group_id)
{
	size_t num_groups = N / 4;
	std::complex<double>  in0, in1, in2, in3;
	in0 = in[(0 * num_groups) + group_id];
	in1 = in[(1 * num_groups) + group_id];
	in2 = in[(2 * num_groups) + group_id];
	in3 = in[(3 * num_groups) + group_id];

	if (Ns != 1)
	{
		double angle = -2 * M_PI*(group_id % Ns) / (Ns * 4);
		in1 *= TWIDLE(1, angle);
		in2 *= TWIDLE(2, angle);
		in3 *= TWIDLE(3, angle);
	}

	std::complex<double>  v0, v1, v2, v3;
	v0 = in0 + in2;
	v2 = in0 - in2;
	v1 = in1 + in3;
	v3 = std::complex<double>(in1.imag() - in3.imag(), in3.real() - in1.real());
	in0 = v0 + v1;
	in2 = v0 - v1;
	in1 = v2 + v3;
	in3 = v2 - v3;

	int Idout = (group_id / Ns)*Ns * 4 + (group_id%Ns);
	out[(0 * Ns) + Idout] = in0;
	out[(1 * Ns) + Idout] = in1;
	out[(2 * Ns) + Idout] = in2;
	out[(3 * Ns) + Idout] = in3;
}
void FFT_8(std::complex<double>* in, std::complex<double>* out, size_t N, size_t Ns, size_t group_id)
{
	size_t num_groups = N / 8;
	std::complex<double>  in0, in1, in2, in3, in4, in5, in6, in7;
	in0 = in[(0 * num_groups) + group_id];
	in1 = in[(1 * num_groups) + group_id];
	in2 = in[(2 * num_groups) + group_id];
	in3 = in[(3 * num_groups) + group_id];
	in4 = in[(4 * num_groups) + group_id];
	in5 = in[(5 * num_groups) + group_id];
	in6 = in[(6 * num_groups) + group_id];
	in7 = in[(7 * num_groups) + group_id];

	if (Ns != 1)
	{
		double angle = -2 * M_PI*(group_id % Ns) / (Ns * 8);
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
	v5 = std::complex<double>(M_SQRT1_2* (v5.real() + v7.real() + v5.imag() + v7.imag()), M_SQRT1_2*(v5.imag() + v7.imag() - v5.real() - v7.real()));
	v7 = std::complex<double>(M_SQRT1_2* (v7.real() - tmp.real() - v7.imag() + tmp.imag()), M_SQRT1_2*(v7.imag() - tmp.imag() + v7.real() - tmp.real()));

	in0 = v0 + v1;
	in4 = v0 - v1;

	in1 = v4 + v5;
	in5 = v4 - v5;

	in2 = v2 + v3;
	in6 = v2 - v3;

	in3 = v6 + v7;
	in7 = v6 - v7;

	size_t Idout = (group_id / Ns)*Ns * 8 + (group_id%Ns);
	out[(0 * Ns) + Idout] = in0;
	out[(1 * Ns) + Idout] = in1;
	out[(2 * Ns) + Idout] = in2;
	out[(3 * Ns) + Idout] = in3;
	out[(4 * Ns) + Idout] = in4;
	out[(5 * Ns) + Idout] = in5;
	out[(6 * Ns) + Idout] = in6;
	out[(7 * Ns) + Idout] = in7;
}
