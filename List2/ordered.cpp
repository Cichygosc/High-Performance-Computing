#include <omp.h>
#include <iostream>

#define n 10

using std::cout;
using std::endl;

int main()
{
	int nthreads, tid;

	#pragma omp parallel private(tid) shared(nthreads)
	{
		tid = omp_get_thread_num();
		if (tid == 0)
		{
			nthreads = omp_get_num_threads();
			cout << "Number of threads = " << nthreads << endl;
		}

		cout << "Thread " << tid << " starting\n";
		int myval;
		size_t i;
		#pragma omp parallel for private(myval, i) ordered
		for (i = 1; i <= n; ++i)
		{
			myval = i + 1;
			#pragma omp ordered
			{
				cout << "Thread " << tid << " " << i << " " << myval << endl;
			}
		}
	}
}