import logging
from enum import Enum

logger = logging.getLogger(__name__)

# Enum definitions
class Motor(Enum):
  PAN = 1
  TILT = 2

# Variable definitions
COMMAND_LENGTH = 8

class CommandManager:
  def __init__(self, websocket_client, camera_manager):
    self.websocket_client = websocket_client
    self.camera_manager = camera_manager

  # Camera controls
  async def start_camera(self):
    logger.info("Start camera command called.")
    start_camera_command = bytearray(COMMAND_LENGTH)
    start_camera_command[0] = 0xFF
    start_camera_command[1] = 0x01
    start_camera_command[2] = 0x01
    start_camera_command[3] = 0x00
    start_camera_command[4] = 0x00
    start_camera_command[5] = 0x00
    start_camera_command[6] = 0x00
    start_camera_command[7] = 0x00
    await self.websocket_client.send_message(start_camera_command)
    await self.camera_manager.start_receiving_data()

  async def stop_camera(self):
    logger.info("Stop camera command has not been implemented yet.")

  async def configure_camera(self):
    logger.info("Configure camera command has not been implemented yet.")

  # Motor controls
  async def rotate_motor(self, motor: Motor, angle):
    logger.info("Rotate motor command called.")
    rotate_command = bytearray(COMMAND_LENGTH)
    rotate_command[0] = 0xFF
    rotate_command[1] = 0x02
    rotate_command[2] = motor.value
    rotate_command[3] = 0x01
    angle = int(angle * 100)
    rotate_command[4] = (angle >> 8) & 0xFF
    rotate_command[5] = angle & 0xFF
    rotate_command[7] = 0x00
    await self.websocket_client.send_message(rotate_command)

  async def stop_motor(self, motor: Motor):
    logger.info("Stop motor command has not been implemented yet.")

  # Mode controls
  async def set_manual_mode(self):
    logger.info("Set manual mode command has not been implemented yet.")

  async def set_auto_mode(self):
    logger.info("Set auto mode command has not been implemented yet.")

  # Websocket connection controls
  async def connect_websocket_server(self):
    logger.info("Connect to websocket server command called.")
    await self.websocket_client.connect()
  
  async def disconnect_websocket_server(self):
    logger.info("Disconnect from websocket server command called.")
    await self.websocket_client.close()