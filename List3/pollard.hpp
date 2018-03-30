#ifndef POLLARD_HPP_
#define POLLARD_HPP_

#include <NTL/ZZ_p.h>
#include <map>
#include <utility>

using namespace NTL;

class Pollard
{
public:
	Pollard(ZZ p, ZZ q, ZZ g, ZZ y);
	ZZ findX();
private:
	ZZ p;
	ZZ q;
	ZZ g;
	ZZ y;
	std::map<ZZ, std::pair<ZZ, ZZ>> distinguisedPoints;
private:
	void nextStep(ZZ & a, std::pair<ZZ, ZZ> & alpha);
	void newStartPoint(ZZ & a, std::pair<ZZ, ZZ> & alpha);
};

#endif