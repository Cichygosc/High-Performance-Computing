#ifndef POLLARD_HPP_
#define POLLARD_HPP_

#include <NTL/ZZ_p.h>
#include <set>
#include <utility>

using namespace NTL;

class Pollard
{
public:
	Pollard(ZZ p, ZZ q, ZZ_p g, ZZ_p y);
	ZZ_p findX();
private:
	ZZ p;
	ZZ q;
	ZZ_p g;
	ZZ_p y;
private:
	void nextStep(ZZ_p & a, std::pair<ZZ, ZZ> & alpha);
};

#endif