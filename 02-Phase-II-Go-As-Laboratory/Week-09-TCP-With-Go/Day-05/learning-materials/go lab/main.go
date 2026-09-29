package main

import (
	"bufio"
	"errors"
	"log"
	"net"
	"time"
)

const (
	ReadTimeout   = 10 * time.Second
	WriteTimeout  = 5 * time.Second
	MaxlineLength = 64 * 1024
)

func main() {
	port := "8080"
	listener, err := net.Listen("tcp", ":"+port)
	if err != nil {
		panic(err)
	}
	log.Printf("Server listening on port %s", port)
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Printf("Error accepting connection: %v", err)
			continue
		}
		log.Printf("Connection accepted from %s", conn.RemoteAddr())
		go handleConnection(conn)
	}
}
func handleConnection(conn net.Conn) {
	defer conn.Close()

	scanner := bufio.NewScanner(conn)
	scanner.Buffer(nil, MaxlineLength)

	for {
		conn.SetReadDeadline(time.Now().Add(ReadTimeout))

		if !scanner.Scan() {
			err := scanner.Err()
			if errors.Is(err, bufio.ErrTooLong) {
				log.Printf("Client %s sent oversized line", conn.RemoteAddr())
				conn.SetWriteDeadline(time.Now().Add(WriteTimeout))
				conn.Write([]byte("ERR line too long\n"))
			} else if err != nil {
				log.Printf("Read error from %s: %v", conn.RemoteAddr(), err)
			} else {
				log.Printf("Client %s disconnected", conn.RemoteAddr())
			}
			return
		}

		line := scanner.Text()
		log.Printf("Received from %s: %s", conn.RemoteAddr(), line)

		conn.SetWriteDeadline(time.Now().Add(WriteTimeout))
		response := "ECHO: " + line + "\n"
		if _, err := conn.Write([]byte(response)); err != nil {
			log.Printf("Write error to %s: %v", conn.RemoteAddr(), err)
			return
		}
	}
}
