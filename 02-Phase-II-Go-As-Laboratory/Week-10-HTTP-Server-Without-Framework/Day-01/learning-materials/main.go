package main

import (
	"fmt"
	"net/http"
)

func main() {
	http.HandleFunc("/status", statusHandler)
	http.HandleFunc("/owner", ownerHandler)
	fmt.Println("Server is listening on http://localhost:8080")
	http.ListenAndServe(":8080", nil)
}

func statusHandler(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintln(w, "Server is running...")
	fmt.Fprintln(w, "r.Method: and r.RemoteAddr:", r.Method, r.RemoteAddr)
}
func ownerHandler(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintln(w, "Owner: John Doe")
}
