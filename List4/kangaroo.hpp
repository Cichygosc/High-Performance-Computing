#ifndef KANGAROO_HPP_
#define KANGAROO_HPP_

#include <NTL/ZZ_p.h>
#include <vector>
#include <map>
#include <utility>

enum KangarooType
{
	Tame,
	Wild
};

class Kangaroo
{
public:
	Kangaroo(NTL::ZZ interval, NTL::ZZ p, NTL::ZZ q, NTL::ZZ g, NTL::ZZ y);
	NTL::ZZ findX();
private:
	std::vector<NTL::ZZ> S;
	std::vector<NTL::ZZ> R;
	std::map<NTL::ZZ, std::pair<KangarooType, NTL::ZZ>> distinguishedPoints;
	NTL::ZZ interval;
	NTL::ZZ p;
	NTL::ZZ q;
	NTL::ZZ g;
	NTL::ZZ y;
	int k;
	int u;
	int v;
	
private:
	int hash(const NTL::ZZ & s);
	std::string sha256(const std::string str);
	std::string zzToString(const NTL::ZZ & z);
	void generateJumps(NTL::ZZ intervalLength);
	void nextStep(NTL::ZZ & x, NTL::ZZ & d);
	bool isDistinguished(const NTL::ZZ & x);
	bool isAlreadyInSet(const NTL::ZZ & x);
	void addDistinguished(const NTL::ZZ & x, const NTL::ZZ & d, KangarooType type);
};

#endif