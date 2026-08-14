## Part 1: Exhaustive Explanation of Concepts

To write low-level system software, you must understand how user-space applications communicate with the chaotic, diverse world of physical hardware and networking. You must understand the fundamental Unix philosophy: **"Everything is a file."**

### File Descriptors: The Ultimate Abstraction

**The Problem it Solves:**
Imagine writing a C program that needs to output a string of text. Where does that text go? It might go to a spinning magnetic hard drive, an NVMe solid-state drive, a user's terminal emulator monitor, a memory-based pipe to another program, or across the Atlantic Ocean via a TCP network socket. If the programmer had to write custom hardware drivers to interface with each of these distinct physical mediums, software development would grind to a halt.

**The Abstraction:**
The Operating System isolates the user-space process from the physical hardware using an abstraction called a **File Descriptor (FD)**.
A File Descriptor is nothing more than a non-negative integer (0, 1, 2, 3...). It is not a memory pointer; it is a superficial index number. When your program asks the OS to open a file, create a network connection, or build a pipe, the OS does all the complex hardware initialization in the kernel space. It then hands your process a simple integer (like `4`) and says, "Whenever you want to read or write to that resource, just hand me the number 4, and I will handle the physical routing."

To make this work, the kernel maintains three distinct tables:

1. **The File Descriptor Table (Per-Process):** Every process has its own private array. The integer (FD) is just the index of this array. The array stores pointers to the next table.
2. **The Open File Table (System-Wide):** This table tracks every open resource across the entire OS. It holds the current "offset" (where you are currently reading in the file) and the access mode (Read-Only, Write-Only). Multiple FDs from different processes can point to the exact same entry here.
3. **The Inode Table (System-Wide):** This points to the actual physical data blocks on the hard drive or the underlying driver for the hardware device.

### stdin, stdout, and stderr as Descriptors

When the kernel boots a new process, it does not give it an empty File Descriptor table. By absolute POSIX convention, the first three slots (indices 0, 1, and 2) are automatically pre-populated.

- **FD 0 (Standard Input - `stdin`):** The default stream for reading incoming data.
- **FD 1 (Standard Output - `stdout`):** The default stream for writing normal data payload.
- **FD 2 (Standard Error - `stderr`):** The default stream for writing diagnostic, debugging, or error messages.

By default, when you launch a program from your terminal, the OS wires all three of these descriptors to point to the exact same character device: your terminal window's pseudo-teletype (`/dev/pts/X`). When a C program calls `printf("Hello");`, it is actually calling a wrapper for the `write()` system call, hardcoded to send bytes to File Descriptor 1.

### Files, Sockets, and Pipes as OS Resources

Because the system only speaks in integers (FDs), the concept of a "file" is stretched to encompass almost all OS resources:

- **Regular Files:** When you `open("/tmp/data.txt")`, the OS allocates an FD that routes through the Virtual File System (VFS) to the physical disk.
- **Pipes:** A pipe is an anonymous, unidirectional ring-buffer existing entirely in physical RAM. When a process calls the `pipe()` system call, the OS creates this memory buffer and returns exactly two File Descriptors: one strictly for reading the buffer, and one strictly for writing to it.
- **Sockets:** A socket is a bi-directional network endpoint. When you call `socket()`, the OS allocates a network buffer and binds it to the TCP/IP stack. It hands your program an FD. When you `write()` to this FD, the OS kernel intercepts the bytes, packages them into TCP segments, encapsulates them in IP packets, and sends them out the Ethernet port. To your user-space program, it feels exactly identical to writing to a text file.

---

## Part 2: Underlying Mechanisms & System Inspections

To physically prove that everything is an integer mapping to a resource, we will interrogate the Linux kernel's virtual `/proc` filesystem.

**1. Inspecting the Default Standard Streams**
Open your terminal. We will start a process that simply sleeps in the background so we can inspect it while it is alive.
Run: `sleep 1000 &`
The terminal will print a Job ID and a Process ID (PID). Let's assume the PID is `4567`.
Run: `ls -l /proc/4567/fd`

**What to look for:**
You will see exactly three symbolic links:
`0 -> /dev/pts/0`
`1 -> /dev/pts/0`
`2 -> /dev/pts/0`
This proves that standard input, output, and error are just integers mapped to your specific terminal window device (`/dev/pts/0`).

**2. Proving I/O Redirection Hijacks the FD Table**
Kill the sleep process: `kill 4567`
Now, launch it again, but this time use shell redirection to send its output to a file and its errors to the void.
Run: `sleep 1000 > /tmp/output.log 2> /dev/null &`
Find its new PID (e.g., `4568`).
Run: `ls -l /proc/4568/fd`

