#include<vector>
#include <iostream>

void fillWithPrimes_sieve(std::vector<int>& primes, int min, int max) {
	std::vector<bool> isPrime(max+1, true);

	int lim = sqrt(max);
	for (int div = 2; div <= max; div++) {
		if (isPrime[div]) {
			for (int val = 2 * div; val <= max; val += div) {
				isPrime[val] = false;
			}
			primes.push_back(div);
		}
	}
}