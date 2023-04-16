#include <vector>
#include <iostream>
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


void Sieve_parallel_functional(std::vector<int>& primes, int min, int max) {
	std::vector<int> initialPrimes;
	int lim = sqrt(max);
	fillWithPrimes_sieve(initialPrimes, min, lim);
	primes = initialPrimes;

	int size = max - min + 1;
	std::vector<bool> isPrime(size, true);

	std::vector<int> ranges = getRanges_global(omp_get_max_threads(), min, max);
	//std::cout << ranges.size() << " " << ranges[0] << " " << ranges[1] << std::endl;
	//std::cout << ranges.size() << " " << ranges[2] << " " << ranges[3] << std::endl;

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
			//printf("partLow: %d, partHigh: %d\n", partLow, partHigh);
			for (int i = 0; i < localPrimes.size(); i++) {
				int start = int(partLow / localPrimes[i]) * localPrimes[i];
				if (start < partLow)
					start += localPrimes[i];
				//printf("start: %d, partHigh: %d, localPrime[i]: %d\n", start, partHigh, localPrimes[i]);
				for (int j = start; j <= partHigh; j += localPrimes[i]) {
					isPrime[j - min] = false;
				}
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