#include <omp.h>
#include <iostream>

#define jstart 7
#define n 10

using std::cout;
using std::endl;

int main()
{
	int tid, nthreads;
	int j = jstart;
	int a[n];
	size_t i;

	for (i = 0; i < n; ++i)
	{
		a[i] = i;
	}

	#pragma omp parallel private(tid, i) shared(nthreads, a)
	{
		tid = omp_get_thread_num();
		if (tid == 0)
		{
			nthreads = omp_get_num_threads();
			cout << "Number of threads = " << nthreads << endl;
		}

		cout << "Thread " << tid << " starting\n";
		#pragma omp parallel for firstprivate(j)
		for (i = 1; i <= n; ++i)
		{
			if (i == 1 || i == n)
			{
				j = j + 1;
				cout << "Thread " << tid << ": j=" << j << endl;
			}
			a[i - 1] = a[i - 1] + j;
			// cout << "Thread " << tid << ": a[" << i - 1 << "]= " << a[i - 1] << endl;
		}
	}
	cout << "j: " << j << endl;
}
