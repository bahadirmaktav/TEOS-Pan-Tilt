import logging
import tkinter as tk
import asyncio
from src.command_manager import Motor

logger = logging.getLogger(__name__)

class UiManager(tk.Tk):
  def __init__(self, event_loop, command_manager):
    super().__init__()
    self.event_loop = event_loop
    self.command_manager = command_manager

    # Set Window Title
    self.title("TEOS CONTROLLER")

    # Set Window Size
    self.resolution = 20
    self.width = self.resolution * 51
    self.height = self.resolution * 27
    screenwidth = self.winfo_screenwidth()
    screenheight = self.winfo_screenheight()
    alignstr = '%dx%d+%d+%d' % (self.width, self.height, (screenwidth - self.width) / 2, (screenheight - self.height) / 2)
    self.geometry(alignstr)
    self.resizable(width=False, height=False)

    # Control Display Seperation Line
    self.seperation_canvas = tk.Canvas(self, width=self.width, height=self.height)
    self.seperation_canvas.place(x=0, y=0)
    self.seperation_canvas.create_line(self.resolution * 17, 0, self.resolution * 17, self.height, fill="black")

    # Camera Controls
    self.camera_canvas = tk.Canvas(self, width=self.resolution*15, height=self.resolution*5)
    self.camera_canvas.place(x=self.resolution, y=self.resolution)
    self.camera_canvas.create_rectangle(0, 0, self.resolution*15, self.resolution*5, fill="gray", outline="black")
    self.camera_controls_label = tk.Label(self.camera_canvas, text="Camera Controls")
    self.camera_controls_label.place(x=75, y=10, width=150, height=38)
    self.start_camera_button = tk.Button(self.camera_canvas, text="Start Camera", command=lambda: self.event_loop.create_task(self.start_camera()))
    self.start_camera_button.place(x=48, y=58, width=100, height=32)
    self.stop_camera_button = tk.Button(self.camera_canvas, text="Stop Camera", command=lambda: self.event_loop.create_task(self.stop_camera()))
    self.stop_camera_button.place(x=152, y=58, width=100, height=32)

    # Motor Controls
    self.motor_canvas = tk.Canvas(self, width=self.resolution*15, height=self.resolution*7)
    self.motor_canvas.place(x=self.resolution, y=self.resolution*7)
    self.motor_canvas.create_rectangle(0, 0, self.resolution*15, self.resolution*7, fill="gray", outline="black")
    self.motor_controls_label = tk.Label(self.motor_canvas, text="Motor Controls")
    self.motor_controls_label.place(x=75, y=10, width=150, height=38)
    self.motor_controls_label = tk.Label(self.motor_canvas, text="Pan")
    self.motor_controls_label.place(x=15, y=58, width=50, height=32)
    self.pan_slider = tk.Scale(self.motor_canvas, from_=0, to=180, orient=tk.HORIZONTAL, command=lambda value: self.event_loop.create_task(self.rotate_pan(value)))
    self.pan_slider.place(x=85, y=58, width=200, height=32)
    self.motor_controls_label = tk.Label(self.motor_canvas, text="Tilt")
    self.motor_controls_label.place(x=15, y=98, width=50, height=32)
    self.pan_slider = tk.Scale(self.motor_canvas, from_=0, to=180, orient=tk.HORIZONTAL, command=lambda value: self.event_loop.create_task(self.rotate_tilt(value)))
    self.pan_slider.place(x=85, y=98, width=200, height=32)

    # WebSocket Connection Controls
    self.websocket_canvas = tk.Canvas(self, width=self.resolution*15, height=self.resolution*5)
    self.websocket_canvas.place(x=self.resolution, y=self.resolution*15)
    self.websocket_canvas.create_rectangle(0, 0, self.resolution*15, self.resolution*5, fill="gray", outline="black")
    self.websocket_connection_controls_label = tk.Label(self.websocket_canvas, text="WebSocket Connection Controls")
    self.websocket_connection_controls_label.place(x=25, y=10, width=250, height=38)
    self.connect_ws_button = tk.Button(self.websocket_canvas, text="Connect", command=lambda: self.event_loop.create_task(self.connect_websocket_server()))
    self.connect_ws_button.place(x=48, y=58, width=100, height=32)
    self.disconnect_ws_button = tk.Button(self.websocket_canvas, text="Disconnect", command=lambda: self.event_loop.create_task(self.disconnect_websocket_server()))
    self.disconnect_ws_button.place(x=152, y=58, width=100, height=32)

    # Mode Controls
    self.mode_canvas = tk.Canvas(self, width=self.resolution*15, height=self.resolution*5)
    self.mode_canvas.place(x=self.resolution, y=self.resolution*21)
    self.mode_canvas.create_rectangle(0, 0, self.resolution*15, self.resolution*5, fill="gray", outline="black")
    # TODO(MBM): Implement later.

    # Image Display
    self.display_canvas = tk.Canvas(self, width=self.resolution*16, height=self.resolution*12)
    self.display_canvas.place(x=self.resolution*26, y=self.resolution*7.5)
    self.display_canvas.create_rectangle(0, 0, self.resolution*16, self.resolution*12, outline="black")
    self.image_label = tk.Label(self)
    self.image_label.place(x=self.resolution*26, y=self.resolution*7.5, width=self.resolution*16, height=self.resolution*12)

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

  async def rotate_pan(self, value):
    logger.info("Rotate pan button clicked.")
    await self.command_manager.rotate_motor(Motor.PAN, int(value))

  async def rotate_tilt(self, value):
    logger.info("Rotate tilt button clicked.")
    await self.command_manager.rotate_motor(Motor.TILT, int(value))

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

  # Helper functions
  def validate_positive_float(self, event):
    try:
      # Try to convert the entry content to a float
      value = float(self.angle.get())

      # Check if the value is positive
      if value < 0:
        # If negative, set the entry value to 0
        self.angle.delete(0, tk.END)
        self.angle.insert(0, '0')
    except ValueError:
      # If the conversion to float fails or if it's not a positive number, set the entry value to an empty string
      self.angle.delete(0, tk.END)