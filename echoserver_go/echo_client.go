package main

import (
	"bufio"
	"fmt"
	"io"
	"log"
	"net"
	"os"
)

func reader(r io.Reader) {
	buf := make([]byte, 1024)
	n, err := r.Read(buf[:])
	if err != nil {
		return
	}
	println("Echo from server:", string(buf[0:n]))
}

func main() {
	c, err := net.Dial("unix", "/tmp/echo.sock")
	if err != nil {
		log.Fatal(err)
	}
	defer c.Close()

	var input string
	scanner := bufio.NewScanner(os.Stdin)

	fmt.Printf("Enter text to send to server; \"bye\" to quit...\n")
	for {
		fmt.Printf("\n> ")
		if scanner.Scan() {
			input = scanner.Text()
		}
		if input == "bye" {
			break
		}
		_, err = c.Write([]byte(input))
		if err != nil {
			log.Fatal("write error:", err)
		}
		reader(c)
	}
}
