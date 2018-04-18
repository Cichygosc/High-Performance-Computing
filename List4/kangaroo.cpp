#include "kangaroo.hpp"
#include <openssl/sha.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;
using namespace NTL;

Kangaroo::Kangaroo(ZZ interval, ZZ p, ZZ q, ZZ g, ZZ y)
	:interval{interval}, p{p}, q{q}, g{g}, y{y}, k{0}, u{3}, v{1}
{
	generateJumps(interval);
}

ZZ Kangaroo::findX()
{
	ZZ xTame = PowerMod(g, interval / 2, p);
	ZZ dTame = conv<ZZ>(0);
	ZZ xWild = y;
	ZZ dWild = conv<ZZ>(0);
	bool finished = false;

	ZZ dTameFinish = conv<ZZ>(0);
	ZZ dWildFinish = conv<ZZ>(0);
	do
	{	
		nextStep(xTame, dTame);
		if (isDistinguished(xTame))
		{
			if (isAlreadyInSet(xTame))
			{
				if (distinguishedPoints[xTame].first != KangarooType::Tame)
				{
					dTameFinish = dTame;
					dWildFinish = distinguishedPoints[xTame].second;
					finished = true;
				}
			}	
			else
			{
				addDistinguished(xTame, dTame, KangarooType::Tame);
			}
		}

		nextStep(xWild, dWild);
		if (isDistinguished(xWild))
		{
			if (isAlreadyInSet(xWild))
			{
				if (distinguishedPoints[xWild].first != KangarooType::Wild)
				{
					dTameFinish = distinguishedPoints[xWild].second;
					dWildFinish = xWild;
					finished = true;
				}
			}	
			else
			{
				addDistinguished(xWild, dWild, KangarooType::Wild);
			}
		}

	} while (!finished);

	return interval / 2 + dTameFinish - dWildFinish;
}

void Kangaroo::nextStep(ZZ & x, ZZ & d)
{
	int h = hash(x);
	x = MulMod(x, R[h], p);
	d += S[h];
}

bool Kangaroo::isDistinguished(const ZZ & x)
{
	return (x & 0x3FF) == conv<ZZ>(0);
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