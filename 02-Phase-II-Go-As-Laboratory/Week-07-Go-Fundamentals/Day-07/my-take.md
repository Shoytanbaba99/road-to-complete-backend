Learned that instead of enum you use

type TaskStatus string
const {
Pending TaskStatus = "Pending"
Completed TaskStatus = "Completed"
}

something liek this, so creating a TaskStatus type and then creating constants for the different statuses.

and You also have sort.Slice which is a function that takes a slice and a function and sorts the slice based on the function you provide.
