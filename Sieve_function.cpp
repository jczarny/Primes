#include <vector>
#include <omp.h>
#include "Sieve_sequential.h"

std::vector<int> getRanges_global(int n_threads, int min, int max) {
	int size = max - min + 1;
	std::vector<int> ranges;
	for (int i = 0; i < n_threads; i++) {
		ranges.push_back(int(i * size / n_threads));
		ranges.push_back(int((i + 1) * size / n_threads) - 1);
	}
	return ranges;
}


void Sieve_parallel_functional(std::vector<int>& primes, int min, int max, int segSize) {
	std::vector<int> initialPrimes;
	int lim = sqrt(max);
	fillWithPrimes_sieve(initialPrimes, 2, lim, segSize);
	primes = initialPrimes;

	int size = max - min + 1;
	std::vector<bool> isPrime(size, true);

	std::vector<int> ranges = getRanges_global(omp_get_max_threads(), min, max);

#pragma omp parallel
	{
		std::vector<int> localPrimes;

		// split array for each thread
#pragma omp for schedule(static, 1)
		for (int i = 0; i < initialPrimes.size(); i++) {
			localPrimes.push_back(initialPrimes[i]);
		}

		int i = omp_get_thread_num();
		int part = i;

		int n_threads = omp_get_num_threads();
		do {
			int partLow = ranges[part * 2] + min;
			int partHigh = ranges[part * 2 + 1] + min;

			int segLow = partLow;
			int segHigh = partLow + segSize;

			while (segLow < segHigh) {
				if (segHigh > partHigh)
					segHigh = partHigh;

				for (int i = 0; i < localPrimes.size(); i++) {
					int start = int(segLow / localPrimes[i]) * localPrimes[i];
					if (start < segLow)
						start += localPrimes[i];
					for (int j = start; j <= segHigh; j += localPrimes[i]) {
						isPrime[j - min] = false;
					}
				}

				segLow += segSize;
				segHigh += segSize;
			}

#pragma omp barrier
			part = (part + 1) % n_threads;
		} while (part != i);
	}

	for (int i = 0; i < size; i++) {
		if (isPrime[i])
			primes.push_back(i + min);
	}
}