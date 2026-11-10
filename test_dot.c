#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/dot.h"

static int test_known_values(void) {
    const double a[] = {1.0, 2.0, 3.0};
    const double b[] = {4.0, 5.0, 6.0};
    const double expected = 32.0;
    const double serial = dot_serial(a, b, 3);
    const double parallel = dot_parallel(a, b, 3, 2);

    if (!check_result(expected, serial, 1e-9) || !check_result(expected, parallel, 1e-9)) {
        fprintf(stderr, "test_known_values failed: serial=%.10f parallel=%.10f expected=%.10f\n",
                serial, parallel, expected);
        return 1;
    }

    return 0;
}

static int test_zero_vector(void) {
    const size_t n = 64;
    double a[n];
    double b[n];
    const double expected = 0.0;

    for (size_t i = 0; i < n; ++i) {
        a[i] = 0.0;
        b[i] = 10.0;
    }

    if (!check_result(expected, dot_serial(a, b, n), 1e-9) ||
        !check_result(expected, dot_parallel(a, b, n, 4), 1e-9)) {
        fprintf(stderr, "test_zero_vector failed\n");
        return 1;
    }

    return 0;
}

static int test_single_element(void) {
    const double a[] = {9.0};
    const double b[] = {7.0};
    const double expected = 63.0;

    if (!check_result(expected, dot_serial(a, b, 1), 1e-9) ||
        !check_result(expected, dot_parallel(a, b, 1, 3), 1e-9)) {
        fprintf(stderr, "test_single_element failed\n");
        return 1;
    }

    return 0;
}

static int test_non_divisible_threads(void) {
    const size_t n = 11;
    double a[n];
    double b[n];
    double expected = 0.0;

    for (size_t i = 0; i < n; ++i) {
        a[i] = (double)(i + 1);
        b[i] = 2.0 * (double)(i + 1);
        expected += a[i] * b[i];
    }

    if (!check_result(expected, dot_serial(a, b, n), 1e-9) ||
        !check_result(expected, dot_parallel(a, b, n, 3), 1e-9)) {
        fprintf(stderr, "test_non_divisible_threads failed\n");
        return 1;
    }

    return 0;
}

static int test_check_result(void) {
    if (!check_result(0.0, 0.0, 1e-9) || !check_result(100.0, 99.9999999995, 1e-9)) {
        fprintf(stderr, "test_check_result failed\n");
        return 1;
    }

    return 0;
}

static int test_invalid_cli_arguments(void) {
    const int status = system("./dot 0 0 >/dev/null 2>&1");
    if (status == 0) {
        fprintf(stderr, "test_invalid_cli_arguments failed\n");
        return 1;
    }

    return 0;
}

int main(void) {
    int failures = 0;

    failures += test_known_values();
    failures += test_zero_vector();
    failures += test_single_element();
    failures += test_non_divisible_threads();
    failures += test_check_result();
    failures += test_invalid_cli_arguments();

    if (failures != 0) {
        fprintf(stderr, "Validation failed with %d failing test(s).\n", failures);
        return 1;
    }

    printf("All tests passed.\n");
    return 0;
}
