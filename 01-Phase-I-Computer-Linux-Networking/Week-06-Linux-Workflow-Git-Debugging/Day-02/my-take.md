## 🧠 Core Mental Model

We got Git bisect and git reflog, reflog tracks the history of the HEAD and allows you to recover from catastrophic mistakes, it saves all the commits that were made, even if they are not reachable from any branch or tag, in local drive. While, git bisect is a binary search algorithm that helps you find the commit that introduced a bug by automatically checking out different commits and running a test command. It allows you to quickly narrow down the range of commits to find the one that caused the issue.
