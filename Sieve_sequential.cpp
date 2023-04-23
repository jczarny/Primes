#include <vector>

void primitive_sieve(std::vector<int>& primes, int min, int max) {
	int size = max - min + 1;
	std::vector<bool> isPrime(size, true);

	for (int div = 2; div <= max; div++) {
		if (isPrime[div-min]) {
			for (int val = 2 * div; val <= max; val += div) {
				isPrime[val-min] = false;
			}
			primes.push_back(div);
		}
	}
}

void fillWithPrimes_sieve(std::vector<int>& primes, int min, int max, int segSize) {
	int sqrt_max = sqrt(max);
	int size = max - min + 1;
	primitive_sieve(primes, 2, sqrt_max);

	std::vector<bool> isPrime(size, true);
	int primesSize = primes.size();
	int segLow = min;
	int segHigh = min + segSize;

	while (segLow < segHigh) {
		if (segHigh > max)
			segHigh = max;

		for (int i = 0; i < primesSize; i++) {
			int start = int(segLow / primes[i]) * primes[i];
			if (start < segLow)
				start += primes[i];
			for (int j = start; j <= segHigh; j += primes[i]) {
				isPrime[j - min] = false;
			}
		}
		for (int i = segLow - min; i < segHigh - min; i++) {
			if (isPrime[i]) {
				primes.push_back(i + min);
			}
		}

		segLow += segSize;
		segHigh += segSize;
	}
}