## Part 1: Exhaustive Explanation of Concepts

Today is the culmination of Week 2. You will synthesize the physical realities of memory, files, hardware privilege, and time into a single, cohesive mental model. You will unlearn the idea that a program "does things," and internalize the fact that a program only ever _requests_ things.

### Rebuilding the Mental Model From Scratch

Imagine a completely powered-off computer.
On the SSD platter, there is a sequence of binary instructions formatted as an ELF Executable. It is a dead file (an Inode). It consumes no RAM.

When you type `./my_program` into your shell, the following architecture slams into existence:

1. **The Process Sandbox:** The kernel allocates a Task Control Block. It generates a cryptographic-like illusion called a **Virtual Address Space**. Your program is loaded into this space and genuinely believes it is the only software running on the entire machine, possessing infinite RAM starting at address `0x00000000`.
2. **The Memory Topography:** Inside this illusion, memory is meticulously zoned. The binary blueprint sits at the bottom (Text Segment). Above it is the **Heap**, waiting to grow upwards via dynamic `malloc()` allocations (which secretly trigger `brk` or `mmap` syscalls to request physical Page Frames from the hardware MMU). At the absolute top is the **Stack**, growing downwards, managing your temporary local variables and function call history.
3. **The Peripheral Blindness:** Your program is utterly blind and deaf. It cannot see the hard drive. It cannot see the monitor. To interact with the physical world, the kernel hands it an array of integers called **File Descriptors**. Integer `0` (stdin) maps to your keyboard. Integer `1` (stdout) maps to your screen.
4. **The Privilege Quarantine (Ring 3):** Your program is running in User Mode. It is mathematically forbidden from altering hardware state. If it attempts to execute a privileged instruction or dereference an unmapped memory pointer, the hardware CPU throws a fault, and the kernel instantly assassinates the process (Segmentation Fault).
5. **The Bottleneck (Syscalls):** To survive, the program must negotiate. It loads a specific integer into a CPU register and executes the `syscall` instruction. The CPU freezes the program, elevates its privilege to Ring 0 (Kernel Mode), and hands control to the OS. The OS performs the dangerous physical I/O (like reading a file block), places the result in the program's memory, lowers the privilege back to Ring 3, and resumes the program.
6. **The Relentless Interruption:** While all of this is happening, a physical hardware timer crystal is ticking. Hundreds of times a second, a timer interrupt fires, triggering the kernel's scheduler. The kernel violently rips your program off the CPU, saves its registers, and loads another process. This context switching creates the illusion of simultaneous multi-tasking.

---

## Part 2: Underlying Mechanisms & System Inspections

To prove this entire mental model is true, we will use `strace` to anatomically dissect the lifecycle of a program that simply reads a file and prints it.

**1. Tracing the Lifecycle with `strace**`Create a tiny text file:`echo "OS is magic" > target.txt`.
Run the diagnostic command: `strace cat target.txt`

**What to look for (The Exhaustive Dissection):**
You are about to read the exact conversation between User Space and Kernel Space. Read the output from top to bottom.

- `execve("/usr/bin/cat", ["cat", "target.txt"], 0x7ffd... /* 54 vars */) = 0`
- **What this is:** The shell calls `execve` to wipe its own cloned memory space and replace it with the `cat` binary. Notice it passes the environment variables (`envp`) array here.

- `brk(NULL)`
- **What this is:** The newly loaded `cat` process asks the kernel: "Where does my Heap memory currently end?"

- `mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0)`
- **What this is:** The C standard library is waking up. It needs temporary scratchpad memory. It bypasses the heap (`brk`) and asks the kernel to map two new 4KB pages (8192 bytes) of anonymous RAM directly into its Virtual Address Space.

- `openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3`
- **What this is:** The program is dynamically linking. It opens the system library cache. The kernel hands it File Descriptor `3`.

- _(...several reads and memory mappings of shared C libraries...)_
- `openat(AT_FDCWD, "target.txt", O_RDONLY) = 3`
- **What this is:** The actual payload begins. `cat` asks the kernel to open our file. The kernel resolves the Inode, checks permissions, and grants File Descriptor `3` (reusing `3` because the previous library files were closed).

- `fstat(3, {st_mode=S_IFREG|0644, st_size=12, ...}) = 0`
- **What this is:** `cat` asks the kernel for the Inode metadata. "How big is this file so I know how much buffer memory to allocate?"

- `read(3, "OS is magic\n", 131072) = 12`
- **What this is:** The data transfer. `cat` asks the kernel to pull up to 128KB of data from FD 3. The kernel pulls the 12 bytes across the Ring 0 boundary into the program's User-Space buffer.

- `write(1, "OS is magic\n", 12) = 12`
- **What this is:** `cat` takes that exact buffer and shoves it back across the Ring 0 boundary into File Descriptor `1` (`stdout`). The kernel routes it to your terminal screen.

- `close(3) = 0`
- **What this is:** Cleanup. The file is closed.

- `exit_group(0)`
- **What this is:** The process commits suicide, returning the `0` exit status to the parent shell. The kernel ruthlessly deallocates all virtual memory pages and file descriptors associated with the PID.

---

## Part 3: Code Architecture & Deliberate Breakage

To solidify this, we will write a C script that explicitly combines Memory Allocation, File Descriptors, Syscalls, and Time intervals. Then we will use `strace` to watch it break under pressure.

### The Architecture: The Systems Synthesizer

Create a file named `investigation_report.c`:

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/syscall.h>

