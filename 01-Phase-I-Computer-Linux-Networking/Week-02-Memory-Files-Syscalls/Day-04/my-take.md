## 💡 Key Takeaways

The Cpu itself as 4 privilege ring, linux and windows uses 2 of them, kernel mode, and usermode. The kernel mode has access to all the resources of the system, while the user mode has limited access. The kernel mode is where the operating system runs, while the user mode is where applications run.

to access anything of kernel level from application/user level, we need to use system calls. The system calls are the interface between the user mode and the kernel mode. The system call is a insturction that puts the cpu in kernel mode for a very brief moment, checks the vector table for the system call number, execute the instruction if available and returns to user mode. The system call is a very expensive operation, as it requires a context switch from user mode to kernel mode.

strace tracks the sys calls of any instruction.
