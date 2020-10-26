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
	name string
	conn net.Conn
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
		client := Client{"default", conn} //, bufio.NewWriter(conn)}
		client.conn.Write([]byte("You are now connected to chat server.\n"))
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
		log.Printf("Client %s sent: %s", c.name, msg)
		if msg == "bye\n" {
			log.Printf("Client %s requested disconnect\n", c.name)
			break
		} else if len(msg) > 6 && msg[:4] == "name" {
			name := msg[5:(len(msg) - 1)]
			log.Printf("Client %s set name to %s", c.name, name)
			c.name = name
		} else {
			text := fmt.Sprintf("%s says: %s\n", c.name, msg)
			go broadcast(s, &text)
		}
	}

	deregister(s, c)
}

func broadcast(s *ChatServer, msg *string) {
	log.Printf("Server has %d clients", len(s.clients))
	log.Printf("Broadcasting: %s", *msg)
	for _, client := range s.clients {
		_, err := client.conn.Write([]byte(*msg))
		if err != nil {
			log.Printf("Write error: %v", err)
		}
	}
}
