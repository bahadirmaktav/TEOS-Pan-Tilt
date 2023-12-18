import asyncio
import numpy as np
import cv2
from PIL import Image, ImageTk

class CameraManager:
  def __init__(self, websocket_client):
    self.websocket_client = websocket_client
    self.stop_event = asyncio.Event()

  def set_ui_manager(self, ui_manager):
    self.ui_manager = ui_manager

  async def receive_camera_data(self):
    while True:
      # Receive JPEG image data from the server
      received_data = await self.websocket_client.receive_message()
      jpeg_array = np.frombuffer(received_data, dtype=np.uint8)
      image = cv2.imdecode(jpeg_array, cv2.IMREAD_COLOR)

      # Convert OpenCV image to ImageTk format
      image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
      image = Image.fromarray(image)
      image = ImageTk.PhotoImage(image)

      await self.ui_manager.set_image_label(image)

  async def start_camera_data_receive_thread(self):
    loop = asyncio.get_event_loop()
    loop.run_in_executor(None, self.receive_camera_data)