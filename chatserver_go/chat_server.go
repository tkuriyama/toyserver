package main

import (
	"bufio"
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"sync"
)

type Client struct {
	name   string
	conn   net.Conn
	writer io.Writer
}

type ChatServer struct {
	clients []Client
	buf     string
	mux     sync.Mutex
}

const SockAddr = "/tmp/chat.sock"

func main() {
	if err := os.RemoveAll(SockAddr); err != nil {
		log.Fatal(err)
	}

	l, err := net.Listen("unix", SockAddr)
	if err != nil {
		log.Fatal("Listen error:", err)
	}
	defer l.Close()

	var server = ChatServer{}

	for {
		conn, err := l.Accept()
		if err != nil {
			log.Fatal("Accept error:", err)
		}
		client := Client{"client", conn, bufio.NewWriter(conn)}
		register(&server, &client)
		go serve(&server, &client)
	}
}

func register(s *ChatServer, c *Client) {
	s.mux.Lock()
	defer s.mux.Unlock()

	s.clients = append(s.clients, *c)
	log.Printf("Client %s connected from [%s]", c.name,
		c.conn.RemoteAddr().Network())
}

func deregister(s *ChatServer, c *Client) {
	s.mux.Lock()
	defer s.mux.Unlock()

	for i, client := range s.clients {
		if *c == client {
			s.clients = append(s.clients[:i], s.clients[i+1:]...)
		}
	}
	log.Printf("Client %s disconnected", c.name)
}

func serve(s *ChatServer, c *Client) {
	r := bufio.NewReader(c.conn)
	defer c.conn.Close()

	for {
		msg, err := r.ReadString('\n')
		if err != nil && err != io.EOF {
			log.Printf("Read error for %s: %v", c.name, err)
			break
		}
		if msg == "bye\n" {
			log.Printf("Client %s requested disconnect\n", c.name)
			break
		} else {
			text := fmt.Sprintf("Message from %s: %s\n", c.name, msg)
			broadcast(s, text)
		}
	}

	deregister(s, c)
}

func broadcast(s *ChatServer, msg string) {
	s.mux.Lock()
	defer s.mux.Unlock()

	for _, client := range s.clients {
		client.writer.Write([]byte(msg))
	}
}
