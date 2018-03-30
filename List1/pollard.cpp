#include "pollard.hpp"
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;
using std::pair;

Pollard::Pollard(ZZ p, ZZ q, ZZ_p g, ZZ_p y)
	 :p{p}, q{q}, g{g}, y{y}
{
	
}

ZZ_p Pollard::findX()
{
	ZZ_p A = conv<ZZ_p>(1), B = conv<ZZ_p>(1);
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

	cout << "Steps: " << i << endl;
	ZZ_p::init(q);
	ZZ_p diff1 = conv<ZZ_p>(beta.first - alpha.first);
	ZZ_p diff2 = conv<ZZ_p>(alpha.second - beta.second);
	return diff1 / diff2;
}

void Pollard::nextStep(ZZ_p & a, pair<ZZ, ZZ> & alpha)
{
	int modulo = conv<ZZ>(a) % 3;

	if (modulo == 1)
	{
		a *= y;
		alpha.second = AddMod(alpha.second, conv<ZZ>(1), q);
		// alpha.second++;
		return;
	}

	if (modulo == 0)
	{
		a *= a;
		alpha.first = MulMod(alpha.first, conv<ZZ>(2), q);
		alpha.second = MulMod(alpha.second, conv<ZZ>(2), q);
		// alpha.first *= 2;
		// alpha.second *= 2;
		return;
	}

	if (modulo == 2)
	{
		a *= g;
		alpha.first = AddMod(alpha.first, conv<ZZ>(1), q);
		// alpha.first++;
		return;
	}

}

