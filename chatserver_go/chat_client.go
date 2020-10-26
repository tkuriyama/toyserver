package main

import (
	"bufio"
	"fmt"
	"io"
	"log"
	"net"
	"os"
)

func main() {
	c, err := net.Dial("unix", "/tmp/chat.sock")
	if err != nil {
		log.Fatal(err)
	}
	defer c.Close()

	r := bufio.NewReader(c)
	go reader(r)
	//go repl(&c)
	var input string
	scanner := bufio.NewScanner(os.Stdin)
	fmt.Printf("Connected to chat server...\n")
	for {
		if scanner.Scan() {
			input = scanner.Text()
		}
		text := fmt.Sprintf("%s\n", input)
		_, err := c.Write([]byte(text))
		if err != nil {
			log.Fatal("Error writing to server:", err)
		}

		if input == "bye" {
			break
		}
	}
}

func reader(r *bufio.Reader) {
	for {
		msg, err := r.ReadString('\n')
		if err != nil && err != io.EOF {
			log.Printf("Read error: %v", err)
			break
		}
		log.Printf("%s", msg)
	}
}
