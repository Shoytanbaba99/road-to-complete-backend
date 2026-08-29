So, in Go, The concept of interfeace is a little ibt different, it is not like the traditional OOP interface, where you have to explicitly declare that a type implements an interface. In typescript you can have datas and methods both in the interface, but in Go, you can only have methods in the interface. Any type that has the same method as the interface is said to implement that interface, This is implicit implementation.

So, it would be something like

```go
type Person interface {
    Greet() string
}
type Student struct {
    Name string
    Age int
}
func (s Student) Greet() string{
    return fmt.Sprintf("Hello, my name is %s and I am %d years old.", s.Name, s.Age)
}
```

The packages here, anything starting with capital letter is Eported so, Name and Age will be exported, and if i had written name and age, they would not be exported.

There is also the concept of a struct being inside another struct,

```go

type Address struct{
    Street string
    City string
    State string
    ZipCode string
}

type Student struct{
    Name string
    Age int
    Address Address
}
```

Also the idea of instance for interface, if you initialise or instantiate a struct, you have access to every other method and its variables. But, interface allows you to expose only the methods that are defined in the interface.
