#include <iostream>
#include <vector>
#include <NTL/ZZ.h>
#include "pohlig_hellman.hpp"

using namespace std;
using namespace NTL;

constexpr int PRIME_BITS = 40;
constexpr int MAX_ALPHA = 3;
constexpr int K = 8;
constexpr int Q_BITS = 256;

ZZ generatePrime(vector<ZZ> & primes, vector<long> & alphas);

int main()
{
	SetSeed(conv<ZZ>(time(0)));

	vector<ZZ> primes(K + 1);
	vector<long> alphas(K + 1);

	ZZ p = generatePrime(primes, alphas);

	while (ProbPrime(p) == 0)
	{
		p = generatePrime(primes, alphas);
	}

	// g in (2..p-1)
	// x in (1..p-2)
	ZZ g = RandomBnd(p - 2) + conv<ZZ>(2);
	ZZ x = RandomBnd(p - 2) + conv<ZZ>(1);
	ZZ y = PowerMod(g, x, p);

	cout << "G: " << g << "\nX: " << x << "\nY: " << y << "\nP: " << p << endl;

	/* cout << p << endl;
	cout << NumBits(p) << endl;
	for (auto e : exponents)
		cout << e << endl;
	for (auto a : alphas)
		cout << a << endl; */

	Pohlig_Hellman alg(g, y, p, primes, alphas);
	ZZ foundX = alg.findX();
	ZZ Q = primes[K];
	ZZ GPrime = PowerMod(g, Q, p);
	ZZ yPrime = PowerMod(y, Q, p);
	PowerMod(GPrime, GPrime, foundX, p);
	if (GPrime == yPrime)
	{
		cout << "Success\n";
	}
	else
	{
		cout << "Fail\n";
	}

	return 0;
}

ZZ generatePrime(vector<ZZ> & primes, vector<long> & alphas)
{
	ZZ Q;
	GenPrime(Q,Q_BITS);
	ZZ p = Q;

	long alpha = RandomBnd(MAX_ALPHA) + 1;
	p *= power(conv<ZZ>(2), alpha);

	primes[0] = conv<ZZ>(2);
	alphas[0] = alpha;

	for (int i = 1; i < K; ++i)
	{
		ZZ prime;
		GenPrime(prime, PRIME_BITS);
		alpha = RandomBnd(MAX_ALPHA) + 1;
		primes[i] = prime;
		alphas[i] = alpha;
		p *= power(prime, alpha);
	}

	primes[K] = Q;
	alphas[K] = 1;

	return p + conv<ZZ>(1);
}