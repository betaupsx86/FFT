#include <FFT.h>
#include <thread>
#include <vector>

void FFT_thread(std::complex<double>* in, std::complex<double>* out, size_t N, size_t Ns, size_t thread_id, size_t work, void(*FFT_func)(std::complex<double>*, std::complex<double>*, size_t, size_t, size_t)) {
	for (size_t i = 0; i < work; i++)
	{
		FFT_func(in, out, N, Ns, work*thread_id + i);
	}
}
//Threaded multiple-stage implementation of the DFT. We spawn several threads and handle a fixed amount of work in each one.
void thread_DFT(std::complex<double>*x, std::complex<double>* X, size_t N) {
	std::complex<double>* tmp = new std::complex<double>[N]; //tmp array to hold the intermediate results between stages

	size_t stages = log2(N);								//Total number of stages
	size_t FFT_8_stages = stages / 3;						//Number of stages to compute using our largest FFT func (FFT_8)
	size_t remainder_stages = stages % 3;					//Remainding stages
	size_t Ns = 1;											//Defined as pow (2 , number of completed stages). Used to calculate twiddle factors and index remapping
	size_t num_groups;										//Number of groups per stages (how many times the FFT funcs are called per stage)
	size_t a = 1;											//Coefficient to adjust the number of threads
	size_t num_threads;										//Number of threads to spawn (Oversubscription will decrease performance). Needs to be a power of 2 to evenly distribute the groups
	size_t work_per_thread;									//Number of groups handled by each thread

	//Looping through the stages that use largest FFT func
	for (size_t i = 0; i < FFT_8_stages; i++) {
		num_groups = N / 8;
		num_threads = (a*std::thread::hardware_concurrency() < num_groups) ? a*std::thread::hardware_concurrency() : num_groups;
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
		num_threads = (a*std::thread::hardware_concurrency() < num_groups) ? a*std::thread::hardware_concurrency() : num_groups;
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
		num_threads = (a*std::thread::hardware_concurrency() < num_groups) ? a*std::thread::hardware_concurrency() : num_groups;
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
//Silly implementation where we spawn as many threads as groups in each stage.
void silly_threaded_DFT(std::complex<double>* x, std::complex<double>* X, size_t N) {
	std::complex<double>* tmp = new std::complex<double>[N]; //tmp array to hold the intermediate results between stages

	size_t num_threads;
	size_t stages = log2(N);
	size_t FFT_8_stages = stages / 3;
	size_t remainder_stages = stages % 3;

	size_t Ns = 1;

	for (size_t i = 0; i < FFT_8_stages; i++) {
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
	else if (remainder_stages == 2) {
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