from src.menu_controller import MenuController
from src.command_controller import CommandController
from src.websocket_client import WebSocketClient
import configs
import asyncio

if __name__ == "__main__":
  client = WebSocketClient(uri=configs.websocket_uri)
  command_controller = CommandController(client)
  menu_controller = MenuController(command_controller)

  async def main():
    await client.connect()
    await menu_controller.execute()

  asyncio.run(main())