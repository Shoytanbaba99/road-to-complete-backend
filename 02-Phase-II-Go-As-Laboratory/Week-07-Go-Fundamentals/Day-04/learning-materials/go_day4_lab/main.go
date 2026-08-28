package main

import "fmt"

type Packet struct {
	ID         int
	Payload    string
	Checksum   int
	IsVerified bool
}

func NewPacket(id int, payload string) *Packet {
	return &Packet{
		ID:         id,
		Payload:    payload,
		Checksum:   len(payload),
		IsVerified: false,
	}
}

func (p *Packet) Verify(expectedChecksum int) bool {
	if p.Checksum == expectedChecksum {
		p.IsVerified = true
		return true
	}
	return false
}

func (p Packet) Display() string {
	return fmt.Sprintf("ID: %d, Payload: %s, Verified: %t", p.ID, p.Payload, p.IsVerified)
}

func main() {

	packet := NewPacket(1, "Ping")

	fmt.Println("Initial State")
	fmt.Println(packet.Display())
	fmt.Println()

	fmt.Println("Packet Verification")
	verified := packet.Verify(len(packet.Payload))
	fmt.Printf("Verification Result: %t\n", verified)
	fmt.Println()

	fmt.Println("After Verification")
	fmt.Println(packet.Display())
	fmt.Println()

	fmt.Println("=== Testing Wrong Checksum ===")
	packet2 := NewPacket(2, "Hello")
	fmt.Printf("Before: %s\n", packet2.Display())
	verified2 := packet2.Verify(99)
	fmt.Printf("Verification result: %t\n", verified2)
	fmt.Printf("After: %s\n", packet2.Display())

}
