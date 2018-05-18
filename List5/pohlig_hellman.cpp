#include "pohlig_hellman.hpp"
#include "pollard.hpp"
#include <iostream>

using namespace std;
using namespace NTL;

using Congruence = vector<pair<ZZ, ZZ>>;

Pohlig_Hellman::Pohlig_Hellman(ZZ g, ZZ y, ZZ p, vector<ZZ> primes, vector<long> alphas)
	:g{g}, y{y}, p{p}, primes{primes}, alphas{alphas}
{
	
}

ZZ Pohlig_Hellman::findX()
{
	int r = primes.size() - 1;
	ZZ order = p - conv<ZZ>(1);
	Congruence congruence;
	for (int i = 0; i < r; ++i)
	{
		ZZ orderI = power(primes[i], alphas[i]);
		ZZ gi = PowerMod(g, order / orderI, p);
		ZZ hi = PowerMod(y, order / orderI, p);
		ZZ x = findX(primes[i], gi, hi, alphas[i]);
		congruence.emplace_back(x, orderI);
	}

	return CRT(congruence);
}

ZZ Pohlig_Hellman::findX(ZZ & pi, ZZ & gi, ZZ & hi, long & ei)
{
	ZZ inv;
	InvMod(inv, gi, p);
	ZZ x = conv<ZZ>(0);
	ZZ y = PowerMod(gi, power(pi, ei - 1), p);
	for (int k = 1; k < ei + 1; ++k)
	{
		ZZ hk = PowerMod(MulMod(PowerMod(inv, x, p), hi, p), power(pi, (ei - k)), p);
		// y^dk = hk mod p
		Pollard pollard(p, pi, y, hk);
		ZZ dk = pollard.findX();
		x += dk * power(pi, k - 1);
	}

	return x;
}

ZZ Pohlig_Hellman::CRT(const Congruence & congruence)
{
	ZZ N = congruence[0].second;
    ZZ X = ZZ(0);

    for (auto it = congruence.cbegin() + 1; it != congruence.cend(); it++) 
    {
        N *= it->second;
    }

    for (const auto& congruencePair : congruence)
     {
        ZZ ai = congruencePair.first;
        ZZ ni = congruencePair.second;
        ZZ mi = N / ni;
        ZZ d, s, t;
        XGCD(d, s, t, mi, ni);
        X += mi * ai * s;
    }
    return X % N;
}