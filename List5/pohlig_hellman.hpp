#ifndef POHLIG_HELLMAN_HPP
#define POHLIG_HELLMAN_HPP

#include <vector>
#include <NTL/ZZ.h>

using Congruence = std::vector<std::pair<NTL::ZZ, NTL::ZZ>>;

class Pohlig_Hellman
{
public:
	Pohlig_Hellman(NTL::ZZ g, NTL::ZZ y, NTL::ZZ p, std::vector<NTL::ZZ> primes, std::vector<long> alphas);
	NTL::ZZ findX();
private:
	NTL::ZZ g;
	NTL::ZZ y;
	NTL::ZZ p;
	std::vector<NTL::ZZ> primes;
	std::vector<long> alphas;

private:
	NTL::ZZ findX(NTL::ZZ & pi, NTL::ZZ & gi, NTL::ZZ & hi, long & ei);
	NTL::ZZ CRT(const Congruence & congruence);
};

#endif