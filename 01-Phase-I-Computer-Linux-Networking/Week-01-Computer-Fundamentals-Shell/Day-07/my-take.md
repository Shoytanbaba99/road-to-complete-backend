## 🧠 Core Mental Model

The program is essentially a bunch of dead code sitting on the hard drive waiting to be called; it contains the instructions and initial variables. The process is the program running; it is the kernel which runs the process and allows it to get CPU time. It declares a virtual memory space, making the process think it has unlimited space starting from 0x00000000 to 0xFFFFFFFF. But the MMU maps the virtual memories to physical memory. Each virtual memory page is around 4 KB big.

Linux uses file descriptors, so everything on Linux is a process, and every process has a file descriptor table. 0, 1, and 2 are standard input, output, and error. 3 is the first file descriptor for a file opened by the process.

Each process can initialize threads, which are lightweight and share the same virtual memory space, instead of cloning and creating child processes.

Oh, there is also PATH, which is an environment variable. It could be local to a terminal or global and be inside ~/.bashrc or ~/.zshrc.
