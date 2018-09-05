## TCP 回射服务器

import asyncio

LISTEN_IP = '10.211.55.2'
LISTEN_IP = '192.168.0.100'
LISTEN_PORT = 8000


async def handle_echo(reader, writer):

	# try:
	while(True):
		data = await reader.read(100)
		addr = writer.get_extra_info('peername')
		
		
		message = data
		if message == b'':
			print("Close the client socket")
			writer.close()
			break        
		print("Received %r from %r" % (message, addr))

		#login 
		if data[13] == ord('0'): 
			payload = b'/0\x10\x00\x00\x00\x20\x00\x00\x00yooo!\x00\x02\x03\x04\x05\x01\x02\x03\x04\x05\x01\x02\x03\x04\x05\x01\x02\x03\x04\x05\x01\x02\x03\x04\x05'
			lenPayload = chr(len(payload)).encode()
			data = b'\x02\x00\x00\x00\x01\x00\x00\x00' +lenPayload+ b'\x00\x00\x00'+ payload
			writer.write(data)
			await writer.drain()
		#send file
		elif data[12] == ord('#'):
			payload = b'##192.168.0.101\x00'
			lenPayload = chr(len(payload)).encode()
			data = b'\x02\x00\x00\x00\x01\x00\x00\x00' +lenPayload+ b'\x00\x00\x00'+ payload
			writer.write(data)
			await writer.drain()
		# contacts
		elif data[13] == ord('2'):
			payload = b'/2\x10\x00\x00\x00\x20\x00\x00\x00pqqq!\x00\x02\x03\x04\x05\x01\x02\x03\x04\x05\x01\x02\x03\x04\x05\x01\x02\x03\x04\x05\x01\x02\x03\x04\x05'+ \
				b'\x30\x00\x00\x00\x40\x00\x00\x00rsss!\x00\x02\x03\x04\x05\x01\x02\x03\x04\x05\x01\x02\x03\x04\x05\x01\x02\x03\x04\x05\x01\x02\x03\x04\x05'
			lenPayload = chr(len(payload)).encode()
			data = b'\x02\x00\x00\x00\x01\x00\x00\x00' +lenPayload+ b'\x00\x00\x00'+ payload
			writer.write(data)
			await writer.drain()
		print("Sent %r to %r" % (data, addr))
		
		
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
