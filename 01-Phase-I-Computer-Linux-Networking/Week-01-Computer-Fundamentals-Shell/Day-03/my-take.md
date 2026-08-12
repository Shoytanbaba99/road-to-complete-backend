## 💡 Key Takeaways

A process is a heavyweight container, containing memory space, program counter, descriptors and other resources. A thread is a lightweight container, sharing the same memory space and resources like Data segment, heap, and code segment. Threads allow multiple tasks to run concurrently within the same process.

the fork() command shares and duplicated the entire process, with its virtual memory space, while the clone() command allows for more fine-grained control over what is shared between the parent and child processes. The clone() command can be used to create threads that share certain resources, such as memory space, while still allowing for separate execution contexts.

Context switching is the process of saving the state of currently running process or thread and restoring hte state of another thread and continuing to finish it before going back to the original thread. Context switching has high overhead and has no benefit to the application.

modern linux uses, Completely fair scheduler (CFS) to schedule threads and processes. where each thread is assigned a virtual runtime and the one with hte lowest virtual runtime is scheduled to run next.

when dealing with multiple threading, and context switching, the user is totally responsible for the management of memory and resources, and the user must ensure that the threads do not interfere with each other. This can lead to issues such as race conditions. as shown in the race_condition.c example, where two threads are trying to increment the same variable without proper synchronization, leading to unexpected results.

## ❓ Remaining Questions / Areas to Explore
