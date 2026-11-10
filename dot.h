#ifndef DOT_H
#define DOT_H

#include <stddef.h>

double dot_serial(const double *a, const double *b, size_t n);
double dot_parallel(const double *a, const double *b, size_t n, int threads);
void init_vector(double *vector, size_t n, double value);
int check_result(double expected, double actual, double tolerance);
double get_wall_time(void);

#endif
