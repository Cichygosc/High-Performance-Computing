#include <iostream>
#include <NTL/ZZ.h>
#include "pollard.hpp"

using namespace std;
using namespace NTL;

int main()
{
	ZZ p, q;

	do 
	{
		GenGermainPrime(p, 50);
		q = (p - 1) / 2;
	} while (ProbPrime(q) == 0);
	
	SetSeed(conv<ZZ>(time(0)));

	ZZ g = conv<ZZ>(1);
	do
	{
		g = conv<ZZ>(RandomBnd(p));
		g = PowerMod(g, 2, p);
	} while (g == 1);

	ZZ randomX = RandomBnd(q);
	ZZ y = PowerMod(g, randomX, p);

	cout << "p: " << p << " q: " << q << " g: " << g <<  " y: " << y << endl;
	cout << "RandomX: " << randomX << endl;

	Pollard pollard(p, q, g, y);
	cout << "FoundX: " << pollard.findX() << endl;

	return 0;
}