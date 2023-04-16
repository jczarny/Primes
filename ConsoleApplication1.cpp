#include <stdio.h>
#include <iostream>
#include <vector>
#include <array>
#include <omp.h>
#include <time.h>
#include "Dividing.h"
#include "Sieve_sequential.h"
#include "Sieve_domain.h"

constexpr auto CACHE_CAP = 64000;
constexpr auto MIN = 2;
constexpr auto MAX = 100000000;

void Sieve_parallel_functional(std::vector<int>& primes, int min, int max) {
	std::vector<int> initialPrimes;
	int lim = sqrt(max);
	fillWithPrimes_sieve(initialPrimes, min, lim);
	int size = max - min + 1;
	std::vector<bool> isPrime(size, true);

#pragma omp parallel num_threads(1)
	{
		std::vector<int> localPrimes;

		// split array for each thread
		#pragma omp for schedule(static, 1)
		for (int i = 0; i < initialPrimes.size(); i++) {
			localPrimes.push_back(initialPrimes[i]);
		}

		int segLow = min;
		int segHigh = min + CACHE_CAP;
		while (segLow < segHigh) {
			std::vector<bool> isPrimeLocally(size, true);

			if (segHigh > max)
				segHigh = max;

			for (int i = 0; i < localPrimes.size(); i++) {

				int start = int(segLow / localPrimes[i]) * localPrimes[i];
				if (start < segLow)
					start += localPrimes[i];
				for (int j = start; j <= segHigh; j += localPrimes[i]) {
					isPrimeLocally[j - min] = false;
				}
			}

			#pragma omp critical
			{
				for (int i = segLow - min; i <= segHigh - min; i++) {
					isPrime[i] = (isPrime[i] && isPrimeLocally[i]);
				}
			}

			segLow += CACHE_CAP;
			segHigh += CACHE_CAP;
		}
	}
	primes = initialPrimes;
	for (int i = initialPrimes.back(); i < max; i++) {
		if (isPrime[i])
			primes.push_back(i+min);
	}
}


void printPrimesSize(std::vector<int>& primes, int min, int max) {
	int counter = 0;
	for (auto prime : primes) {
		if (prime >= MIN && prime <= MAX)
			counter++;
	}
	std::cout << "size: " << counter << std::endl;
}

void printPrimes(std::vector<int>& primes, int min, int max) {
	for (auto prime : primes) {
		if (prime >= MIN && prime <= MAX)
			printf("%d ", prime);
	}
	std::cout << std::endl;
}

int main(int argc, char* argv[])
{
	double tstart, tstop;
	std::vector<int> primes = { };

	
	primes = { };
	tstart = clock();
	fillWithPrimes_sieve(primes, MIN, MAX);
	tstop = clock();
	std::cout << "Sito sekwencyjne: " << (tstop - tstart) / CLOCKS_PER_SEC << " \n";
	printPrimesSize(primes, MIN, MAX);
	//printPrimes(primes, MIN, MAX);
	

	
	primes = { };
	tstart = clock();
	Sieve_parallel_domain(primes, MIN, MAX);
	tstop = clock();
	std::cout << "Sito domenowe: " << (tstop - tstart) / CLOCKS_PER_SEC << " \n";
	printPrimesSize(primes, MIN, MAX);
	//printPrimes(primes, MIN, MAX);
	

	primes = { };
	tstart = clock();
	Sieve_parallel_functional(primes, MIN, MAX);
	tstop = clock();
	std::cout << "Sito funkcyjne: " << (tstop - tstart) / CLOCKS_PER_SEC << " \n";
	printPrimesSize(primes, MIN, MAX);
	//printPrimes(primes, MIN, MAX);

	
	primes = { 2 };
	tstart = clock();
	fillWithPrimes_divide(primes, MIN, MAX);
	tstop = clock();
	std::cout << "Przez dzielenie: " << (tstop - tstart) / CLOCKS_PER_SEC << " \n";
	printPrimesSize(primes, MIN, MAX);
	//printPrimes(primes, MIN, MAX);
	
}