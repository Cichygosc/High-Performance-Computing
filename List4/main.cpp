#include <iostream>
#include <NTL/ZZ.h>
#include <cmath>
#include <NTL/RR.h>
#include "kangaroo.hpp"

using namespace std;
using namespace NTL;

int main()
{
	ZZ p, q;

	do 
	{
		GenGermainPrime(p, 45);
		q = (p - 1) / 2;
	} while (ProbPrime(q) == 0);
	
	SetSeed(conv<ZZ>(time(0)));

	ZZ g;
	do
	{
		g = conv<ZZ>(RandomBnd(p));
		g = PowerMod(g, 2, p);
	} while (g == 1);

	ZZ interval = conv<ZZ>(conv<RR>(M_PI / 8) * conv<RR>(q));
	ZZ randomX = RandomBnd(interval);
	ZZ y = PowerMod(g, randomX, p);

	cout << "p: " << p << " q: " << q << " g: " << g <<  " y: " << y << endl;
	cout << "Interval: " << interval << endl;
	cout << "RandomX: " << randomX << endl;

	Kangaroo kangaroo(interval, p, q, g, y);
	cout << kangaroo.findX() << endl;


	return 0;
}