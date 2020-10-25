package main

import (
	"io"
	"log"
	"net"
	"os"
)

const SockAddr = "/tmp/echo.sock"

func echoServer(c net.Conn) {
	log.Printf("Client Connected [%s]", c.RemoteAddr().Network())
	// copy until EOF or error
	io.Copy(c, c)
	log.Printf("Client connection closed [%s]", c.RemoteAddr().Network())
	c.Close()
}

func main() {
	if err := os.RemoveAll(SockAddr); err != nil {
		log.Fatal(err)
	}

	l, err := net.Listen("unix", SockAddr)
	if err != nil {
		log.Fatal("listen error:", err)
	}
	defer l.Close()

	for {
		conn, err := l.Accept()
		if err != nil {
			log.Fatal("accept error:", err)
		}

		go echoServer(conn)
	}
}
