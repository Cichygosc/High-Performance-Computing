#include <iostream>
#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;

ZZ generatePrime(int primeLength, int k);

int main()
{
	SetSeed(conv<ZZ>(time(0)));
	ZZ p = generatePrime(40, 8);
	while (ProbPrime(p) == 0)
	{
		p = generatePrime(40, 8);
	}

	cout << p << endl;
	cout << NumBits(p) << endl;
	return 0;
}

ZZ generatePrime(int primeLength, int k)
{
	ZZ p = GenPrime_ZZ(256, 90);
	p *= power(conv<ZZ>(2), RandomBnd(10) + 3);

	for (int i = 1; i < k; ++i)
	{
		ZZ prime;
		GenPrime(prime, primeLength);
		long alpha = RandomBnd(10) + 3;
		p *= power(prime, alpha);
	}

	return p + conv<ZZ>(1);
}