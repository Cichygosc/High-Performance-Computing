#include <omp.h>
#include <iostream>
#include <cmath>

#define n 10
#define pi 3.14159

using std::cout;
using std::endl;

int main()
{
	int tid, nthreads;
	int a[n];
	size_t i;
	float x = 0.0;

	#pragma omp parallel private(tid, i) shared(nthreads, a, x)
	{
		tid = omp_get_thread_num();
		if (tid == 0)
		{
			nthreads = omp_get_num_threads();
			cout << "Number of threads = " << nthreads << endl;
		}

		cout << "Thread " << tid << " starting\n";
		#pragma omp parallel for lastprivate(x)
		for (i = 1; i <= n; ++i)
		{
			x = i * 7;
			a[i] = exp(x);
		}
	}
	cout << "x: " << x << endl;
}
