import websockets
from websockets.exceptions import ConnectionClosed

class WebSocketClient:
  def __init__(self, uri):
    self.uri = uri
    self.websocket = None

  async def connect(self):
    try:
      self.websocket = await websockets.connect(self.uri)
      print(f"Connected to {self.uri}")
    except ConnectionClosed as e:
      print(f"Failed to connect to {self.uri}. Reason: {e.reason}")   

  async def send_message(self, message):
    if not self.websocket.open:
      print("WebSocket connection is not open!")
      return
    await self.websocket.send(message)
    print(f"Sent message: {message}")

  async def receive_message(self):
    return await self.websocket.recv()
    # print(f"Received message: {message}")

  async def close(self):
    await self.websocket.close()
    print("Connection closed")