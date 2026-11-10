#include "dot.h"

#include <math.h>
#include <omp.h>

void init_vector(double *vector, size_t n, double value) {
    if (vector == NULL || n == 0) {
        return;
    }

#pragma omp parallel for schedule(static)
    for (size_t i = 0; i < n; ++i) {
        vector[i] = value;
    }
}

int check_result(double expected, double actual, double tolerance) {
    const double diff = fabs(expected - actual);
    const double scale = fmax(fabs(expected), fabs(actual));

    if (scale == 0.0) {
        return diff <= tolerance;
    }

    return diff <= tolerance * scale;
}

double get_wall_time(void) {
    return omp_get_wtime();
}
