GO language initializes in a directory with go mod init <name>. It creates a file with .mod which contains the name of the module and its dependencies. The go.mod file is used to manage the dependencies of a Go project.

Any folders with go file inside the initalized directory will be considered as a package.
go fmt command formats the code in the current directory and its subdirectories. go vet
runs static analysis on the code to find potential issues. go run compiles and runs the Go program. go build compiles the code and creates an executable file. go test runs the tests in the current directory and its subdirectories. go get is used to download and install packages from remote repositories.

go env command displays the environment variables, like GOOS, GOARCH, GOPATH, GOROOT, etc.
