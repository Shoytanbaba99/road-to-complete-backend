# My Take & Synthesis

> **Goal:** Write down your own mental model, key insights, and personal understanding after studying the day raw materials.

## 💡 Key Takeaways

Source code and Executable code is different in the sense t hat src code is the human readable code that we write in programming languages like Python, Java, etc. It is the code that we write and understand as developers. Executable code, on the other hand, is the machine-readable code that is generated after the source code is compiled or interpreted. It is the code that can be executed by a computer to perform specific tasks.

Process and Program, Program is a set of instruction with its own logic, library, heap, stack, and other resources. A process is an instance of a program that is currently being executed by the operating system. A process has its own memory space, file descriptors, and other system resources.

as there are multiple processes running on a system, they must share the same RAM, they might conflict with each other if they try to access the same memory location at the same time. This is where the concept of process isolation comes into play. Each process has its own virtual address space which managed by MMU, which is mapped to physical memory by the operating system. This ensures that processes do not interfere with each other's memory and can run independently.

operating system runs on multiple processes, but all these processes are child of the same process for instance, in the case of linux it would be systemd, which is the first process that is started when the system boots up. All other processes are spawned by systemd and are its children.

every process spawns with a PID and PPID. PID is the process identifier, and PPID is the parent process identifier.

you can view the status of the processes at /proc, which is a virtual filesystem that provides information about the system and its processes. Each process has its own directory under /proc with its PID as the name. Inside each process directory, you can find various files that provide information about the process, such as its status, memory usage, open file descriptors, and more.

## 🔬 Practical Lab Findings

readelf -h /bin/ls reveals i guess, that linux executable files are in ELF format, which is a common standard for executable files, object code, shared libraries, and core dumps. The output of readelf -h shows the ELF header information, including the file type, architecture, entry point address, and other details.

/proc/[pid]/maps provides information about the memory mapping that occurs with virtual memory. r-xp(text segment), r-w with data/heap and stack segment. Essentially heap contains teh global variables and stack contains the local variables.

New processes are created using the fork() system call, which create a new process by duplicating the calling process. The new process is called child process, it gets its own pid. the returning value of fork() is 0 for the child process and the pid of the child process for the parent process. After a fork(), both the parent and child processes continue executing from the point where the fork() was called. When the child has ended its execution, it terminates but the parent must use wait() system call to wait for the child process to finish and collect its exit status. If the parent does not wait for the child, the child process becomes a zombie process, which is a process that has completed execution but still has an entry in the process table.
