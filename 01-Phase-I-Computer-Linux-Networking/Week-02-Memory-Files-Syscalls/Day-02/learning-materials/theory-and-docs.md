## Part 1: Exhaustive Explanation of Concepts

To write low-level, high-performance software, you must entirely abandon the concept that the memory addresses your C or C++ programs use are actual, physical locations inside your RAM hardware. The reality of modern computing is that every process operates in a state of absolute, meticulously engineered deception. This deception is known as Virtual Memory.

### Virtual Memory and Virtual Addresses

* **The Problem it Solves:** Before Virtual Memory, programs were loaded directly into physical RAM. If Program A was loaded at physical address `0x1000` and Program B was loaded at `0x5000`, severe problems arose. First, Program A could accidentally (or maliciously) calculate an address of `0x5000` and overwrite Program B's memory, causing a system-wide crash. Second, fragmentation was rampant; if a program needed a contiguous 10MB block of RAM, but the physical RAM was fragmented into 2MB chunks, the program could not run, even if the total free memory was 100MB. Finally, programs were strictly limited by the physical size of the RAM chips installed on the motherboard.
* **The Abstraction:** The Operating System, in hardwired collaboration with the CPU's **Memory Management Unit (MMU)**, provides the abstraction of **Virtual Memory**.
* Every single process is handed a pristine, contiguous, private, and massive "Virtual Address Space" (on 64-bit systems, this space is astronomically huge, effectively infinite for practical purposes).
* When your C program prints a pointer address like `0x7ffc82b2000`, this is a **Virtual Address**. It is a fake number. It is a cryptographic-like token that the MMU intercepts on the fly, millions of times a second, and mathematically translates into a completely different, hidden Physical Address where the electrical charge is actually stored on the silicon.
* **Why every process thinks it has its own memory:** Because every process starts its virtual address space at `0x0000000000000000`. Process A and Process B can both write data to virtual address `0x400000`. The MMU transparently maps Process A's `0x400000` to physical RAM address `0x1A000`, and maps Process B's `0x400000` to physical RAM address `0x8F000`. They share the same fake addresses, but their physical realities are completely isolated.



### Pages, Page Tables, and the MMU

* **The Problem it Solves:** If the OS had to track the translation of every single solitary byte (mapping virtual byte 1 to physical byte 9, virtual byte 2 to physical byte 73), the translation map would be exactly the same size as the RAM itself, leaving zero space for actual programs.
* **The Abstraction:** Memory is chunked into **Pages**.
* Both Virtual Memory and Physical RAM are divided into fixed-size contiguous blocks called Pages (typically 4096 bytes, or 4KB).
* The translation mapping does not happen byte-by-byte; it happens Page-by-Page. Virtual Page 1 maps to Physical Page Frame 45. Virtual Page 2 maps to Physical Page Frame 12.
* This mapping is stored in RAM in a highly optimized data structure called a **Page Table**. Every process has its own distinct Page Table.
* When the CPU tries to execute an instruction at a virtual address, the MMU splits the address into two parts: the Virtual Page Number (VPN) and the Offset. It looks up the VPN in the Page Table to find the physical Page Frame Number (PFN), and then applies the exact same Offset to find the specific byte within that 4KB physical page.



### Page Faults (The Greatest Illusion of All)

* **The Problem it Solves:** Your computer might have 16GB of physical RAM, but you might have 50 Chrome tabs, a video editor, and a virtual machine open, requesting a total of 30GB of memory. How does the system survive?
* **The Abstraction:** The Page Table does not just hold physical addresses; it holds a metadata bit called the **Valid/Invalid Bit** (or Present Bit).
* When your process asks for 1GB of memory via `malloc()`, the OS does *not* give you 1GB of physical RAM. It updates your Page Table with 1GB worth of Virtual Pages, but leaves the physical mapping empty, setting the Present Bit to `0` (Invalid).
* When your program finally attempts to read or write to one of those virtual pages, the MMU checks the Page Table, sees the Present Bit is `0`, and immediately halts the CPU. It throws a hardware exception called a **Page Fault**.
* The OS kernel intercepts this Page Fault. The kernel pauses your program, goes out to the physical RAM, finds an empty 4KB Page Frame (or evicts an old, unused page to the hard drive swap file), updates your Page Table with the new physical address, sets the Present Bit to `1`, and tells the CPU to retry the exact instruction that failed.
* Your program has absolutely no idea it was paused, no idea a hardware fault occurred, and no idea its data was just loaded from the hard drive. This is known as **Demand Paging**.



---

## Part 2: Underlying Mechanisms & System Inspections

To prove that the OS uses demand paging and to witness page faults happening in real-time, we will interrogate the Linux kernel.

