Tests are basically atuomated checks to see if the function is behaving as expected. In Go, to write tests, a file must end with `_test.go` and the test function must start with `Test` and take a single argument of type `*testing.T`.

Table Driven Test is basically creating a table of test cases and iterating over them to run the same test logic for each case. This is for when we have multiple inputs and expected outputs for a function.
