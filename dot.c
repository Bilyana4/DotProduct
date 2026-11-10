#include "dot.h"

#include <omp.h>


double dot_serial(const double *a, const double *b, size_t n) {
    double sum = 0.0;

    for (size_t i = 0; i < n; ++i) {
        sum += a[i] * b[i];
    }

    return sum;
}

double dot_parallel(const double *a, const double *b, size_t n, int threads) {
    double sum = 0.0;

    if (n == 0) {
        return 0.0;
    }

    if (threads > 1) {
        omp_set_num_threads(threads);
    }

#pragma omp parallel for reduction(+:sum) schedule(static) num_threads(threads)
    for (size_t i = 0; i < n; ++i) {
        sum += a[i] * b[i];
    }

    return sum;
}