**1. Inspecting Page Size**
Run the command: `getconf PAGE_SIZE`

* **Observation:** The terminal will output `4096`. This physically proves that your OS manages memory in strict 4 Kilobyte chunks.

**2. Observing Page Faults in Real-Time (`ps` and `sar`)**
We want to prove that processes are constantly generating page faults as they demand memory.
Run the command: `ps -eo pid,comm,min_flt,maj_flt --sort=-min_flt | head -n 10`

* **What to look for:**
* `min_flt` (Minor Page Faults): The process accessed a virtual page that was not currently mapped, but the OS was able to fix the fault by assigning a physical RAM page instantly.
* `maj_flt` (Major Page Faults): The process accessed a virtual page, but the data was not in RAM at all; the OS had to suspend the process for a massive amount of time to physically read the 4KB chunk from the slow SSD/Hard Drive (Swap space or a memory-mapped file).



**3. Inspecting the Page Table of a Live Process (`/proc`)**
Run the command: `sleep 1000 &`
Find its PID using `ps aux | grep sleep`. Let's assume the PID is `8888`.
Run the command: `cat /proc/8888/smaps | head -n 25`

* **What to look for:** This file details the exact Page Table allocations for this process.
* Look at the `Size:` vs `Rss:` (Resident Set Size).
* `Size` is the amount of Virtual Memory the process *thinks* it has.
* `Rss` is the amount of Physical Memory the OS has *actually* given it. If `Size` is 2048 kB and `Rss` is 64 kB, you have mathematical proof of Demand Paging. The OS is lying to the process, saving 1984 kB of physical RAM until the process actually triggers a page fault to demand it.



---

## Part 3: Code Architecture & Deliberate Breakage

To witness the physical reality of Virtual Memory mapping and the ruthlessness of the Page Fault mechanism, we will write a C program that requests a massive amount of memory, proves that physical RAM is not immediately consumed, and then deliberately triggers faults.

### The Architecture: Demand Paging Sandbox

Create a file named `virtual_memory.c`:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    // 1 Gigabyte of memory
    size_t gb = 1024 * 1024 * 1024; 
    
    printf("PID: %d\n", getpid());
    printf("Step 1: About to request 1GB of Virtual Memory.\n");
    printf("Check 'top' in another terminal. Press Enter to continue...\n");
    getchar();

    // The OS updates the Page Table, setting the Valid bits to 0 (Invalid).
    // It allocates ALMOST ZERO physical RAM.
    char *massive_array = (char *)malloc(gb);
    
    if (massive_array == NULL) {
        printf("Malloc failed!\n");
        return 1;
    }

    printf("Step 2: 1GB Virtual Memory allocated at Virtual Address: %p\n", (void*)massive_array);
    printf("Look at 'top' again. The 'VIRT' column increased by 1G, but 'RES' (Physical RAM) barely moved.\n");
    printf("Press Enter to begin forcing Page Faults...\n");
    getchar();

    // Step 3: Triggering Minor Page Faults
    // We are going to write 1 byte into every 4KB page.
    // This forces the MMU to throw a Page Fault on every loop iteration, 
    // forcing the OS to find a physical 4KB frame and map it.
    size_t page_size = 4096;
    for (size_t i = 0; i < gb; i += page_size) {
        massive_array[i] = 'A';
    }

    printf("Step 3: Finished triggering 262,144 Page Faults.\n");
    printf("Look at 'top' now. 'RES' has spiked to 1GB because we forced physical allocation.\n");
    
    printf("\n=== INITIATING DELIBERATE BREAKAGE ===\n");
    printf("Press Enter to attempt an illegal memory access...\n");
    getchar();

    // Deliberate Breakage: Bypassing the valid Virtual Address Space
    // We take our valid pointer, and jump 1 byte past the 1GB allocation.
    // Because this Virtual Page was never allocated in our Page Table, the OS
    // has no mapping for it.
    char *illegal_address = massive_array + gb + 1;
    
    printf("Attempting to write to unmapped Virtual Address: %p\n", (void*)illegal_address);
    
    // The MMU looks up the VPN, sees the Page Table has no entry, throws a Page Fault.
    // The OS kernel catches the fault, realizes we never called malloc() for this page, 
    // and brutally murders the process.
    *illegal_address = 'X'; 
    
    // We will never reach this line.
    printf("Successfully wrote to illegal address!\n");

    free(massive_array);
    return 0;
}

