Alright, Go has Interface named Context, which is designed to solve the problem of deadlines and cancellation.

we create a context.Background() at the top level, and then we can create a context.WithTimeout() or context.WithCancel() to establish a deadline or cancellation signal. We can cancel context.WithTimeout() early and manually cancel context.WithCancel() when we want to signal an abort condition.

Also, we can use the ctx.Done() and ctx.Err() channels to see if the context has been canceled or the deadline has been exceeded.
