## 🧠 Core Mental Model

fd, the file descriptors points to custom resources available in the kernel. The kernel maintains a table of file descriptors for each process. Each entry in this table points to an open file description, which points to inode table entry. The kernel uses this table to manage access to files and other resources.

Essentially every single resources of the kernel is considered a file. By POSIX convention, 0,1,2 are reserved for stdin, stdout, and stderr respectively. Other fds, are allocated as needed, and are used to access files, sockets, pipes, and other resources. We can use read(), write(), open(), close() and dup2() to manipulate these resources with the fd that the kernel provides to the user space.
