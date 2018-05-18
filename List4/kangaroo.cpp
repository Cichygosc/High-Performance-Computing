#include "kangaroo.hpp"
#include <openssl/sha.h>
#include <omp.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;
using namespace NTL;

Kangaroo::Kangaroo(ZZ interval, ZZ p, ZZ q, ZZ g, ZZ y)
	:interval{interval}, p{p}, q{q}, g{g}, y{y}, k{0}
{
	generateJumps(interval);
}

ZZ Kangaroo::findX()
{
	ZZ intervalMiddle = interval / 2;
	ZZ resultTame;
	ZZ resultWild;
	bool finished = false;
	#pragma omp parallel firstprivate(p, q, g, y, intervalMiddle) shared(distinguishedPoints, finished, resultTame, resultWild)
	{
		int tid = omp_get_thread_num();
		// m = 4
		// B = m * sqr(b - a) / 4
		// v = B / (m / 2)
		NTL::ZZ v = SqrRoot(interval) / 2;
		NTL::ZZ x;
		NTL::ZZ d;
		KangarooType type;
		if (tid < 2)
		{
			type = KangarooType::Tame;
			x = PowerMod(g, intervalMiddle + tid * v, p);
			d = tid * v;
		}
		else
		{
			type = KangarooType::Wild;
			x = MulMod(y, PowerMod(g, (tid - 2) * v, p), p);
			d = (tid - 2) * v;
		}

		// cout << type << " starts at " << x << " with distance " << d << endl;

		do
		{
			nextStep(x, d);
			if (isDistinguished(x))
			{
				if (isAlreadyInSet(x))
				{
					// cout << type << " found same as " << distinguishedPoints[x].first << endl;
					if (distinguishedPoints[x].first != type)
					{
						if (type == KangarooType::Tame)
						{
							resultTame = d;
							resultWild = distinguishedPoints[x].second;
							finished = true;
						}
						else
						{
							resultTame = distinguishedPoints[x].second;
							resultWild = d;
							finished = true;
						}
					}
				}
				else
				{
					// cout << type << " found new at " << x << endl;
					addDistinguished(x, d, type);
				}
			}

		} while (!finished);
	}

	return AddMod(intervalMiddle, SubMod(resultTame, resultWild, q), q);
}

void Kangaroo::nextStep(ZZ & x, ZZ & d)
{
	int h = hash(x);
	x = MulMod(x, R[h], p);
	d = AddMod(d, S[h], q);
}

bool Kangaroo::isDistinguished(const ZZ & x)
{
	return (x & 0xFFF) == conv<ZZ>(0);
}

bool Kangaroo::isAlreadyInSet(const ZZ & x)
{
	return distinguishedPoints.find(x) != distinguishedPoints.end();
}

void Kangaroo::addDistinguished(const ZZ & x, const ZZ & d, KangarooType type)
{
	distinguishedPoints.insert(pair<ZZ, pair<KangarooType, ZZ>>(x, {type, d}));
}

void Kangaroo::generateJumps(ZZ intervalLength)
{
	ZZ s;
	ZZ meanValue;
	ZZ sqrRoot = SqrRoot(intervalLength);
	int pow = 0;
	do
	{
		s = power2_ZZ(pow);
		S.push_back(s);
		R.push_back(PowerMod(g, s, p));
		k++;
		pow++;
		meanValue = (s - 1) / pow;
	} while (meanValue < sqrRoot);
}

int Kangaroo::hash(const ZZ & s)
{
	string sString = zzToString(s);
	string sha = sha256(sString);
	ZZ number (NTL::INIT_VAL, sha.c_str());
	return number % k;
}

string Kangaroo::zzToString(const ZZ & z)
{
	stringstream buffer;
	buffer << z;
	return buffer.str();
}

string Kangaroo::sha256(const string str)
{
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, str.c_str(), str.size());
	SHA256_Final(hash, &sha256);
	stringstream ss;
	for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
	{
		ss << dec << setw(2) << setfill('0') << (int)hash[i];
	}

	return ss.str();
}