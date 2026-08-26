package main

import (
	"fmt"
	"unicode/utf8"
)

func Simple_Counter() {
	words := []string{"apple", "banana", "apple", "orange", "banana", "apple"}
	wordCount := make(map[string]int)

	for _, word := range words {
		wordCount[word] = wordCount[word] + 1
	}

	emojiString := "Hello 🌍"

	fmt.Println("Word Counts")
	for word, count := range wordCount {
		fmt.Printf("%s:%d\n", word, count)
	}

	fmt.Println("RAW BYTE(len) %d \n", len(emojiString))
	fmt.Printf("Human Crahcter Count: %d \n", utf8.RuneCountInString(emojiString))
}
