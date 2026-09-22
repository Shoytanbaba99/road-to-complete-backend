package main

import (
	"bufio"
	"errors"
	"io"
	"log"
	"net"
	"strings"
	"time"
)

const IdleTimeout = 5 * time.Second

func main() {
	addr := "127.0.0.1:9002"
	listener, err := net.Listen("tcp", addr)
	if err != nil {
		log.Fatalf("Error starting TCP server: %v", err)
	}
	log.Printf("Timeout-protected server running on %s", addr)
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Printf("Error accepting connection: %v", err)
			continue
		}
		go handleConnection(conn)
	}
}

func handleConnection(conn net.Conn) {
	defer conn.Close()
	log.Printf("Client connected: %s", conn.RemoteAddr())
	greeting := "Hello! You are connected to the timeout-protected server.\n"
	_, err := conn.Write([]byte(greeting))
	if err != nil {
		log.Printf("Error sending greeting: %v", err)
		return
	}
	reader := bufio.NewReader(conn)
	for {
		err := conn.SetReadDeadline(time.Now().Add(IdleTimeout))
		if err != nil {
			log.Printf("Error setting read deadline: %v", err)
			return
		}
		line, err := reader.ReadString('\n')
		if err != nil {
			var netErr net.Error
			if errors.As(err, &netErr) && netErr.Timeout() {
				log.Printf("Client %s timed out due to inactivity", conn.RemoteAddr())
				conn.Write([]byte("You have been disconnected due to inactivity.\n"))
				return
			}
			if errors.Is(err, io.EOF) {
				log.Printf("Client %s disconnected", conn.RemoteAddr())
				return
			}
			log.Printf("Error reading from client %s: %v", conn.RemoteAddr(), err)
			return
		}

		text := strings.TrimSpace(line)
		if text == "exit" {
			log.Printf("Client %s requested to exit", conn.RemoteAddr())
			conn.Write([]byte("Goodbye!\n"))
			return
		}
		if text == "" {
			continue
		}

		if text == "SHUTDOWN" {
			log.Printf("Client %s requested shutdown", conn.RemoteAddr())

			conn.Write([]byte("Shutting down write channel...\n"))

			tcpConn, ok := conn.(*net.TCPConn)
			if !ok {
				log.Printf("Connection is not a TCP connection")
				return
			}

			if err := tcpConn.CloseWrite(); err != nil {
				log.Printf("Error closing write channel: %v", err)
				return
			}

		}
		log.Printf("[%s]: %s", conn.RemoteAddr(), text)
		response := "ACK: " + text + "\n"
		if _, err := conn.Write([]byte(response)); err != nil {
			log.Printf("Error sending response to client %s: %v", conn.RemoteAddr(), err)
			return
		}
	}
}