**What to look for:**
`0 -> /dev/pts/0`
`1 -> /tmp/output.log`
`2 -> /dev/null`
The `sleep` binary itself did absolutely nothing different. Before the `sleep` program even started executing, the Bash shell hijacked its File Descriptor table, overwriting index 1 to point to a file, and index 2 to point to the kernel's black hole.

**3. Inspecting Sockets and Pipes (`lsof`)**
We will now use the `lsof` (List Open Files) command to view more exotic resources.
Run a command that establishes a network connection and holds a pipe open:
`nc -l 8080 | grep "payload" &`
Find the PID of the `nc` (netcat) process.
Run: `lsof -p <PID>`

**What to look for:**
Look at the `FD` and `TYPE` columns.

- You will see an FD identified as `IPv4` with a type of `sock` (Socket), showing it is listening on port 8080.
- You will see FD `1` (stdout) identified as `FIFO` (First-In, First-Out). This is the anonymous RAM pipe connecting `nc` to `grep`. It is not a file on the disk; it is a raw memory resource.

---

## Part 3: Code Architecture & Deliberate Breakage

To understand the immense power of File Descriptors, we will write a C program that manually executes the exact same hijacking the Bash shell performs during redirection. We will use the `dup2()` system call to violently overwrite our own `stdout`.

### The Architecture: Manual FD Hijacking

Create a file named `fd_manipulator.c`:

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    printf("Step 1: This text is going to FD 1, which currently points to the terminal.\n");

    // Open a regular file on the disk. The OS will give us the lowest available FD (likely 3).
    // Flags: O_WRONLY (Write only), O_CREAT (Create if missing), O_TRUNC (Clear it if it exists).
    // 0644 are the file permissions (rw-r--r--).
    int target_fd = open("/tmp/hijacked_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (target_fd < 0) {
        perror("Failed to open file");
        return 1;
    }

    printf("Step 2: Opened file. The OS gave us File Descriptor: %d\n", target_fd);

    // The Hijack: dup2(oldfd, newfd)
    // This tells the kernel: "Take whatever resource 'target_fd' is pointing to,
    // and forcefully make FD 1 (stdout) point to that exact same resource."
    if (dup2(target_fd, STDOUT_FILENO) < 0) {
        perror("Failed to duplicate file descriptor");
        return 1;
    }

    // Now that FD 1 points to the file, we no longer need the original FD 3 pointer.
    close(target_fd);

    // Step 3: Proving the Hijack
    // printf() is hardcoded to write to FD 1. It has no idea we changed the plumbing.
    printf("Step 3: This text is printed using printf(), but it will NEVER appear on your screen.\n");
    printf("It has been secretly routed to /tmp/hijacked_output.txt.\n");

    // ---------------------------------------------------------
    // DELIBERATE BREAKAGE SANDBOX
    // Uncomment one of the breakages below to observe system failure.
    // ---------------------------------------------------------

    /* --- BREAKAGE 1: Resource Exhaustion (FD Leak) --- */
    // printf("Initiating Resource Exhaustion...\n");
    // int count = 0;
    // while (1) {
    //     int fd = open("/dev/null", O_RDONLY);
    //     if (fd < 0) {
    //         // We use fprintf to stderr (FD 2) because we broke stdout earlier!
    //         fprintf(stderr, "\nCRASH: The OS refused to give us more FDs!\n");
    //         fprintf(stderr, "Total FDs opened before failure: %d\n", count);
    //         perror("Kernel Reason");
    //         break;
    //     }
    //     count++;
    // }

    /* --- BREAKAGE 2: Writing to a Closed / Invalid FD --- */
    // int bad_fd = 999;
    // // FD 999 doesn't exist in our table.
    // ssize_t bytes_written = write(bad_fd, "Hello", 5);
    // if (bytes_written < 0) {
    //     fprintf(stderr, "\nCRASH: Attempted to write to FD %d\n", bad_fd);
    //     perror("Kernel Reason");
    // }

    return 0;
}

