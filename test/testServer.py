## TCP 回射服务器

import asyncio

LISTEN_IP = '10.211.55.2'
LISTEN_PORT = 8888


async def handle_echo(reader, writer):

    # try:
    while(True):
        data = await reader.read(100)
        writer.write(data)
        message = data.decode()

        if message == '':
            print("Close the client socket")
            writer.close()
            break

        addr = writer.get_extra_info('peername')
        print("Received %r from %r" % (message, addr))

        #print("Send: %r" % message)
        
        await writer.drain()
    # except Exception as e:
    #    throw(e)

    #print("Close the client socket")
    # writer.close()

loop = asyncio.get_event_loop()
coro = asyncio.start_server(handle_echo, LISTEN_IP, LISTEN_PORT, loop=loop)
server = loop.run_until_complete(coro)

# Serve requests until Ctrl+C is pressed
print('Serving on {}'.format(server.sockets[0].getsockname()))
try:
    loop.run_forever()
except KeyboardInterrupt:
    pass

# Close the server
server.close()
loop.run_until_complete(server.wait_closed())
loop.close()
