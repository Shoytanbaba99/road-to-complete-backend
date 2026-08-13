## Part 1: Exhaustive Explanation of Concepts

To understand a Unix-like operating system, you must internalize its most famous, unified abstraction: **"Everything is a file"** (or more accurately, a file descriptor). To organize this universe of files, the kernel relies on a strict, globally unified namespace and a cryptographic-like permission matrix dictating exactly who can read, modify, or execute data.

### The Filesystem Hierarchy and the VFS

- **The Problem it Solves:** A computer has multiple physical storage devices (NVMe drives, SATA SSDs, USB sticks) and virtual data interfaces (kernel parameters, random number generators, device drivers). If a program wants to read data, having to specify `Drive C:\` vs `Drive D:\` or writing custom code to talk to a hardware driver creates massive friction.
- **The Abstraction:** The **Virtual File System (VFS)** and the **Filesystem Hierarchy Standard (FHS)**.
- The OS provides a single, unified, inverted tree structure starting at the absolute root: `/`.
- Physical drives are not accessed by drive letters. Instead, they are "mounted" (grafted) onto arbitrary directories (mount points) within this single tree. When a user navigates to `/home/user/data`, the kernel silently translates that path across potentially multiple different physical hard drives.
- **Key FHS Directories:**
- `/bin` and `/usr/bin`: Essential user binaries (executables like `ls`, `cat`).
- `/sbin`: System binaries (commands requiring root, like `fdisk`, `iptables`).
- `/etc`: Host-specific system-wide configuration files (pure text, no executables).
- `/var`: Variable data. Files whose sizes grow and shrink dynamically (logs in `/var/log`, databases, mail spools).
- `/dev`: Device nodes. These are not real files on a disk. They are magical pointers directly to the kernel's hardware drivers. `/dev/sda` is your raw hard drive. `/dev/null` is a black hole.
- `/proc` and `/sys`: Virtual filesystems mapped directly into RAM. They contain live kernel data structures pretending to be text files.

### Inodes, Dentries, and Links

Before understanding permissions or links, you must understand how the filesystem physically stores data. Humans use filenames (`report.pdf`), but the kernel and hard drive know absolutely nothing about filenames.

- **The Inode (Index Node):** This is the true identity of a file. An inode is a data structure on the disk containing all the metadata about a file: its size, its owner, its permissions, timestamps, and crucially, the array of physical disk block addresses where the actual file data lives. **An inode does not contain the filename.** Every inode has a unique integer ID.
- **The Dentry (Directory Entry):** A directory is simply a special type of file whose data payload is a tabular list mapping human-readable strings (filenames) to Inode numbers.

**Symbolic vs Hard Links**

- **The Problem it Solves:** We need a file to exist in multiple logical locations in the tree simultaneously, without duplicating the massive data payload on the physical disk.
- **Hard Links:** A hard link is simply a second, third, or fourth Dentry (filename) that points to the exact same Inode number.
- Because they point directly to the inode, all hard links are absolute peers. There is no "original" file and "shortcut".
- The inode maintains a `link count` (an integer). If you delete a filename, the OS simply removes the Dentry and decrements the inode's link count. The actual data on the disk is only destroyed when the link count reaches exactly `0`.
- _Constraint:_ Hard links cannot cross filesystem boundaries (you cannot hard link a file on an SSD to a file on a USB drive) because Inode numbers are only guaranteed to be unique within a single filesystem. Furthermore, you cannot hard link a directory (to prevent infinite recursive loops in the tree).

- **Symbolic Links (Soft Links):** A symlink is a completely distinct, brand-new file with its own unique Inode. The data blocks of this new Inode do not contain user data; they contain a literal string representing the absolute or relative path to another filename.
- If you delete the target file, the symlink remains, but it becomes "broken" or "dangling" (pointing to a path that no longer exists).
- Because they just contain text paths, symlinks _can_ cross filesystems and _can_ point to directories.

### Ownership, Permissions, and chmod/chown

- **The Problem it Solves:** In a multi-user environment (derived from 1970s mainframe Unix), users must be prevented from reading or destroying each other's data, and malicious users must be prevented from modifying system binaries.
- **The Abstraction:** Every single Inode contains exactly two ownership IDs: a **UID (User ID)** and a **GID (Group ID)**. Alongside ownership, the Inode contains a 9-bit permission matrix.

The matrix is divided into three groups of three bits: **User (Owner)**, **Group**, and **Others (World)**.
Each triad defines **Read (r)**, **Write (w)**, and **Execute (x)**.

- **For standard files:**
- `Read`: Can view the file's contents (e.g., `cat`, `less`).
- `Write`: Can modify the file's data blocks.
- `Execute`: Can ask the kernel to load the file into RAM and run it as a process.

- **For directories (Crucial Distinction):**
- `Read`: Can read the Dentry table (can run `ls` to see the list of filenames).
- `Write`: Can modify the Dentry table (can create new files, rename files, or delete files inside the directory).
- `Execute`: **Traversal.** You can `cd` into the directory, or access a known file inside it. If you do not have execute permission on a directory, it completely blocks you from accessing _anything_ beneath it, regardless of the permissions of the child files.

**Octal Representation (`chmod`)**
Because permissions are a 9-bit binary string (e.g., `111 101 100`), they are perfectly represented in Base-8 (Octal) mathematics.

- `Read (r)` = binary `100` = 4
- `Write (w)` = binary `010` = 2
- `Execute (x)` = binary `001` = 1

Thus, `chmod 754 file.txt` mathematically applies:

- User: 4 + 2 + 1 = 7 (rwx)
- Group: 4 + 0 + 1 = 5 (r-x)
- Other: 4 + 0 + 0 = 4 (r--)

---

## Part 2: Underlying Mechanisms & System Inspections

To prove these abstractions are physical reality, we will directly interrogate the kernel's Virtual Filesystem layer and Inode tables.

**1. Inspecting the VFS Abstraction (`findmnt` and `df`)**
Run the command: `findmnt` or `df -h`

- **What to look for:** You will see a list of mounted filesystems. Notice the `TARGET` column. Your physical `/dev/nvme0n1p2` might be mounted at `/`. But you will also see `/sys` (sysfs) and `/proc` (procfs). These have no physical block device backing them; they exist purely in the kernel's RAM but are grafted into the tree so you can interact with them using standard file tools.

**2. Proving Inodes and Dentries (`ls -i` and `stat`)**
Run the command: `touch original.txt`
Run the command: `ls -li original.txt` (The `-i` flag forces `ls` to print the Inode number).

- **Observation:** The first column is a massive integer (e.g., `1456723`). The third column (usually a `1`) is the **link count**.
  Run the deep inspection command: `stat original.txt`
- **Observation:** `stat` directly dumps the raw `struct stat` from the kernel. You will see the physical Block size, the exact Inode number, the specific UID and GID (both the integer and string name), and the granular Access/Modify/Change timestamps. Notice that the filename is just a superficial label at the top.

**3. Proving Hard Links and Inode Sharing (`ln`)**
Run the command: `ln original.txt hardlink.txt`
Run the command: `ls -li original.txt hardlink.txt`

- **Observation:** Both files have the **exact same Inode number** in the first column. Furthermore, their link count has physically increased to `2`.
- **Debugging Step:** Edit `hardlink.txt` using `nano` or `echo "hello" > hardlink.txt`. Run `cat original.txt`. The data is there. Delete `original.txt` via `rm original.txt`. Now run `cat hardlink.txt`. The data remains completely intact, because `rm` only deleted one Dentry, dropping the link count to 1, leaving the Inode and data blocks untouched.

**4. Proving Symbolic Links (`ln -s`)**
Run the command: `ln -s hardlink.txt softlink.txt`
Run the command: `ls -li softlink.txt hardlink.txt`

- **Observation:** `softlink.txt` has a completely **different Inode number**. Its link count is 1. Its permissions are usually `lrwxrwxrwx` (the `l` indicates it is a symlink).
- **Debugging Step:** Run `stat softlink.txt`. Look at the file size. If the filename "hardlink.txt" is 12 characters long, the size of the symlink on disk will be exactly 12 bytes. The entire data payload of the symlink is literally just the string "hardlink.txt".

---

## Part 3: Code Architecture & Deliberate Breakage

To witness how permissions actually work—and how they are wildly misunderstood—we will build a specific directory structure and then deliberately break it to prove that directory permissions override file permissions.

### The Architecture: Building the Trap

Open your terminal. You must run these commands as your normal user (we will use `sudo` only to simulate another user).

```bash
# 1. Create a directory structure
mkdir parent_dir
touch parent_dir/sensitive_data.txt
echo "Top Secret" > parent_dir/sensitive_data.txt

