In go, the struct has zero memory overload, empty struct takes 0 bytes of memory. So whenever any memory is allocated, it zeroes out anything inside it. For example, if you have a struct with an int and a string, when you create an instance of that struct, the int will be initialized to 0 and the string will be initialized to an empty string.

Pointers like \*T stores the address of a value of type T
&x takes the address of x and returns a pointer to it.

\*p is used to dereference a pointer p, which means accessing the value that the pointer points to.

also you cant do ptr++ like in c.

Go has methods, the same methods which are in c inside classes, but in go you can define outside of a struct/class. Two types of method, one is value receiver and otehr is pointer reciever.

Value reciever: func (a Account) Balance() float64
Pointer reciever: func (a \*Account) Deposit(amount float64)

(a Account) here is the receiver, which is similar to the 'this' keyword in other languages. It allows you to call the method on an instance of the struct.
