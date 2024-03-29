import asyncio # type: ignore


################################################################################


async def handle_echo(reader, writer):

    while True:
        data = await reader.read(100)
        message = data.decode()
        addr = writer.get_extra_info('peername')

        print(f"Received {message!r} from {addr!r}")

        print(f"Send: {message!r}")
        writer.write(data)
        await writer.drain()

        if message.lower().strip() == 'bye':
            print("Close the connection")
            writer.close()
            break


async def main():
    server = await asyncio.start_server(
        handle_echo, '127.0.0.1', 8888)

    addr = server.sockets[0].getsockname()
    print(f'Serving on {addr}')

    async with server:
        await server.serve_forever()


################################################################################


if __name__ == '__main__':
    asyncio.run(main())
