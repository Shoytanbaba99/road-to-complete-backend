## 🧠 Core Mental Model

linux uses a single directory and other directories are mounted in this tree.

key directories for linux are:
/bin for binaries
sbin for system binaries
/etc for configuration files
/var for variable data logging or datrabase
/dev contains pointers to hardware devices
proc and sys is responsible for mapping virtual filesystem to ram.

INODE is a datastructure containing all the metadata about a file. Inode doesnt have teh file name it only has a id that points to the array of physical disk block addresses.
Dentry is responsible for mapping the inode to HR strings/filenames

soft link creates a new inode which contains the name of the filename, while hardlink creates a new dentry that points to the same inode.

Ownership 000 (r,w,x) first index is owner, second is group, and third is others.
r = 4
w = 2
x = 1

file 777 means anyone can read file, write/modify file, and run it as process
directory 777 means anyone can read whats in the directory, write/modify files in the directory, and enter the directory.

files are vulnerable even if they have 000, if directory has write and executer permission, anyone can sever the link between the Dentry and inode of that file.

there is also a sticky bit, 1777 which allows onl the owner to be able to delete.

## 🔬 Practical Lab Findings
