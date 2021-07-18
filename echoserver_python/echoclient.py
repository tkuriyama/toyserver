
import asyncio # type: ignore
import time # type: ignore


################################################################################


async def tcp_echo_client():
    reader, writer = await asyncio.open_connection(
        '127.0.0.1', 8888)

    msg  = ''
    while msg.lower() != 'bye' and writer:
        msg = input('Enter message: ').strip()
        print(f'Send: {msg}')
        writer.write(msg.encode())
        await writer.drain()

        data = await reader.read(100)
        print(f'Received: {data.decode()!r}')

        time.sleep(0.0001)

    print('Close the connection')
    writer.close()
    await writer.wait_closed()


################################################################################


if __name__ == '__main__':
    asyncio.run(tcp_echo_client())