```

### Build and Run

1. Compile the code: `gcc virtual_memory.c -o virtual_memory`
2. Open a second terminal window and run: `top -p $(pidof virtual_memory)` (You will have to run `top` and filter by the PID manually once the program prints its PID).
3. Run the program in the first terminal: `./virtual_memory`

### Deliberate Breakage and Observation

**Breakage 1: Observing the Demand Paging Illusion**
Follow the prompts in the script. When you hit Step 2, look at your `top` output.

* **VIRT (Virtual Image):** Will read roughly `1.0g`. Your process mathematically holds 1 Gigabyte of virtual address space.
* **RES (Resident Size):** Will read something tiny, like `1200` (1.2MB).
* **Why?** You called `malloc()`, but the OS lied to you. It gave you fake addresses and no physical silicon.
* When you hit Step 3, the `for` loop executes. It skips 4096 bytes at a time, deliberately touching exactly one byte per page. You will watch the `RES` column in `top` explode up to `1.0g`. You just forced the OS to physically resolve a quarter of a million Page Faults, halting your program, mapping RAM, and resuming it, over and over again.

**Breakage 2: The Segmentation Fault (Invalid Page Fault)**
When you press Enter to initiate the deliberate breakage, the program attempts to write to `massive_array + gb + 1`.
**Observe the Logs:** The program immediately crashes with `Segmentation fault (core dumped)`.
**Why exactly did this break?** Not all Page Faults are resolved successfully. When the MMU generated a Page Fault for that specific Virtual Address, the OS kernel stepped in and checked its master record of your allocations. It saw that you never requested the Virtual Page containing that address. Because you accessed an unmapped page, the OS determined you were a rogue process performing an illegal operation, and it terminated you via the `SIGSEGV` signal. A Segmentation Fault is simply a Page Fault that the OS refuses to fix.

---

## Part 4: Record What You Learned

### What assumption is this system making?

The Virtual Memory architecture makes the massive, fundamental assumption that **programs rarely need all the memory they ask for, and they exhibit strong Spatial and Temporal Locality.**

The OS assumes that when a program asks for 1GB of memory, it won't actually use it all at once, allowing the OS to radically overcommit physical RAM (selling the same physical RAM to multiple processes, assuming they won't all demand it simultaneously). It assumes **Spatial Locality**: if a program accesses byte `0x1000`, it is highly likely to access byte `0x1001` very soon. This is why memory is mapped in 4096-byte Pages; the OS pays the heavy context-switching cost of a Page Fault once, brings in an entire 4KB block, and assumes the next several thousand memory accesses will hit that same block without requiring kernel intervention. If a programmer writes an algorithm that randomly accesses memory addresses miles apart from each other (defeating spatial locality), they will trigger a continuous storm of Page Faults (thrashing), slowing the program down by a factor of 10,000x as the OS desperately tries to swap pages in and out of the physical RAM.

---

### Capstone Project: Build a User-Space Memory Allocator (A mini `malloc`)

To deeply internalize how virtual addresses are requested from the kernel and managed in user-space, you must bypass the standard C library and interact directly with the kernel's memory boundary.

**Your Assignment:**
Write a C program that implements your own custom version of `malloc()` and `free()` using the `sbrk()` system call.

**Requirements:**

1. **Do not use `#include <stdlib.h>`.** You are forbidden from using the standard `malloc()`, `calloc()`, or `free()`.
2. Your program must include `<unistd.h>` to access the `sbrk()` system call. `sbrk(increment)` asks the kernel to physically move the upper boundary of your Virtual Memory Heap by `increment` bytes.
3. Write a function `void* custom_malloc(size_t size)`.
* Inside this function, you must define a struct to hold metadata (e.g., `struct block_meta { size_t size; int free; struct block_meta *next; };`).
* You must call `sbrk(size + sizeof(struct block_meta))` to request memory directly from the kernel.
* You must populate the metadata at the very beginning of the returned block, and then return a pointer that points *just after* the metadata (so the user doesn't overwrite your management data).


4. Write a `void custom_free(void *ptr)` function.
* This function must take the user's pointer, subtract `sizeof(struct block_meta)` to find the hidden metadata header, and set the `free` flag to `1`.


5. In your `main()` function, write a test suite:
* Allocate 3 different arrays using `custom_malloc()`.
* Write strings into them to prove the virtual addresses are valid and backed by physical memory.
* Call `custom_free()` on the middle array.
* Print the addresses returned by `custom_malloc()` to physically prove that your allocator is shifting the heap boundary.



**Why this is difficult:** You are building the foundational infrastructure of memory management. You must manage a linked list of metadata headers completely hidden from the user, embedded directly in the raw memory blocks you receive from the kernel. If your pointer arithmetic is wrong by even one byte when hiding or revealing the metadata header, the user will overwrite your linked list pointers, causing your custom allocator to catastrophically corrupt its own memory mapping on the next allocation attempt. Completing this proves you understand that virtual memory is just a massive, empty canvas provided by the OS, and it is entirely up to user-space logic to organize it.
