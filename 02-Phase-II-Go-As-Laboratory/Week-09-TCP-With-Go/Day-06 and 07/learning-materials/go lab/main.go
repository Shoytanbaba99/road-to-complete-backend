package main

import (
	"bufio"
	"fmt"
	"net"
	"strconv"
	"strings"
	"time"
)

func main() {
	ln, err := net.Listen("tcp", "127.0.0.1:9090")
	if err != nil {
		fmt.Println("listen error:", err)
		return
	}
	defer ln.Close()
	fmt.Println("Server is listening on 127.0.0.1:9090")
	for {
		conn, err := ln.Accept()
		if err != nil {
			fmt.Println("accept error:", err)
			continue
		}
		go handleConnection(conn)
	}
}

func handleConnection(conn net.Conn) {
	defer conn.Close()

	inventory := map[string]int{}

	scanner := bufio.NewScanner(conn)
	scanner.Buffer(make([]byte, 1024), 1024)

	for {

		conn.SetReadDeadline(time.Now().Add(30 * time.Second))

		if !scanner.Scan() {

			if err := scanner.Err(); err != nil {
				fmt.Println("session closed:", err)
			}
			return
		}

		line := strings.TrimSpace(scanner.Text())
		if line == "" {
			continue
		}
		fields := strings.Fields(line)
		cmd := strings.ToUpper(fields[0])

		var resp string
		switch cmd {
		case "ADD":
			if len(fields) != 3 {
				resp = "ERR usage: ADD <item> <qty>"
				break
			}
			item := fields[1]
			qty, err := strconv.Atoi(fields[2])
			if err != nil || qty < 0 {
				resp = "ERR invalid quantity"
				break
			}
			inventory[item] += qty
			resp = fmt.Sprintf("OK %d", inventory[item])

		case "TAKE":
			if len(fields) != 3 {
				resp = "ERR usage: TAKE <item> <qty>"
				break
			}
			item := fields[1]
			qty, err := strconv.Atoi(fields[2])
			if err != nil || qty < 0 {
				resp = "ERR invalid quantity"
				break
			}
			if inventory[item] < qty {
				resp = "ERR insufficient stock"
				break
			}
			inventory[item] -= qty
			resp = fmt.Sprintf("OK %d", inventory[item])

		case "CHECK":
			if len(fields) != 2 {
				resp = "ERR usage: CHECK <item>"
				break
			}
			resp = fmt.Sprintf("OK %d", inventory[fields[1]])

		case "QUIT":
			fmt.Fprintln(conn, "OK bye")
			return

		default:
			resp = "ERR unknown command"
		}

		if _, err := fmt.Fprintln(conn, resp); err != nil {
			return
		}
	}
}
