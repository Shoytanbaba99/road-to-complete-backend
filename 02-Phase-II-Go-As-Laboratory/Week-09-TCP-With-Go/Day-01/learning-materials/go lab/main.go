package main

import (
	"fmt"
	"io"
	"net"
)

func main() {
	addr := "127.0.0.1:8888"

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

	remoteAddr := conn.RemoteAddr().String()
	localAddr := conn.LocalAddr().String()
	banner := fmt.Sprintf("Welcome, %s. You are connected to %s\n", remoteAddr, localAddr)
	if _, err := conn.Write([]byte(banner)); err != nil {
		fmt.Println("Error writing to connection:", err)
		return
	}

	buf := make([]byte, 1024)
	var totalBytes int
	for {
		n, err := conn.Read(buf)
		if err != nil {
			if err == io.EOF {
				fmt.Printf("[%s] disconnected cleanly. Total bytes: %d\n", remoteAddr, totalBytes)
			} else {
				fmt.Printf("[%s] error reading from connection: %v\n", remoteAddr, err)
			}
			return
		}
		totalBytes += n
		fmt.Printf("[%s] received %d bytes: %s\n", remoteAddr, n, string(buf[:n]))
	}
}
