import logging
import tkinter as tk
import asyncio

logger = logging.getLogger(__name__)

class UiManager(tk.Tk):
  def __init__(self, event_loop, command_manager):
    super().__init__()
    self.event_loop = event_loop
    self.command_manager = command_manager

    # set window title
    self.title("TEOS CONTROLLER")

    # set window size
    width=1000
    height=600
    screenwidth = self.winfo_screenwidth()
    screenheight = self.winfo_screenheight()
    alignstr = '%dx%d+%d+%d' % (width, height, (screenwidth - width) / 2, (screenheight - height) / 2)
    self.geometry(alignstr)
    self.resizable(width=False, height=False)

    # Camera Controls
    self.camera_controls_label = tk.Label(text="Camera Controls")
    self.camera_controls_label.place(x=0, y=0, width=150, height=40)
    self.start_camera_button = tk.Button(text="Start Camera", command=lambda: self.event_loop.create_task(self.start_camera()))
    self.start_camera_button.place(x=0, y=45, width=100, height=40)
    self.stop_camera_button = tk.Button(text="Stop Camera", command=lambda: self.event_loop.create_task(self.stop_camera()))
    self.stop_camera_button.place(x=105, y=45, width=100, height=40)

    # Motor Controls
    self.motor_controls_label = tk.Label(text="Motor Controls")
    self.motor_controls_label.place(x=0, y=105, width=150, height=40)
    self.angle = tk.Entry(self)
    self.angle.place(x=0, y=150, width=100, height=40)
    self.pan_rotate = tk.Button(text="Rotate Pan", command=lambda: self.event_loop.create_task(self.rotate_pan()))
    self.pan_rotate.place(x=105, y=150, width=100, height=40)
    self.tilt_rotate = tk.Button(text="Rotate Tilt", command=lambda: self.event_loop.create_task(self.rotate_tilt()))
    self.tilt_rotate.place(x=210, y=150, width=100, height=40)

    # WebSocket Connection Controls
    self.websocket_connection_controls_label = tk.Label(text="WebSocket Connection Controls")
    self.websocket_connection_controls_label.place(x=0, y=210, width=250, height=40)
    self.connect_ws_button = tk.Button(text="Connect", command=lambda: self.event_loop.create_task(self.connect_websocket_server()))
    self.connect_ws_button.place(x=0, y=255, width=100, height=40)
    self.disconnect_ws_button = tk.Button(text="Disconnect", command=lambda: self.event_loop.create_task(self.disconnect_websocket_server()))
    self.disconnect_ws_button.place(x=105, y=255, width=100, height=40)

    # Mode Controls
    # TODO(MBM): Implement later.

    # Image Display
    self.image_label = tk.Label(self)
    self.image_label.place(x=500, y=150, width=400, height=400)

  async def show(self):
    while True:
      self.update()
      await asyncio.sleep(.1)

  async def start_camera(self):
    logger.info("Start camera button clicked.")
    await self.command_manager.start_camera()

  async def stop_camera(self):
    logger.info("Stop camera button clicked.")
    await self.command_manager.stop_camera()  

  async def rotate_pan(self):
    logger.info("Rotate pan button clicked.")

  async def rotate_tilt(self):
    logger.info("Rotate tilt button clicked.")

  async def connect_websocket_server(self):
    logger.info("Connect to websocket server button clicked.")
    await self.command_manager.connect_websocket_server()

  async def disconnect_websocket_server(self):
    logger.info("Disconnect from websocket server button clicked.")
    await self.command_manager.disconnect_websocket_server()

  async def set_image_label(self, image):
    self.image_label.configure(image=image)
    self.image_label.image = image
    self.update_idletasks()