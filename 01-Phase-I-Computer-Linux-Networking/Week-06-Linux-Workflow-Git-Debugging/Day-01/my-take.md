## 🧠 Core Mental Model

The Objects of git, you got the blob, tree, commit, and ref. The blob is the content of a file, the tree is a directory structure, the commit is a snapshot of the project at a point in time, and the ref is a pointer to a commit.

Doing git add stages the files, compresses then with zlib, and stores them in the .git/objects directory. The SHA-1 hash of the compressed content is used as the key to store the blob. The tree contains the snapshot, or the SHA1 hash of the blobs, and a SHA1 hash of the tree itself. The commit contains the SHA1 hash of the tree, the SHA1 hash of the parent commit, author and committer info, and the commit message. The ref, which is branch or tag, is a human-readable pointer to a commit hash.

The merge command finds the common ancestor of two branches, compares the changes, and creates a new merge commit with two parent pointers. The rebase command unplugs a sequence of commits from one part of the graph and replays them on top of another, creating new commits with new SHA-1 hashes.
