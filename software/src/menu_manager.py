from software.src.command_manager import Motor

class MenuManager:
  def __init__(self, command_manager):
    self.command_manager = command_manager

  def main_menu(self):
    print("\n######### Main Menu #########")
    print("1. Go to camera control settings.")
    print("2. Go to motor control settings.")
    print("3. Go to mode control settings.")
    print("0. Exit\n")

  def motor_control_menu(self):
    print("\nSelect the motor you want to rotate.")
    print("1. PAN")
    print("2. TILT")
    print("0. Go back to the main menu.\n")

  def motor_control_submenu(self):
    print("\n######### Motor Control Menu #########")
    print("1. Enter the angle you want to adjust the motor with the command.(Example: 1 45 : rotate to 45 degree)")
    print("0. Go back to the motor control menu.\n")

  def camera_control_menu(self):
    print("\n######### Camera Control Menu #########")
    print("1. Start the camera.")
    print("2. Stop the camera.")
    print("0. Go back to the main menu.\n")

  async def execute(self):
    while True:
      self.main_menu()
      choice = input("Enter your choice: ")
      if choice == '1':
        # Camera Control Menu
        while True:
          self.camera_control_menu()
          camera_control_choice = input("Enter your choice: ")
          if camera_control_choice == '1':
            await self.command_manager.start_camera()
          elif camera_control_choice == '2':
            await self.command_manager.stop_camera()
          elif camera_control_choice == '0':
            break
          else:
            print("Invalid choice. Please try again.")
      elif choice == '2':
        # Motor Control Menu
        while True:
          self.motor_control_menu()
          motor_choice = input("Enter your choice: ")
          if motor_choice == '1' or motor_choice == '2':
            while True:
              self.motor_control_submenu()
              motor_control_choice = input("Enter your choice: ")
              if motor_control_choice.startswith('1'):
                option, angle = map(int, motor_control_choice.split())
                if isinstance(option, int) and isinstance(angle, int):
                  print(f"Input type: Both numbers are integers ({option}, {angle})")
                else:
                  print("Input type: Not both numbers are integers.")
                  print("Invalid rotation command entered. Please try again.")
                  break
                motor = Motor(int(motor_choice))
                print(f"{motor.name} will be rotated to {abs(angle)} degrees.")
                await self.command_manager.rotate_motor(motor, abs(angle))
              elif motor_control_choice == '0':
                break
              else:
                print("Invalid choice. Please try again.")
          elif motor_choice == '0':
            break
          else:
            print("Invalid choice. Please try again.")
      elif choice == '3':
        # Implement mode control settings
        print("Mode control settings have not been implemented yet.")
        pass
      elif choice == '0':
        await self.command_manager.close_websocket_conenction()
        break
      else:
        print("Invalid choice. Please try again.")