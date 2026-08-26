package main

import (
	"fmt"
	"reflect"
	"unsafe"
)

func check() {
	fmt.Println("=== 1. SLICE HEADER & REALLOCATION PROOF ===")
	// Create a slice with len=2, cap=2
	s := make([]int, 2, 2)
	s[0] = 10
	s[1] = 20

	// Inspect the slice header directly in memory
	header := (*reflect.SliceHeader)(unsafe.Pointer(&s))
	fmt.Printf("Initial Slice -> Addr: 0x%x, Len: %d, Cap: %d\n", header.Data, header.Len, header.Cap)

	// Append within capacity limit (will trigger reallocation)
	s = append(s, 30)
	newHeader := (*reflect.SliceHeader)(unsafe.Pointer(&s))
	fmt.Printf("Post-Append   -> Addr: 0x%x, Len: %d, Cap: %d\n", newHeader.Data, newHeader.Len, newHeader.Cap)

	if header.Data != newHeader.Data {
		fmt.Println(">> Reallocation occurred: Underlying backing array moved to a new memory address!")
	}

	fmt.Println("\n=== 2. STRINGS, BYTES, AND UTF-8 RUNES ===")
	// "Go" + Rocket emoji (🚀 is 4 bytes) + Japanese character '日' (3 bytes)
	sampleStr := "Go🚀日"

	fmt.Printf("String: %s\n", sampleStr)
	fmt.Printf("len(sampleStr) in bytes: %d\n", len(sampleStr))

	strHeader := (*reflect.StringHeader)(unsafe.Pointer(&sampleStr))
	fmt.Printf("String Header -> Data Pointer: 0x%x, Len: %d\n", strHeader.Data, strHeader.Len)

	fmt.Println("\n--- Byte-by-Byte Traversal (Raw uint8) ---")
	for i := 0; i < len(sampleStr); i++ {
		fmt.Printf("Byte %d: 0x%X (%q)\n", i, sampleStr[i], sampleStr[i])
	}

	fmt.Println("\n--- Range Loop Traversal (UTF-8 Decoded Runes) ---")
	for byteOffset, runeVal := range sampleStr {
		fmt.Printf("Offset %d: Rune %U (%c) [Size: %d bytes]\n",
			byteOffset, runeVal, runeVal, utf8RuneSize(runeVal))
	}
}
func main() {
	check()

	cache := NewCache()
	cache.Put("user_101", []byte("Active"))

	data, err := cache.Get("user_101")
	if err == nil {
		fmt.Printf("Retrieved: %s\n", string(data))
	}

	Simple_Counter()

}

func utf8RuneSize(r rune) int {
	return len(string(r))
}
