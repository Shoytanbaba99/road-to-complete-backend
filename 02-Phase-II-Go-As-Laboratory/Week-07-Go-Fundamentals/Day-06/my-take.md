So, anything can be an error in Go, as long as it implements the error error interface, which is just a single method called Error() that returns a string.

There are layers to errors, Sentinel errors are fixed standard errors, and they return a pointer to a single instance of an error. Custom errors are structs that hold extra details. Like if i wanted an erorr coming from database with a status code and endpoint, i can create a struct with those fields and implement the Error() method on it. We could implement these and the error message may return independantly or we could wrap the error with fmt.Errorf() and the %w verb, which allows us to wrap the inner error inside our new message.

Now to peer into errors, there is errors.Is() which checks if an error is in the error chain, and errors.As() which allows us to unpack and check a TYPE of error, and if it is the type we are looking for, we can extract it into a variable.

aaand in latest go of 1.20+ you have errors.Join() which allows you to join multiple errors into one error.
