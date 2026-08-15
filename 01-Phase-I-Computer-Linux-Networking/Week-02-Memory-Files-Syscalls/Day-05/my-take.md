## 💡 Key Takeaways

4 commands to access and speak to kernel are, open(), read(), write(), close().

fd = open(file, flags, mode) - open a file and return a file descriptor (fd) for it.
read(fd, buffer, count) - read data from a file descriptor into a buffer.
write(fd, buffer, count) - write data from a buffer to a file descriptor.
close(fd) - close a file descriptor.

there are three different data strcture working, file descriptor table of the process alllows us to talk to the kernel aobut the file, open file table of the kernel keeps track of all open files in the system, and even the offset. And, the inode table of the kernel keeps track of all the files in the system, their metadata, and their location on disk(header).

When weare saving, it doesnt instaly calls a sys call to save data or things on ssd or hdd, it goes through some distinct layers of caching, and buffering. User-Space Buffering, Kernel-Space Buffering, and Disk Caching. The data is first written to the user-space buffer, then to the kernel-space buffer, and finally to the disk cache before being written to the actual storage device.

commands like fprintf() only saves to user space buffer, untill there isa line end, when it sends a write syscall to the kernel, then in hte kernel it still doesnt get saved, it is tagged as dirty and at its convenient time saves to the disks.

So, essentially, When userspace buffer occurs, and the process dies we lose the data, when kernel space buffer occurs, and the process dies we still have the data in the kernel space buffer, but if the system crashes we lose the data(power cord unplug).
