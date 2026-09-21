package main

import (
	"bufio"
	"log"
	"net"
	"runtime"
	"strings"
	"time"
)

func main() {
	address := "127.0.0.1:9999"

	listener, err := net.Listen("tcp", address)
	if err != nil {
		panic(err)
	}
	defer listener.Close()

	log.Printf("Server is listening on %s\n", address)
	log.Printf("[Diagnostics] Initial Goroutine: %d\n", runtime.NumGoroutine())

	go func() {
		for {
			time.Sleep(5 * time.Second)
			log.Printf("[Diagnostics] Active Goroutine: %d\n", runtime.NumGoroutine())
		}
	}()

	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Printf("Error accepting connection: %v\n", err)
			continue
		}
		log.Printf("Accepted connection from %s\n", conn.RemoteAddr().String())
		go handleConnection(conn)
	}

}

func handleConnection(conn net.Conn) {
	defer conn.Close()
	remoteAddr := conn.RemoteAddr().String()
	log.Printf("[+] Connect: %s | Goroutines: %d", remoteAddr, runtime.NumGoroutine())

	conn.Write([]byte("Connected to Concurrent Echo Server. Type commands or Quit.\n"))
	scanner := bufio.NewScanner(conn)
	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		if line == "" {
			continue
		}
		if line == "QUIT" {
			conn.Write([]byte("Goodbye!\n"))
			log.Printf("[-] Disconnect: %s | Goroutines: %d", remoteAddr, runtime.NumGoroutine())
			return
		}
		response := "Echo: " + line + "\n"
		if _, err := conn.Write([]byte(response)); err != nil {
			log.Printf("Error writing to connection: %v\n", err)
			return
		}
	}
	if err := scanner.Err(); err != nil {
		log.Printf("Error reading from connection: %v\n", err)
	}
	log.Printf("[-] Disconnect: %s | Goroutines: %d", remoteAddr, runtime.NumGoroutine())
}
