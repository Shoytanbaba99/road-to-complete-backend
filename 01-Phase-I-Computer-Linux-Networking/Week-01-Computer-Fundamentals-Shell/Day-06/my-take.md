## 💡 Key Takeaways

Passwords and filepaths directly hardcoed into src code is bad practice, and insecure, and on the other hand forcing everything to parse differnet cofniguratino file is bad. so we have environement varialbe, key value paired,
when C calls execve(path, file,envp)

environment variables are top down so the parent creating process passes down its varialbes but child doesnt do the same and sends its variables to parent.

ther eis local shell variable creating var in terminal and exported ones where we write export and then var="" to export that variable. any process we run from this terminal will get this variable passed down to it.

when we run cat or ls, it goes through a list of env variable PATH list basically,
it checks from left to right and goes through every directory in the PAHT untill it finds one that matches with the initial comand like cat or ls like /usr/bin/cat or /usr/bin/ls and then executes that file.

running cat /proc/pid/environ will show all the environment variables for that process id where it had to traverse through the PATH variable to find the cat command and then execute it.

using which ls or which cat does the same and prints the first hit it gets from the PATH variable list.

there is this concept of SHEBASH #! and a path name that basically tells the executing process to use this exact path name first and run it. and if there is no executable there that matches the name then it crashes. essentially overriding the /.bashrc or the PATH variable list and forcing the process to use this specific path name.

there is also prepending a path name and maliciously putting a file with the same name as a command in the front of the PATH variable list so that when you run that command, it executes the malicious file instead of the intended one. as stated in deploy.sh
