Well, Go's way of getting argc, argv, and environment variables is through the os package. The os.Args slice contains the command-line arguments, and the os.Environ() function returns a slice of strings representing the environment variables. We can use os.Getenv() to retrieve the value of a specific environment variable.

And for flags, Go has a built-in flag package that provides a simple way to parse these command line flags easily. we define flags and then call flag.Parse() to parse the command line arguments.

```go
flag.StringVar(&filePath, "file", "default.json", "path to tasks file")
```

like this one, here rfilepath is the varaible, file is the name default.json is the default fallback value and path to tasks file is the description of the flag.

The heirarchy of these variables is something like, Cli flag > Environment Variable > Default Value.

i guess i also leanred that -- indicates the end of the flags, and anything after that is treated as arguments.
