Along with Testing, Go has built in Fuzzing and Benchmarking capabilities. Benchmark checks for performance (speed and memory and heap allocations) and Fuzzing checks for edge cases and unexpected inputs by randomly generating inputs.

both must be in a file that ends with `_test.go` and must be in the same package as the code being tested. The function name must start with `Benchmark` or `Fuzz` and take a single pointer argument of type `*testing.B` or `*testing.F` respectively.