int main() {
    printf("PID: %d\n", getpid());

    // 1. Memory (The Heap)
    // We request memory. We know this triggers a brk() or mmap() syscall under the hood.
    char *buffer = (char *)malloc(1024);
    if (!buffer) return 1;

    // 2. Time (Monotonic Clock)
    // We measure absolute execution time, immune to NTP shifts.
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // 3. Files & Syscalls (The Bottleneck)
    // We bypass the C library and talk directly to the VFS.
    int fd = open("test_payload.txt", O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd < 0) {
        perror("Failed to open file");
        return 1;
    }

    // 4. The Payload
    snprintf(buffer, 1024, "This is raw data crossing the user/kernel boundary.\n");

    // We trigger the mode switch via SYS_write
    long bytes_written = syscall(SYS_write, fd, buffer, 53);

    // 5. Deliberate Time Delay
    // We force the OS to context switch us off the CPU for 1 second.
    sleep(1);

    // 6. Cleanup & Time Calc
    close(fd);
    free(buffer);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1e9);

    printf("Successfully wrote %ld bytes. Total monotonic time: %f seconds.\n", bytes_written, elapsed);

    // ---------------------------------------------------------
    // DELIBERATE BREAKAGE SANDBOX
    // ---------------------------------------------------------

    /* --- BREAKAGE 1: FD Exhaustion via strace --- */
    // while(1) { open("/dev/null", O_RDONLY); }

    /* --- BREAKAGE 2: The Malicious Read --- */
    // syscall(SYS_read, 999, NULL, 1024);

    return 0;
}

```

### Build and Run

1. Compile the code: `gcc investigation_report.c -o investigation_report`
2. Run the program: `./investigation_report`

### Deliberate Breakage and Observation

**Breakage 1: Tracing the Invalid Syscall (The Malicious Read)**
Uncomment Breakage 2: `syscall(SYS_read, 999, NULL, 1024);`
This line is a dual-assault on the kernel. We are passing an invalid File Descriptor (`999`) AND an unmapped Virtual Memory pointer (`NULL`). Which one will the kernel catch first?

Recompile the code, but this time, **do not run it normally**. Run it through `strace` so we can see the kernel's exact response to the illegal request:
`strace ./investigation_report`

**Observe the State/Logs:**
Scroll to the very bottom of the `strace` output.
You will see your valid `write` syscall, the `sleep` syscall, and then:
`read(999, NULL, 1024) = -1 EBADF (Bad file descriptor)`

**Why exactly did this break?**
The kernel intercepted the `SYS_read` request in Ring 0. The kernel's internal logic processes arguments sequentially. It first checks the File Descriptor integer against your process's FD table. It saw that index 999 was empty. It immediately rejected the syscall with `EBADF` (Bad File Descriptor) and returned `-1` to your program.
Because the kernel aborted the operation at the FD check, it _never even evaluated_ the `NULL` pointer. The kernel is highly optimized; it fails fast on the cheapest validation checks before performing complex Virtual Memory Page Table lookups.

---

## Part 4: Record What You Learned

### What assumption is this system making?

The combination of processes, virtual memory, file descriptors, and system calls makes the absolute, foundational assumption that **user-space software is chaotic, inherently dangerous, and must be treated as a hostile entity by the hardware.**

The entire operating system architecture assumes that if a program were given direct access to physical memory addresses or hardware controllers, it would instantly corrupt the state of the machine. Therefore, the system assumes that absolute abstraction is the only path to stability. It assumes that forcing every physical interaction through the bottleneck of the CPU mode-switch (the Syscall), validated by the kernel's strictly enforced data tables (FD tables, Page Tables), is a worthy trade-off. The OS sacrifices raw physical speed in exchange for absolute systemic resilience, assuming that the illusion of control it provides to the process is indistinguishable from reality.

---

### Capstone Project: Build a Custom System Call Interceptor (`mini_strace`)

To completely master the boundary between User Space and Kernel Space, you must build a program that manipulates that boundary from the outside.

**Your Assignment:**
Write a C program that acts as a rudimentary version of `strace`. It will launch a child process and intercept its execution every single time it attempts to make a system call.

**Requirements:**

1. Your program will use the `ptrace()` system call (Process Trace). (Requires `<sys/ptrace.h>`, `<sys/wait.h>`, `<sys/user.h>`).
2. Your program must call `fork()`.
3. **Inside the Child:**

- Call `ptrace(PTRACE_TRACEME, 0, NULL, NULL)`. This tells the kernel: "I want my parent to inspect me."
- Execute a harmless binary using `execl("/bin/echo", "echo", "Hello", NULL)`.

4. **Inside the Parent:**

- The parent must enter a `while(1)` loop.
- Call `wait(&status)`. The child process will physically freeze and hand control to the parent every time it enters or exits a syscall.
- If `WIFEXITED(status)` is true, the child has finished. Break the loop.
- Use `ptrace(PTRACE_GETREGS, child_pid, NULL, &regs)` to read the child's CPU registers. (You will use `struct user_regs_struct regs;`).
- Print the value of `regs.orig_rax`. (On x86_64 Linux, `orig_rax` holds the exact integer ID of the system call the child is attempting to execute, e.g., 1 for `write`, 2 for `open`).
- Call `ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL)` to unfreeze the child and let it run until its next syscall transition.

5. **Verification:** Run your `mini_strace`. You will see a flood of integers printed to your screen (e.g., `59, 12, 9, 257...`). These are the raw Syscall IDs. Compare them to the `/usr/include/asm/unistd_64.h` file on your system to prove that your C program successfully intercepted the kernel-mode transitions of a completely separate process.

**Why this is difficult:** You are writing debugger-level software. You must orchestrate parent-child process synchronization using `wait()`, interact directly with hardware CPU registers via C structs, and manipulate the kernel's process tracing facilities. Completing this project proves you have achieved absolute mastery over the OS concepts covered in Week 2.
