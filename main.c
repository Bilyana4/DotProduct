#include "dot.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_usage(const char *program_name) {
    fprintf(stderr, "Usage: %s [n] [threads] (n is requested if omitted)\n", program_name);
}

static int parse_positive_long(const char *text, long *value) {
    char *end = NULL;
    long parsed = 0;

    if (text == NULL || *text == '\0') {
        return 0;
    }

    errno = 0;
    parsed = strtol(text, &end, 10);
    while (isspace((unsigned char)*end)) {
        ++end;
    }
    if (errno != 0 || end == text || *end != '\0' || parsed <= 0) {
        return 0;
    }

    *value = parsed;
    return 1;
}

static int parse_positive_int(const char *text, int *value) {
    char *end = NULL;
    long parsed = 0;

    if (text == NULL || *text == '\0') {
        return 0;
    }

    errno = 0;
    parsed = strtol(text, &end, 10);
    if (errno != 0 || end == text || *end != '\0' || parsed <= 0) {
        return 0;
    }

    *value = (int)parsed;
    return 1;
}

int main(int argc, char *argv[]) {
    const int default_threads = 1;
    long n = 0;
    int threads = default_threads;
    char input[128];
    double *a = NULL;
    double *b = NULL;
    double serial_result = 0.0;
    double parallel_result = 0.0;
    double serial_time = 0.0;
    double parallel_time = 0.0;
    double serial_start = 0.0;
    double parallel_start = 0.0;
    double speedup = 0.0;
    const double tolerance = 1e-9;
    int serial_ok = 0;
    int parallel_ok = 0;
    int all_ok = 0;

    if (argc > 3) {
        print_usage(argv[0]);
        return 1;
    }

    if (argc == 1) {
        printf("Enter vector size n: ");
        fflush(stdout);
        if (fgets(input, sizeof(input), stdin) == NULL ||
            !parse_positive_long(input, &n)) {
            fprintf(stderr, "Invalid value for n. Enter a positive integer.\n");
            return 1;
        }
    } else if (!parse_positive_long(argv[1], &n)) {
        fprintf(stderr, "Invalid value for n: %s\n", argv[1]);
        print_usage(argv[0]);
        return 1;
    }

    if (argc == 3 && !parse_positive_int(argv[2], &threads)) {
        fprintf(stderr, "Invalid value for threads: %s\n", argv[2]);
        print_usage(argv[0]);
        return 1;
    }

    a = (double *)malloc((size_t)n * sizeof(double));
    b = (double *)malloc((size_t)n * sizeof(double));
    if (a == NULL || b == NULL) {
        fprintf(stderr, "Failed to allocate vectors for n = %ld\n", n);
        free(a);
        free(b);
        return 1;
    }

    init_vector(a, (size_t)n, 1.0);
    init_vector(b, (size_t)n, 2.0);

    serial_start = get_wall_time();
    serial_result = dot_serial(a, b, (size_t)n);
    serial_time = get_wall_time() - serial_start;

    parallel_start = get_wall_time();
    parallel_result = dot_parallel(a, b, (size_t)n, threads);
    parallel_time = get_wall_time() - parallel_start;

    if (parallel_time > 0.0 && serial_time > 0.0) {
        speedup = serial_time / parallel_time;
    }

    serial_ok = check_result(2.0 * (double)n, serial_result, tolerance);
    parallel_ok = check_result(2.0 * (double)n, parallel_result, tolerance);
    all_ok = serial_ok && parallel_ok;

    printf("n = %ld\n", n);
    printf("threads = %d\n", threads);
    printf("serial result = %.10f, time = %.6f s\n", serial_result, serial_time);
    printf("parallel result = %.10f, time = %.6f s\n", parallel_result, parallel_time);
    printf("speedup = %.6f\n", speedup);
    printf("RESULT: %s\n", all_ok ? "PASS" : "FAIL");

    free(a);
    free(b);

    return all_ok ? 0 : 1;
}
