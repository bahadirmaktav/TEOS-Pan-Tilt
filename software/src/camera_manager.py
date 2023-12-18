import logging
import numpy as np
import cv2
from PIL import Image, ImageTk

logger = logging.getLogger(__name__)

class CameraManager:
  def __init__(self, event_loop, websocket_client):
    self.event_loop = event_loop
    self.websocket_client = websocket_client
    self.stop_receiving = False

  def set_ui_manager(self, ui_manager):
    self.ui_manager = ui_manager

  async def stop_receiving_data(self):
    self.stop_receiving = True

  async def receive_camera_data(self):
    while not self.stop_receiving:
      # Receive JPEG image data from the server
      received_data = await self.websocket_client.receive_message()
      jpeg_array = np.frombuffer(received_data, dtype=np.uint8)
      image = cv2.imdecode(jpeg_array, cv2.IMREAD_COLOR)

      # Convert OpenCV image to ImageTk format
      image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
      image = Image.fromarray(image)
      image = ImageTk.PhotoImage(image)

      # Set image to tkinter label to show on window
      await self.ui_manager.set_image_label(image)
      # logger.info("image data received")
    logger.info("Receiving camera image data thread stopped.")

  async def start_receiving_data(self):
    self.stop_receiving = False
    self.event_loop.create_task(self.receive_camera_data())
    logger.info("Receiving camera image data thread started.")
