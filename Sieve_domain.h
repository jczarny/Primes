#pragma once
std::vector<int> getRanges(int num_thread, int n_threads, int min, int max);
void Sieve_parallel_domain(std::vector<int>& primes, int min, int max, int segSize);