# 2. Lock down the file completely (Only owner can read/write)
chmod 600 parent_dir/sensitive_data.txt

# 3. Open up the directory completely (Everyone can do everything)
chmod 777 parent_dir

# 4. Prove the state
ls -l parent_dir/sensitive_data.txt
ls -ld parent_dir

```

### Deliberate Breakage 1: The Deletion Paradox

You own `sensitive_data.txt`. The permissions are `600` (`-rw-------`). If a malicious user logs in, they cannot read your file. They cannot write to your file.

**The Attack:**
We will simulate a malicious user named `nobody` (a standard unprivileged system account) attempting to destroy your file.
Run: `sudo -u nobody rm parent_dir/sensitive_data.txt`

**Observe the Logs/State:**
The system might prompt: `rm: remove write-protected regular file 'parent_dir/sensitive_data.txt'?`
Type `y` and hit enter.
Run: `ls parent_dir`
**The file is gone. It was successfully deleted by a user who had zero permissions on the file.**

**Why exactly did this break?**
Deleting a file _does not modify the file_. Deleting a file modifies the _Directory_ by removing the file's Dentry. Because you set the `parent_dir` permissions to `777` (world-writable), you gave every user on the system the exact right to add or remove Dentries from that directory's table. The kernel did not check the file's permissions because the file's data was not being accessed; the directory's data was being modified.

### Fixing the Breakage: The Sticky Bit

How does `/tmp` work? `/tmp` is `777` (world-writable) so all programs can use it, but if anyone can delete anything in a `777` directory, why can't users delete each other's temporary files?

The kernel provides a special 10th permission bit: **The Sticky Bit** (Restricted Deletion Flag).
Run: `mkdir sticky_dir`
Run: `chmod 1777 sticky_dir` (The `1` sets the Sticky bit).
Run: `ls -ld sticky_dir` (Notice the permissions are `drwxrwxrwt` – the `t` at the end).
Run: `touch sticky_dir/my_file.txt`
Run: `sudo -u nobody rm sticky_dir/my_file.txt`

**Observe the State:**
`rm: cannot remove 'sticky_dir/my_file.txt': Operation not permitted`.
When the kernel evaluates a directory with the `t` bit set, it completely overrides normal write logic. It enforces a strict new rule: You can only delete a file inside this directory if you are the UID owner of the file, OR the UID owner of the directory.

### Deliberate Breakage 2: The Blind Drop-Box

Can a user write a file into a directory that they cannot even see inside? Yes.
Run: `mkdir dropbox`
Run: `chmod 733 dropbox` (User: rwx, Group: -wx, Other: -wx)
Run: `ls -ld dropbox` (`drwx-wx-wx`)

**The Attack:**
Simulate the `nobody` user trying to look inside the box:
Run: `sudo -u nobody ls dropbox`
_Output:_ `ls: cannot open directory 'dropbox': Permission denied`. (They lack the `r` bit).

Simulate the `nobody` user dropping a file in:
Run: `sudo -u nobody touch dropbox/secret_drop.txt`
_Output:_ Silent success.
Run: `ls dropbox` (As your normal user, who has the `r` bit).
You will see `secret_drop.txt` exists.
**Why?** Because creating a file only requires Write (to modify the Dentry table) and Execute (to traverse into the directory). Reading the existing table was not required to append a new entry to it.

---

## Part 4: Record What You Learned

### What assumption is this system making?

The Unix permission model makes the absolute, uncompromising assumption that **a directory is nothing more than a discrete file containing a tabular list of names, and modifying that list has absolutely zero logical connection to the security of the files those names point to.**

The system assumes that if you grant someone Write access to a directory, you are explicitly granting them the god-like power to sever the connection between a filename and its inode within that specific namespace, regardless of who owns the actual data. It assumes that security is fundamentally hierarchical and cascading; a hyper-secure file (`000` permissions) sitting inside a world-writable directory is inherently insecure because its existence path can be manipulated. Finally, the system assumes that the UID of the running process (e.g., the shell executing `rm`) is the ultimate, non-forgeable cryptographic token used for bitwise AND operations against the inode's permission matrix. If the kernel's math says yes, the operation happens immediately, without any higher-level semantic checks of "intent."

---

### Capstone Project: Build a Custom Inode Inspector (`mini_stat`)

To deeply internalize the separation between human-readable filenames and the kernel's internal Inode data structures, you must interact directly with the kernel's system calls without using bash utilities.

**Your Assignment:**
Write a C program that replicates the core functionality of the `stat` command.

**Requirements:**

1. Your C program must accept a single filename as a command-line argument (e.g., `./mini_stat my_file.txt`).
2. You are strictly forbidden from using `system()`, `popen()`, or executing external bash tools.
3. You must include `<sys/stat.h>` and use the raw `stat()` system call to populate a `struct stat`.
4. Your program must print exactly the following information to the standard output, correctly formatted:

- **The Inode Number:** (extract `st_ino`).
- **The Link Count:** (extract `st_nlink`).
- **The Raw UID Integer:** (extract `st_uid`).
- **The Raw GID Integer:** (extract `st_gid`).
- **The File Size in Bytes:** (extract `st_size`).

5. **The Hard Part:** The `st_mode` field contains the file type and the 9 permission bits mashed together into a single 16-bit integer. You must write the bitwise AND logic (using the POSIX macros like `S_IRUSR`, `S_IWGRP`, etc.) to parse this integer and print the permissions as a familiar 9-character string (e.g., `rwxr-xr--`).
6. _Test it:_ Compile your program. Create a hard link to a file. Run `./mini_stat` on both the original and the hard link. Visually confirm that the Inode numbers are identical and the link count is 2.

**Why this is difficult:** You are crossing the boundary from user-space shell abstractions directly into the raw memory structures the OS kernel uses to track files. You must perform bitwise masking to extract the specific 9 bits of permissions from a larger integer, proving you understand exactly how `chmod 755` maps to physical binary zeros and ones in the system's memory.