```

### Build and Run

1. Compile the code: `gcc fd_manipulator.c -o fd_manipulator`
2. Run the program: `./fd_manipulator`
3. Observe the terminal output. You will only see Steps 1 and 2.
4. Run: `cat /tmp/hijacked_output.txt`. You will see Step 3 physically written to the disk.

### Deliberate Breakage and Observation

**Breakage 1: Resource Exhaustion (`RLIMIT_NOFILE`)**
Uncomment the `while(1)` loop in the C code under Breakage 1. Recompile and run.
**Observe the Logs:** The program will loop violently, opening `/dev/null` over and over, collecting FDs `4, 5, 6...` until it suddenly crashes with:
`CRASH: The OS refused to give us more FDs!`
`Total FDs opened before failure: 1021`
`Kernel Reason: Too many open files`

**Why exactly did this break?** The OS allocates a finite amount of kernel memory for the File Descriptor Table of each process. To prevent a malicious program from consuming all kernel memory by opening millions of files, the OS enforces a strict "ulimit" (usually 1024 FDs per process). Because we intentionally forgot to call `close(fd)` inside the loop (an FD Leak), we rapidly exhausted our quota. The `open()` system call returned `-1`, and the `errno` was set to `EMFILE` (Too many open files).

**Breakage 2: The EBADF Error (Bad File Descriptor)**
Comment out Breakage 1, and uncomment Breakage 2. Recompile and run.
**Observe the Logs:**
`CRASH: Attempted to write to FD 999`
`Kernel Reason: Bad file descriptor`

**Why exactly did this break?** We asked the kernel to write data to index `999` in our File Descriptor Table. The kernel checked our table, saw that slot `999` was completely empty (a NULL pointer), and immediately rejected the system call, returning `-1` and setting `errno` to `EBADF`. This proves that user-space programs cannot force physical I/O; they must humbly request it using valid integer tokens previously granted by the kernel.

---

## Part 4: Record What You Learned

### What assumption is this system making?

The File Descriptor architecture makes the absolute, foundational assumption that **user-space memory is infinitely dangerous, and processes must never be trusted with actual memory pointers to hardware resources.**

By forcing processes to use superficial integers (FDs) instead of raw memory addresses, the Operating System assumes strict architectural supremacy. The OS assumes that if a process goes rogue and tries to randomly corrupt memory, it can only corrupt the integer `4`, not the physical kernel data structure representing the hard drive. Furthermore, the system assumes that **all data flows can be reduced to a 1-dimensional array of bytes**. Whether the destination is a GPU, a network socket, or a text file, the OS assumes that implementing a universal `read()` and `write()` interface against a File Descriptor is sufficient to handle the infinite complexity of modern hardware.

---

Your Assignment:
Write a C program that manually constructs the file descriptor routing for the equivalent of this complex shell command:
grep "ERROR" < input.txt | sort -r > output.txt 2> error_logs.txt

Requirements:

    File Preparation: Before writing the C code, use your terminal to create a file named input.txt containing 50 lines of text, where roughly 10 lines contain the word "ERROR".

    Resource Allocation: In your C program, you must use the open() system call (requiring <fcntl.h>) to acquire three distinct File Descriptors:

        fd_in: Open input.txt as Read-Only (O_RDONLY).

        fd_out: Open output.txt as Write-Only, create it if missing, and truncate it to zero if it exists (O_WRONLY | O_CREAT | O_TRUNC, with 0644 permissions).

        fd_err: Open error_logs.txt with the same write/create flags as above.

    The Memory Buffer: Use the pipe(pipefd) system call to create a 2-integer array representing the read and write ends of an anonymous RAM pipe.

    Process A (The Filter): Call fork() to create your first child process. Inside this child's code block:

        Use dup2() to overwrite STDIN_FILENO (0) so it points to fd_in.

        Use dup2() to overwrite STDOUT_FILENO (1) so it points to the write-end of your pipe (pipefd[1]).

        Use dup2() to overwrite STDERR_FILENO (2) so it points to fd_err.

        Crucial Step: You must explicitly close() every single raw FD (fd_in, fd_out, fd_err, pipefd[0], pipefd[1]) because they have already been duplicated into the standard 0, 1, and 2 slots.

        Use execlp("grep", "grep", "ERROR", NULL) to replace the process with the grep binary.

    Process B (The Sorter): From the parent process, call fork() a second time to create the next child. Inside this child's code block:

        Use dup2() to overwrite STDIN_FILENO (0) so it points to the read-end of your pipe (pipefd[0]).

        Use dup2() to overwrite STDOUT_FILENO (1) so it points to fd_out.

        Use dup2() to overwrite STDERR_FILENO (2) so it points to fd_err.

        Crucial Step: Close all raw FDs just as you did in Process A.

        Use execlp("sort", "sort", "-r", NULL) to replace the process.

    The Parent Process (The Orchestrator):

        The parent process is not executing any binaries, but it does hold copies of all the FDs it opened and the pipe ends it created. You must write the code for the parent to close() all 5 of these FDs (fd_in, fd_out, fd_err, pipefd[0], pipefd[1]).

        Finally, the parent must call wait(NULL) twice to reap the zombie states of both children.

Why this is difficult:
You are managing seven independent file descriptors simultaneously across three isolated virtual memory spaces. The architecture of a pipe mandates that an End-Of-File (EOF) signal is only sent to the reader when every single write-end of that pipe across the entire operating system is closed.
