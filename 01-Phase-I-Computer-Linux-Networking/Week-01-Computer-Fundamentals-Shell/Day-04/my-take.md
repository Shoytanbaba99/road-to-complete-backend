## 💡 Key Takeaways

Linux kernel only has understanding of C. The bash intercepts whatever we write into terminal and translates it into C code that the kernel can understand.
like execve(), dup2() fork() etc. whatever command we write the shell intercepts and parses it and breaks it into token.

File descriptors are used to represent files in the kernel, every process gets its own file descriptor table, linux considers everything as a file, even sockets and pipes. By Posix convention 0,1,2 are stdin, stdout, stderr. and 3 4 5 and others can be assigned to any file or socket. when we call and run any command on shell, like cat > file.txt the ">" is a redirection operator, the shell will open the file.txt and assign it to file descriptor 1 (stdout) and then call execve() to run cat command. else it would have printed the output to the terminal. when we run strace we can literally see the kernel calls and how the shell is interacting with the kernel with C. ther is dup (3,1) which duplicates file descriptor 3 to 1. openat (FDCW, "file.txt", O_WRONLY|O_CREAT|O_TRUNC, 0666) which here opens the file.txt and assigns it to file descriptor and allows for writing, or creating the file if it doesn't exist, and truncating it if it does. then execve() is called to execute the cat command with the new file descriptor setup.

There is also pipe | use connects the stdin of one process to the stdout of another process. the shell creates a pipe using hte pipe() system call, the one on the left of the pipe is assigned to file descriptor 1 (stdout/read) and the one on the right is assigned to file descriptor 0 (stdin/write). then execve() is called to run the commands on both sides of the pipe.

## 🔬 Practical Lab Findings
