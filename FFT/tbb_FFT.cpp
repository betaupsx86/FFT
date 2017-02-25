#include <tbb/tbb.h>
#include <FFT.h>
#include <vector>

void parallel_DFT(std::complex<float>* x, std::complex<float>* X, size_t N) {
	std::complex<float>* tmp = new std::complex<float>[N]; //tmp array to hold the intermediate results between stages

	size_t num_threads;
	size_t stages = log2(N);
	size_t FFT_8_stages = stages / 3;
	size_t remainder_stages = stages % 3;

	size_t Ns = 1;

	for (size_t i = 0; i < FFT_8_stages; i++) {
		size_t num_groups = N / 8;
		tbb::parallel_for((size_t) 0, num_groups, [=] (size_t group_id) {
			FFT_8(x, X, N, Ns, group_id);
		});
		x = X;
		X = tmp;
		tmp = x;

		Ns = Ns << 3;
	}

	if (remainder_stages == 1) {
		size_t num_groups = N / 2;
		tbb::parallel_for((size_t)0, num_groups, [=](size_t group_id) {
			FFT_2(x, X, N, Ns, group_id);
		});
		x = X;
		X = tmp;
		tmp = x;
	}
	else if (remainder_stages == 2) {
		size_t num_groups = N / 4;
		tbb::parallel_for((size_t)0, num_groups, [=](size_t group_id) {
			FFT_4(x, X, N, Ns, group_id);
		});
		x = X;
		X = tmp;
		tmp = x;
	}

	if ((FFT_8_stages + (remainder_stages>0)) % 2) {
		tmp = X;
		delete[] tmp;
		tmp = NULL;
		x = NULL;
		X = NULL;
	}
	else {
		std::memcpy(X, x, N * sizeof *X);
		delete[] tmp;
		tmp = NULL;
		x = NULL;
		X = NULL;
	}
}
