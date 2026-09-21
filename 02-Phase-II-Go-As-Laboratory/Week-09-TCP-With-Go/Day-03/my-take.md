Go Routines aer essentially, user level threads. They are managed by the Go runtime and are much lighter than OS threads. When you use the `go` keyword to start a function, it runs concurrently in its own goroutine. Concurrently means it handles each problem independently, and Parallelism means its handling multiple problem simultaneously.

GO uses M:N scheduling, where M goroutines are multiplexed onto N OS threads. It uses GMP model, G is go routine, M is OS thread and P is processor.
