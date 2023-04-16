#include <vector>
#include "Sieve_sequential.h"
#include <omp.h>
#include <iostream>

std::vector<int> getRanges(int num_thread, int n_threads, int min, int max) {
	int size = max - min;
	std::vector<int> ranges;
	ranges.push_back(int(num_thread * size / n_threads));
	ranges.push_back(int((num_thread + 1) * size / n_threads) - 1);
	return ranges;
}


void Sieve_parallel_domain(std::vector<int>& primes, int min, int max) {
	int segCap = int(sqrt(max - min)) * 64;
	std::vector<int> initialPrimes;
	int lim = sqrt(max);
	fillWithPrimes_sieve(initialPrimes, min, lim);
	primes = initialPrimes;

#pragma omp parallel
	{
		std::vector<int> ranges = getRanges(omp_get_thread_num(), omp_get_num_threads(), min, max);
		int low = ranges[0];
		int high = ranges[1];
		int size = high - low + 1;

		int segLow = low + min;
		int segHigh = low + segCap + min;

		while (segLow < segHigh) {
			std::vector<bool> isPrimeLocally(segCap, true);

			if (segHigh > high + min + 1)
				segHigh = high + min + 1;

			for (int i = 0; i < initialPrimes.size(); i++) {
				int start = int(segLow / initialPrimes[i]) * initialPrimes[i];
				if (start < segLow)
					start += initialPrimes[i];

				for (int j = start; j <= segHigh; j += initialPrimes[i]) {
					isPrimeLocally[j - segLow] = false;
				}
			}
#pragma omp critical
			{
				for (int i = 0; i < segHigh - segLow; i++) {
					if (isPrimeLocally[i]) {
						primes.push_back(i + segLow);
					}
				}
			}

			segLow += segCap;
			segHigh += segCap;
		}
	}
}