#include <omp.h>
#include <iostream>

#define N 100

using std::cout;
using std::endl;

int main()
{
	int nthreads, tid;
	size_t i;
	int a[N], b[N], c[N];

	for (i = 0; i < N; ++i)
	{
		a[i] = b[i] = i;
	}

	#pragma omp parallel shared(a, b, c, nthreads) private (i, tid)
	{
		tid = omp_get_thread_num();
		if (tid == 0)
		{
			nthreads = omp_get_num_threads();
			cout << "Number of threads = " << nthreads << endl;
		}

		cout << "Thread " << tid << " starting\n";
		#pragma omp for
		for (i = 0; i < N; ++i)
		{
			c[i] = a[i] + b[i];
			cout << "Thread " << tid << ": c[" << i << "]= " << c[i] << endl;
		}
	}
}