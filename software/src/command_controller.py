from src.websocket_client import WebSocketClient
from enum import Enum

# Enum definitions
class Motor(Enum):
  PAN = 1
  TILT = 2

# Variable definitions
COMMAND_LENGTH = 8

class CommandController:
  def __init__(self, client):
    self.client = client

  # Camera controls
  async def start_camera(self):
    start_camera_command = bytearray(COMMAND_LENGTH)
    start_camera_command[0] = 0xFF
    start_camera_command[1] = 0x01
    start_camera_command[2] = 0x01
    await self.client.send_message(start_camera_command)
    # TODO(MBM): Start a thread to receive camera image data from websocket server and show it on window with opencv. 

  async def stop_camera(self):
    print("Stop camera command has not been implemented yet.")

  async def configure_camera(self):
    print("Configure camera command has not been implemented yet.")

  # Motor controls
  async def rotate_motor(self, motor: Motor, angle):
    rotate_command = bytearray(COMMAND_LENGTH)
    rotate_command[0] = 0xFF
    rotate_command[1] = 0x01
    rotate_command[2] = motor.value
    rotate_command[3] = 0x01
    angle = int(angle * 100)
    rotate_command[4] = (angle >> 8) & 0xFF
    rotate_command[5] = angle & 0xFF
    rotate_command[7] = 0x00
    await self.client.send_message(rotate_command)

  async def stop_motor(self, motor: Motor):
    print("Stop motor command has not been implemented yet.")

  # Mode controls
  async def set_manual_mode(self):
    print("Set manual mode command has not been implemented yet.")

  async def set_auto_mode(self):
    print("Set auto mode command has not been implemented yet.")

  # Websocket connection controls
  async def close_websocket_conenction(self):
    await self.client.close()