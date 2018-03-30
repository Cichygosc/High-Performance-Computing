#include <omp.h>
#include <iostream>

#define Ndim 5
#define Mdim 6
#define Pdim 7

using std::cout;
using std::endl;

int main()
{
	float tmp;
	int nthreads, tid;
	size_t i, j, k;
	int calculatedValues = 0;
	int A[(Ndim - 1) * Ndim + Pdim], B[(Pdim - 1) * Pdim + Mdim], C[(Ndim - 1) * Ndim + Mdim];

	for (i = 0; i < (Ndim - 1) * Ndim + Pdim; ++i)
	{
		A[i] = i + 1;
	}

	for (i = 0; i < (Pdim - 1) * Pdim + Mdim; ++i)
	{
		B[i] = i + 1;
	}

	for (i = 0; i < (Ndim - 1) * Ndim + Mdim; ++i)
	{
		C[i] = 0;
	}

	#pragma omp parallel firstprivate(calculatedValues) private(i, j, k, tid) shared(A, B, C, nthreads)
	{
		tid = omp_get_thread_num();
		#pragma omp parallel for schedule(dynamic) private(tmp, i, j, k)
		for (i = 0; i < Ndim; ++i)
		{
			for (j = 0; j < Mdim; ++j)
			{
				tmp = 0.0;
				for (k = 0; k < Pdim; k++)
				{
					tmp += *(A + (i * Ndim + k)) * *(B + (k * Pdim + j));
					calculatedValues++;
					// cout << "Thread " << tid << " tmp: " << tmp << endl;
				}
				*(C + (i * Ndim + j)) = tmp;
			}
		}
		cout << "Thread " << tid << " values: " << calculatedValues << endl;
	}

	for (i = 0; i < Ndim; ++i)
	{
		for (int j = 0; j < Mdim; ++j)
		{
			cout << C[i * Ndim + j] << " ";
		}
		cout << endl;
	}
}