import asyncio
import websockets
import binascii

class WebSocketClient:
  def __init__(self, uri):
    self.uri = uri
    self.websocket = None

  async def connect(self):
    self.websocket = await websockets.connect(self.uri)
    print(f"Connected to {self.uri}")

  async def send_message(self, message):
    await self.websocket.send(message)
    print(f"Sent message: {message}")

  async def receive_messages(self):
    while True:
      message = await self.websocket.recv()
      print(f"Received message: {message}")

  async def close(self):
    await self.websocket.close()
    print("Connection closed")