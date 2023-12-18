import logging
import websockets
from websockets.exceptions import ConnectionClosed

logger = logging.getLogger(__name__)

class WebSocketClient:
  def __init__(self, uri):
    self.uri = uri
    self.websocket = None

  async def connect(self):
    try:
      self.websocket = await websockets.connect(self.uri)
      logger.info(f"Connected to {self.uri}")
    except ConnectionClosed as e:
      logger.error(f"Failed to connect to {self.uri}. Reason: {e.reason}")   

  async def send_message(self, message):
    if not self.websocket.open:
      logger.error("WebSocket connection is not open!")
      return
    await self.websocket.send(message)
    logger.info(f"Sent message: {message}")

  async def receive_message(self):
    return await self.websocket.recv()

  async def close(self):
    await self.websocket.close()
    logger.info("Connection closed")