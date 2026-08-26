So the arrays are fixed size in go. and you have splices which has len and cap, len is how much the splice can see and cap is how muc hthe storage has before it need to be reallocated.

Then you have maps which are key value pairs and you can use the make function to create a map. You cant leave it uninitialized because it will be nil and you cant add any key value pairs to it.

Then you have strings, normal typical strings are 1 byte but special strings has random bytes. A rune is a int32 (4 bytes) and it is used to represent a unicode character. We use it when we need to represent a character that is not in the ASCII range.
