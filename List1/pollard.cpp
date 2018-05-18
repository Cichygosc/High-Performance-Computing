#include "pollard.hpp"
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;
using std::pair;

Pollard::Pollard(ZZ p, ZZ q, ZZ g, ZZ y)
	 :p{p}, q{q}, g{g}, y{y}
{
	
}

ZZ Pollard::findX()
{
	ZZ A = conv<ZZ>(1), B = conv<ZZ>(1);
	size_t i = 0;
	pair<ZZ, ZZ> alpha(0, 0);
	pair<ZZ, ZZ> beta(0, 0);
	
	do
	{
		i++;
		nextStep(A, alpha);
		nextStep(B, beta);
		nextStep(B, beta);
	} while (A != B);

	ZZ diff11 = SubMod(result1.first, result2.first, q);
	ZZ diff22 = SubMod(result2.second, result1.second, q);
	return MulMod(diff11, InvMod(diff22, q), q);
}

void Pollard::nextStep(ZZ & a, pair<ZZ, ZZ> & alpha)
{
	int modulo = conv<ZZ>(a) % 3;

	if (modulo == 1)
	{
		a = MulMod(a, y, p);
		alpha.second = AddMod(alpha.second, conv<ZZ>(1), q);
		return;
	}

	if (modulo == 2)
	{
		a = MulMod(a, a, p);
		alpha.first = MulMod(alpha.first, conv<ZZ>(2), q);
		alpha.second = MulMod(alpha.second, conv<ZZ>(2), q);
		return;
	}

	if (modulo == 0)
	{
		a = MulMod(a, g, p);
		alpha.first = AddMod(alpha.first, conv<ZZ>(1), q);
		return;
	}
}

