#include <vector>

bool isPrime(std::vector<int>& primes, int num) {
	int j = primes.size();
	int snum = sqrt(num);
	int i = 0;
	while (true) {
		if (num % primes[i] == 0) return false;
		else if (snum < primes[i]) return true;

		i += 1;
		if (i == j) {
			int tempPrime = primes.back() + 1;
			while (!isPrime(primes, tempPrime)) { tempPrime += 1; };
			primes.push_back(tempPrime);
			j += 1;
		}
	}
}


void fillWithPrimes_divide(std::vector<int>& primes, int min, int max) {
	for (int num = 2; num < max; num++) {
		if (isPrime(primes, num))
			primes.push_back(num);
	}
}