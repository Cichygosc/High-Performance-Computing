#include "pollard.hpp"
#include <omp.h>
#include <iostream>
#include <algorithm>

using std::cout;
using std::cerr;
using std::endl;
using std::pair;
using std::hex;

#define MAX_LENGTH 81920

Pollard::Pollard(ZZ p, ZZ q, ZZ g, ZZ y)
	 :p{p}, q{q}, g{g}, y{y}
{
	
}

ZZ Pollard::findX()
{
	bool finished = false; 
	pair<ZZ, ZZ> result1;
	pair<ZZ, ZZ> result2;
	#pragma omp parallel shared(p, q, g, y, distinguisedPoints, finished, result1, result2)
	{
		size_t i = 0;

		pair<ZZ, ZZ> alpha;
		ZZ A;
		newStartPoint(A, alpha);

		while (!finished)
		{
			i++;
		    nextStep(A, alpha);

			if ((A & 0x3FF) == conv<ZZ>(0))
			{
				if (distinguisedPoints.find(A) != distinguisedPoints.end())
				{
					if (distinguisedPoints[A] != alpha)
					{
						result1 = alpha;
						result2 = distinguisedPoints[A];
						finished = true;	
					}
					else
					{
						i = 0;
						newStartPoint(A, alpha);
					}
				}
				else
				{
					distinguisedPoints.insert(pair<ZZ, pair<ZZ, ZZ>>(A, alpha));
					newStartPoint(A, alpha);
				}
			}

			if (i > MAX_LENGTH)
			{
				i = 0;
				newStartPoint(A, alpha);
				alpha.first = RandomBnd(q);
			}
		}
	}


	ZZ diff11 = SubMod(result1.first, result2.first, q);
	ZZ diff22 = SubMod(result2.second, result1.second, q);
	return MulMod(diff11, InvMod(diff22, q), q);
}

void Pollard::newStartPoint(ZZ & a, pair<ZZ, ZZ> & alpha)
{
	alpha.first = RandomBnd(q);
	alpha.second = RandomBnd(q);
	ZZ g0 = PowerMod(g, alpha.first, p);
	ZZ y0 = PowerMod(y, alpha.second, p);
	a = MulMod(g0, y0, p);
}

void Pollard::nextStep(ZZ & a, pair<ZZ, ZZ> & alpha)
{
	int modulo = a % 3;

	if (modulo == 1)
	{
		a = MulMod(a, y, p);
		alpha.second = AddMod(alpha.second, conv<ZZ>(1), q);
		return;
	}

	if (modulo == 0)
	{
		a = MulMod(a, a, p);
		alpha.first = MulMod(alpha.first, conv<ZZ>(2), q);
		alpha.second = MulMod(alpha.second, conv<ZZ>(2), q);
		return;
	}

	if (modulo == 2)
	{
		a = MulMod(a, g, p);
		alpha.first = AddMod(alpha.first, conv<ZZ>(1), q);
		return;
	}
}

