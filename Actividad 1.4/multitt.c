#include <stdio.h>
#include <omp.h>
static long numpasos = 100000;
double paso;
#define NUM_THREADS 20
void main()
{
    int i, nthreads;
    double pi, sum[NUM_THREADS], t1, t2, tiempo;
    paso = 1.0/numpasos;
    omp_set_num_threads(NUM_THREADS);
    const double startTime = omp_get_wtime();
    t1 = omp_get_wtime();
#pragma omp parallel
{
    int i, id, nthrds;
    double x;
    id = omp_get_thread_num();
    nthrds = omp_get_num_threads();
    if (id == 0) nthreads = nthrds;

    for (i = id, sum[id] = 0.0; i < numpasos; i = i + nthrds)
    {
        x = (i + 0.5) * paso;
        sum[id] += 4.0/(1.0 + (x*x));
    }
}
for (i = 0, pi = 0.0; i < nthreads; i++)
    pi += sum[i] * paso;
const double endTime = omp_get_wtime();
tiempo = t2-t1;
printf("Pi = %lf\n", pi);
printf("Tiempo total = %lf\n", (endTime - startTime));
}

