# Dot Product with OpenMP

This project computes the dot product of two vectors using both serial and parallel implementations. It measures execution time, prints the speedup, and validates correctness against a known expected value.

## Build

```bash
make
```

## Run

```bash
./dot [n] [threads]
```

- `n` is the vector length and must be greater than 0.
- If `n` is omitted, the program asks for the vector length in the console.
- `threads` is the OpenMP thread count and must be greater than or equal to 1.
- If omitted, the program uses 1 thread.

On Windows, run `build_and_run.bat` to compile and start the program; it will prompt for `n` when no arguments are provided. You can also pass values directly, for example `build_and_run.bat 1000 2`.

## Test

```bash
make test
```

## Notes

- The dot product is a memory-bound operation, so speedup often saturates as memory bandwidth becomes the limiting factor.
- Floating-point addition is not associative, so small differences may appear between serial and parallel results, even when they are mathematically equivalent.
- The comparison uses a relative tolerance of `1e-9`.
