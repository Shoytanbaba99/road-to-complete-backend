package main

import (
	"bufio"
	"net"
	"strings"
)

func main() {
	addr := "127.0.0.1:9999"
	listener, err := net.Listen("tcp", addr)
	if err != nil {
		panic(err)
	}
	defer listener.Close()

	for {
		conn, err := listener.Accept()
		if err != nil {
			panic(err)
		}

		handleConnection(conn)
	}
}

func handleConnection(conn net.Conn) {
	defer conn.Close()
	scanner := bufio.NewScanner(conn)
	for scanner.Scan() {
		line := scanner.Text()
		parts := strings.Fields(line)
		if len(parts) == 0 {
			continue
		}
		switch parts[0] {
		case "UPPER":
			if len(parts) < 2 {
				conn.Write([]byte("Error: UPPER command requires an argument\n"))
				continue
			}
			response := strings.ToUpper(strings.Join(parts[1:], " "))
			conn.Write([]byte(response + "\n"))
		case "REVERSE":
			if len(parts) < 2 {
				conn.Write([]byte("Error: REVERSE command requires an argument\n"))
				continue
			}
			response := strings.Join(parts[1:], " ")
			response = reverseString(response)
			conn.Write([]byte(response + "\n"))
		case "QUIT":
			conn.Write([]byte("Goodbye!\n"))
			return
		default:
			conn.Write([]byte("Error: Unknown command\n"))
		}
	}
}

func reverseString(s string) string {
	runes := []rune(s)
	for i, j := 0, len(runes)-1; i < j; i, j = i+1, j-1 {
		runes[i], runes[j] = runes[j], runes[i]
	}
	return string(runes)
}
