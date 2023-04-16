#include <stdio.h>
#include <iostream>
#include <vector>
#include <array>
#include <omp.h>
#include <time.h>
#include "Dividing.h"
#include "Sieve_sequential.h"
#include "Sieve_domain.h"
#include "Sieve_function.h"

constexpr auto MIN = 2;
constexpr auto MAX = 100000000;
constexpr auto NUM_THREADS = 2;

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
	omp_set_num_threads(NUM_THREADS);
	double tstart, tstop;
	std::vector<int> primes = { };

	
	primes = { };
	tstart = clock();
	fillWithPrimes_sieve(primes, MIN, MAX);
	tstop = clock();
	std::cout << "Sito sekwencyjne: " << (double) (tstop - tstart) / CLOCKS_PER_SEC << " \n";
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
	

	/*
	primes = { 2 };
	tstart = clock();
	fillWithPrimes_divide(primes, MIN, MAX);
	tstop = clock();
	std::cout << "Przez dzielenie: " << (tstop - tstart) / CLOCKS_PER_SEC << " \n";
	printPrimesSize(primes, MIN, MAX);
	//printPrimes(primes, MIN, MAX);
	*/
}